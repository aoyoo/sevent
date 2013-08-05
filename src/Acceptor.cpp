#include <boost/bind.hpp>

#include <errno.h>
#include <fcntl.h>

#include <Acceptor.h>
#include <EventLoop.h>
#include <InetAddress.h>
#include <SocketsOps.h>

#include <Logger.h>

using namespace sevent;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	:loop_(loop),
	acceptSocket_(sockets::createNonblockingOrDie()),
	acceptChannel_(loop, acceptSocket_.fd()),
	listening_(false)
{
	acceptSocket_.setReuseAddr(true); //TODO
	//acceptSocket_.setReusePort(true); //TODO seems not support 
	acceptSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(boost::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor(){
	LOG_INFO("Acceptor destructor");
	acceptChannel_.disableAll();
	acceptChannel_.remove();
}

void Acceptor::listen(){
	LOG_DEBUG("Acceptor listen");
	listening_ = true;
	acceptSocket_.listen();
	LOG_DEBUG("Acceptor enableReading");
	acceptChannel_.enableReading();
}

void Acceptor::handleRead(){
	LOG_DEBUG("Acceptor handleRead");
	loop_->assertInLoopThread();
	assert(newConnectionCallback_); //DIFF must has a newConnectionCallback_ as normal
	InetAddress peerAddr(0);  //FIXME
	int sockfd = acceptSocket_.accept(&peerAddr);
	if(sockfd >= 0){
		newConnectionCallback_(sockfd, peerAddr);
	}else{ //DIFF if not sockfd, what to do? just exit?
		LOG_ERROR("accept error " << strerror(errno));
	}
}




