#include <Connection.h>

#include <errno.h>
#include <stdio.h>

#include <boost/bind.hpp>

#include <Message.h>
#include <Task.h>
#include <Server.h>
#include <ThreadPool.h>
#include <Channel.h>
#include <EventLoop.h>
#include <Socket.h>
#include <SocketsOps.h>
#include <Logger.h>

using namespace sevent;

Connection::Connection(EventLoop* loop,
                             const string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
  : loop_(loop),
    name_(nameArg),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr)
{
  channel_->setReadCallback(
      boost::bind(&Connection::handleRead, this, _1));
  channel_->setWriteCallback(
      boost::bind(&Connection::handleWrite, this));
  channel_->setCloseCallback(
      boost::bind(&Connection::handleClose, this));
  channel_->setErrorCallback(
      boost::bind(&Connection::handleError, this));
  LOG_INFO("Connection::ctor[" <<  name_ << "] at " << this
            << " fd " << sockfd << " localAddr " << localAddr_.toIpPort() << " peerAddr " << peerAddr_.toIpPort());
  socket_->setKeepAlive(true);
}

Connection::~Connection()
{
  LOG_INFO("Connection::~Connection[" <<  name_ << "] at " << this
            << " fd " << socket_->fd() << " localAddr " << localAddr_.toIpPort() << " peerAddr " << peerAddr_.toIpPort());
}

int Connection::send(const void* data, size_t len)
{
	if (state_ == kConnected)
	{
		if (loop_->isInLoopThread()){
		  sendInLoop(data, len);
		}else{
			string message(static_cast<const char*>(data), len);
			loop_->runInLoop(
		      boost::bind(&Connection::sendInLoop,
		                  this,     // FIXME
		                  message));
		}
	}else{
		LOG_ERROR("Connection" << name_ << " status " << state_ << " error while send");
		return -1;
	}
	return 0;
}

// FIXME efficiency!!!
int Connection::send(Buffer* buf)
{
  if (state_ == kConnected)
  {
    if (loop_->isInLoopThread())
    {
      sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();
    }
    else
    {
      loop_->runInLoop(
          boost::bind(&Connection::sendInLoop,
                      this,     // FIXME
                      buf->retrieveAllAsString()));
                    //std::forward<string>(message)));
    }
  }else{
		LOG_ERROR("Connection" << name_ << " status " << state_ << " error while send");
		return -1;
	}
	return 0;
}

void Connection::sendInLoop(const void* data, size_t len)
{
  loop_->assertInLoopThread();
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool error = false;
  if (state_ == kDisconnected)
  {
    LOG_WARN ("disconnected, give up writing");
    return;
  }
  // if no thing in output queue, try writing directly
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
  {
    nwrote = sockets::write(channel_->fd(), data, len);
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && writeCompleteCallback_)
      {
        loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
      }
    }
    else // nwrote < 0
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      { 
				//TODO DIFF what to do when error
        LOG_ERROR("Connection::sendInLoop");
        if (errno == EPIPE) // FIXME: any others?
        {
          error = true;
        }
      }
    }
  }

  assert(remaining <= len);
  if (!error && remaining > 0)
  {
    LOG_TRACE("I am going to write more data");
    size_t oldLen = outputBuffer_.readableBytes();
    if (oldLen + remaining >= highWaterMark_
        && oldLen < highWaterMark_
        && highWaterMarkCallback_)
    {
      loop_->queueInLoop(boost::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    }
    outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);
    if (!channel_->isWriting())
    {
      channel_->enableWriting();
    }
  }
}

void Connection::sendInLoop(const string& message)
{
  sendInLoop(message.data(), message.size());
}

void Connection::shutdown()
{
  // FIXME: use compare and swap
  if (state_ == kConnected)
  {
    setState(kDisconnecting);
    // FIXME: shared_from_this()?  //??
    loop_->runInLoop(boost::bind(&Connection::shutdownInLoop, this));
  }
}

void Connection::shutdownInLoop()
{
  loop_->assertInLoopThread();
  if (!channel_->isWriting())  //?? where is else ?
  {
    // we are not writing
    socket_->shutdownWrite();
  }
}

void Connection::setTcpNoDelay(bool on)
{
  socket_->setTcpNoDelay(on);
}

void Connection::connectEstablished()
{
	LOG_DEBUG("Connection::connectEstablished" << name());
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  channel_->tie(shared_from_this());
  channel_->enableReading();
	if(connectionCallback_)
	  connectionCallback_(shared_from_this());
}

void Connection::connectDestroyed()
{
  loop_->assertInLoopThread();
  if (state_ == kConnected)
  {
    setState(kDisconnected);
    channel_->disableAll();

    //connectionCallback_(shared_from_this()); //user don't care about this
  }
  channel_->remove();
}

void Connection::handleRead(Timestamp receiveTime)
{
	LOG_DEBUG("Connection::handleRead");
	loop_->assertInLoopThread();
	int savedErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno); 
	if (n > 0)
	{
		int msgNum = inputBuffer_.getMessageNum();
		LOG_DEBUG("getMessageNum " << msgNum << " Buffer readableBytes " << inputBuffer_.readableBytes());
		//如果buffer里有多于1条Message，要都处理掉
		for(int i = 0; i < msgNum; ++i){
			MessagePtr msg(new Message(inputBuffer_.peek()));
			inputBuffer_.retrieve(msg->getTotalLen());
			LOG_DEBUG("Read Message " << msg->toString() << " total len " << msg->getTotalLen());
			TaskPtr t(new Task(shared_from_this(), msg->shared_from_this(), messageCallback_));
			getLoop()->getServer()->getThreadPool()->addTask(t);
		}
	}
	else if (n == 0)
	{
		handleClose();
	}
	else
	{
		errno = savedErrno;
		LOG_ERROR("Connection::handleRead");
		handleError();
	}
}

void Connection::handleWrite()
{
  loop_->assertInLoopThread();
  if (channel_->isWriting())
  {
    ssize_t n = sockets::write(channel_->fd(),
                               outputBuffer_.peek(),
                               outputBuffer_.readableBytes());
    if (n > 0)
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0)
      {
        channel_->disableWriting();
        //if (writeCompleteCallback_)
        //{
        //  loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
        //}
        if (state_ == kDisconnecting)
        {
          shutdownInLoop();
        }
      }
      else
      {
        LOG_TRACE ("I am going to write more data");
      }
    }
    else
    {
      LOG_ERROR("Connection::handleWrite");
      // if (state_ == kDisconnecting)
      // {
      //   shutdownInLoop();
      // }
    }
  }
  else
  {
    LOG_TRACE ("Connection fd = " << channel_->fd()
              << " is down, no more writing");
  }
}

void Connection::handleClose()
{
	LOG_DEBUG("Connection " << name_ << " fd = " << channel_->fd() << " handleClose");
  loop_->assertInLoopThread();
  assert(state_ == kConnected || state_ == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);
  channel_->disableAll();

  ConnectionPtr guardThis(shared_from_this());
	if(connectionCallback_)
		connectionCallback_(guardThis);
  // must be the last line
  closeCallback_(guardThis);
}

void Connection::handleError()
{
  int err = sockets::getSocketError(channel_->fd());
  LOG_ERROR ("Connection::handleError [" << name_
            << "] - SO_ERROR = " << err << " " << strerror(err));
}

