#include <sys/epoll.h>
#include <poll.h>
#include <iostream>
#include <sstream>

#include <Channel.h>
#include <EventLoop.h>
#include <Logger.h>

using namespace sevent;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
	:loop_(loop),
	fd_(fd),
	events_(0),
	revents_(0),
	index_(-1),
	tied_(false),
	eventHandling_(false)
{
	LOG_DEBUG("Channel::constructor fd " << fd_ << " POLLIN " << POLLIN << " POLLOUT " << POLLOUT);
}
	
Channel::~Channel(){
	assert(!eventHandling_); //??
}

void Channel::tie(const boost::shared_ptr<void>& obj) //important
{
  tie_ = obj;
  tied_ = true;
}

void Channel::update(){
	loop_->updateChannel(this);
}

void Channel::remove(){
	assert(isNoneEvent()); //??
	loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
	LOG_DEBUG("Channel::handleEvent fd: " << fd_ << " events " << reventsToString());
  boost::shared_ptr<void> guard;
  if (tied_)
  {
		LOG_DEBUG("Channel::handleEvent tied_ " << fd_);
    guard = tie_.lock();
    if (guard)
    {
			LOG_DEBUG("Channel::handleEvent tied guard " << fd_);
			handleEventWithGuard(receiveTime);
    }
  }
  else //到这里的话，说明此Channel不属于某个Connection？
  {
		LOG_DEBUG("Channel::handleEvent not tied_ " << fd_);
    handleEventWithGuard(receiveTime);
  }
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{ 
	LOG_DEBUG("Channel::handleEventWithGuard fd: " << fd_);
	eventHandling_ = true;
	if(revents_ & (POLLERR | POLLNVAL)){
		if (errorCallback_) errorCallback_();
	}
	if((revents_ & POLLHUP) && !(revents_ & POLLIN)){
		LOG_WARN("Channel::handle_event() POLLHUP");
		if (closeCallback_) closeCallback_();
	}
	if (revents_ & POLLNVAL){
		LOG_WARN("Channel::handle_event() POLLNVAL");
	}
	if (revents_ & POLLRDHUP){
		LOG_WARN("Channel::handle_event() POLLRDHUP");
	}

	if (revents_ & POLLIN){
		LOG_DEBUG("Channel::handleEventWithGuard POLLIN fd: " << fd_);
		if (readCallback_) readCallback_(receiveTime);
	}
	if (revents_ & POLLOUT){
		LOG_DEBUG("Channel::handleEventWithGuard POLLOUT fd: " << fd_);
		if (writeCallback_) writeCallback_();
	}
  eventHandling_ = false;
}

string Channel::reventsToString() const
{
  std::ostringstream oss;
  oss << fd_ << ": ";
  if (revents_ & POLLIN)
    oss << "IN ";
  if (revents_ & POLLPRI)
    oss << "PRI ";
  if (revents_ & POLLOUT)
    oss << "OUT ";
  if (revents_ & POLLHUP)
    oss << "HUP ";
  if (revents_ & POLLRDHUP)
    oss << "RDHUP ";
  if (revents_ & POLLERR)
    oss << "ERR ";
  if (revents_ & POLLNVAL)
    oss << "NVAL ";

  return oss.str().c_str();
}







