#include "DispatchServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

DispatchServer::DispatchServer(const std::string name, const InetAddress &addr)
	:server_(name, addr), addr_(addr), count(0)
{
	server_.setConnectionCallback(boost::bind(&DispatchServer::onConnection, this, _1));
	server_.setMessageCallback(boost::bind(&DispatchServer::onMessage, this, _1, _2, _3));
}

DispatchServer::~DispatchServer(){
	LOG_INFO("DispatchServer destroy");
}

void DispatchServer::onConnection(const ConnectionPtr &conn){
	LOG_INFO(conn->name() << (conn->connected()? " UP" : " DOWN"));
	if (conn->connected()) {
		EventLoop *loop = server_.getNextLoop();
		string name = conn->name();
		ClientPtr client(new Client(loop, addr_, name));
		{
			MutexLockGuard lock(mutex_);
			assert(connections_.find(name) == connections_.end());
			LOG_INFO("add Client " << name);
			connections_[name] = client;
		}
	}else{
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
	LOG_INFO("DispatchServer onMessage");
	__sync_fetch_and_add(&count, 1);
	
	return 0;
}



