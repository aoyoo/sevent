#include <Server.h>

#include <boost/bind.hpp>
#include <stdio.h>

#include <Acceptor.h>
#include <EventLoop.h>
#include <EventLoopThread.h>
#include <EventLoopThreadPool.h>
#include <ThreadPool.h>
#include <SocketsOps.h>
#include <Logger.h>

using namespace sevent;

Server::Server(const std::string &name, const InetAddress &addr)
	:name_(name), 
	started_(false),
	nextConnId_(0),
	threadNum_(0),
	listenLoopThread_(new EventLoopThread),
	acceptor_(new Acceptor(listenLoopThread_->getEventLoop(), addr)),
	ioLoopThreadPool_(new EventLoopThreadPool)
{
	acceptor_->setNewConnectionCallback(boost::bind(&Server::newConnection, this, _1, _2));
	listenLoopThread_->setInitFunc(boost::bind(&Acceptor::listen, acceptor_.get()));
	listenLoopThread_->getEventLoop()->name_ = "listenLoopThread";
	ioLoopThreadPool_->name_ = "ioLoopThreadPool";
}
	
Server::~Server(){
	LOG_INFO("Server destructor");
	listenLoopThread_->getEventLoop()->assertInLoopThread();
	LOG_TRACE("TcpServer::~TcpServer [" << name_ << "] destructing");
	
	for (ConnectionMap::iterator it(connections_.begin()); it != connections_.end(); ++it)
	{
		ConnectionPtr conn = it->second;
		it->second.reset();
		conn->getLoop()->runInLoop(boost::bind(&Connection::connectDestroyed, conn));
		conn.reset();
	}
}

void Server::start(){
	LOG_INFO("Server " << name() << " starting...");
	
	started_ = true;
	
	ioLoopThreadPool_->setThreadNum(threadNum_);
	ioLoopThreadPool_->start();
	listenLoopThread_->start();
}

void Server::stop(){ //TODO need to ensure
	started_ = false;
	listenLoopThread_->stop();
	ioLoopThreadPool_->stop();
}

void Server::newConnection(int sockfd, const InetAddress &peerAddr){
	
	EventLoop *ioLoop = ioLoopThreadPool_->getNextLoop();
	stringstream nameSS;
	nameSS << '#' << nextConnId_;
	++nextConnId_;
	string name(nameSS.str());
	LOG_DEBUG("Server::newConnection get Loop " << ioLoop << " new Connection name " << name << " fd " << sockfd);
	InetAddress localAddr(sockets::getLocalAddr(sockfd)); //empty //FIXME
	ConnectionPtr conn(new Connection(ioLoop, name, sockfd, localAddr, peerAddr));
	
	assert(connections_.find(name) == connections_.end());
	
	conn->setMessageCallback(messageThreadFunc_);
	conn->setCloseCallback(boost::bind(&Server::removeConnection, this, _1));
	connections_[name] = conn;
	
	ioLoop->runInLoop(boost::bind(&Connection::connectEstablished, conn)); //put this Connection into Loop
}

void Server::removeConnection(const ConnectionPtr& conn){
// FIXME: unsafe
	listenLoopThread_->getEventLoop()->runInLoop(boost::bind(&Server::removeConnectionInLoop, this, conn));
}

void Server::removeConnectionInLoop(const ConnectionPtr& conn){
	//LOG_INFO("Server::removeConnectionInLoop [" << name_ << "] - connection " << conn->name());
	LOG_DEBUG("Server::removeConnectionInLoop");
	size_t n = connections_.erase(conn->name());
	(void)n;
	assert(n == 1);
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->queueInLoop(boost::bind(&Connection::connectDestroyed, conn));
}


