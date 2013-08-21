#include "Client.h"

#include <boost/bind.hpp>
#include <stdio.h>		// snprintf

#include "Connector.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include "Logger.h"

using namespace sevent;

namespace detail {

	void removeConnection(EventLoop * loop, const ConnectionPtr & conn) {
		loop->queueInLoop(boost::bind(&Connection::connectDestroyed, conn));
	}
 
	void removeConnector(const ConnectorPtr & connector) {
		//connector->
	}

}

using namespace sevent;

Client::Client(EventLoop * loop, const InetAddress & serverAddr, const string & name)
	:loop_(loop), connector_(new Connector(loop, serverAddr)), name_(name), retry_(false), connect_(true), nextConnId_(1)
{
	connector_->setNewConnectionCallback(boost::bind(&Client::newConnection, this, _1));
	LOG_DEBUG("Client::Client[" << name_ << "]");
}

Client::~Client()
{
	LOG_DEBUG("Client::~Client[" << name_ << "]");
	ConnectionPtr conn;
	{
		MutexLockGuard lock(mutex_);
		conn = connection_;
	}
	if (conn) {
		// FIXME: not 100% safe, if we are in different thread
		// TODO: 下面这2句有点绕啊
		CloseCallback cb = boost::bind(&detail::removeConnection, loop_, _1);
		loop_->runInLoop(boost::bind(&Connection::setCloseCallback, conn, cb));
	} else {
		connector_->stop();
		// FIXME: HACK
		// 为什么要runAfter呢
		//loop_->runAfter(1, boost::bind(&detail::removeConnector, connector_));
		loop_->runInLoop(boost::bind(&detail::removeConnector, connector_));
	}
}

void Client::connect()
{
	// FIXME: check state
	LOG_INFO("Client::connect[" << name_ << "] - connecting to " << connector_->serverAddress().toIpPort());
	connect_ = true;
	connector_->start();
}

void Client::disconnect()
{
	connect_ = false;

	{
		MutexLockGuard lock(mutex_);
		if (connection_) {
			connection_->shutdown();
		}
	}
}

void Client::stop()
{
	connect_ = false;
	connector_->stop();
}

void Client::send(const void* message, size_t len){
	ConnectionPtr conn = connection();
	if(conn)
		conn->send(message, len);
}

void Client::newConnection(int sockfd)
{
	loop_->assertInLoopThread();
	InetAddress peerAddr(sockets::getPeerAddr(sockfd));
	char buf[32];
	snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toIpPort().c_str(), nextConnId_);
	++nextConnId_;
	string connName = name_ + buf;

	InetAddress localAddr(sockets::getLocalAddr(sockfd));
	// FIXME poll with zero timeout to double confirm the new connection
	// FIXME use make_shared if necessary
	ConnectionPtr conn(new Connection(loop_, connName, sockfd, localAddr, peerAddr));

	conn->setMessageCallback(messageCallback_);
	conn->setCloseCallback(boost::bind(&Client::removeConnection, this, _1));	// FIXME: unsafe
	{
		MutexLockGuard lock(mutex_);
		connection_ = conn;
	}
	conn->connectEstablished();
}

void Client::removeConnection(const ConnectionPtr & conn)
{
	loop_->assertInLoopThread();
	assert(loop_ == conn->getLoop());

	{
		MutexLockGuard lock(mutex_);
		assert(connection_ == conn);
		connection_.reset();
	}

	loop_->queueInLoop(boost::bind(&Connection::connectDestroyed, conn));
	if (retry_ && connect_) {
		LOG_INFO("Client::connect[" << name_ << "] - Reconnecting to " << connector_->serverAddress().toIpPort());
		connector_->restart();
	}
}


