#include "DispatchServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

DispatchServer::DispatchServer(const std::string name, const InetAddress &addr)
	:server_(name, addr), addr_(addr), count_(0)
{
	server_.setConnectionCallback(boost::bind(&DispatchServer::onServerConnection, this, _1));
	server_.setMessageCallback(boost::bind(&DispatchServer::onMessage, this, _1, _2, _3));
}

DispatchServer::~DispatchServer(){
	LOG_INFO("DispatchServer destroy");
}

void DispatchServer::onServerConnection(const ConnectionPtr &conn){
	LOG_INFO("Server " << conn->name() << (conn->connected()? " UP" : " DOWN"));
	if (conn->connected()) {
		EventLoop *loop = server_.getNextLoop();
		string name = conn->name();
		ClientPtr client(new Client(loop, addr_, name));
		client->setConnectionCallback(boost::bind(&DispatchServer::onClientConnection, this, _1));
		{
			MutexLockGuard lock(mutex_);
			assert(connections_.find(name) == connections_.end());
			assert(clients_.find(name) == clients_.end());
			LOG_INFO("Server add Connection " << name);
			connections_[name] = conn;
			clients_[name] = client;
		}
	}else{
		string name = conn->name();
		{
			MutexLockGuard lock(mutex_);
			assert(connections_.find(name) != connections_.end());
			assert(clients_.find(name) != clients_.end());
			LOG_INFO("Server erase Connection " << name);
			connections_.erase(name);
			clients_.erase(name);
		}
	}
}

void DispatchServer::onClientConnection(const ConnectionPtr &conn){
	LOG_INFO("Client " << conn->name() << (conn->connected()? " UP" : " DOWN"));
	if (conn->connected()) {
		string name = conn->name();
		{
			MutexLockGuard lock(mutex_);
			assert(connections_.find(name) == connections_.end());
			connections_[name] = conn;
		}
	}else{
	//这里有问题，前面的连接正常，但是Client的接连断掉了，怎么办
		string name = conn->name();
		{
			MutexLockGuard lock(mutex_);
			assert(connections_.find(name) != connections_.end());
			LOG_INFO("erase Client " << name);
			connections_.erase(name);
		}
	}
}

int DispatchServer::onMessage(const ConnectionPtr &conn, const MessagePtr &msg, Timestamp time){
	long long count = __sync_add_and_fetch(&count_, 1);
	if(count % 1000 == 0)
		LOG_INFO("DispatchServer onMessage count " << count);
	
	return 0;
}



