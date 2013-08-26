#include "DispatchServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

DispatchServer::DispatchServer(const std::string name, const InetAddress &serverAddr, const InetAddress &clientAddr)
	:server_(name, serverAddr), clientAddr_(clientAddr), count_(0)
{
	server_.setConnectionCallback(boost::bind(&DispatchServer::onServerConnection, this, _1));
	server_.setMessageCallback(boost::bind(&DispatchServer::onServerMessage, this, _1, _2, _3));
}

DispatchServer::~DispatchServer(){
	LOG_INFO("DispatchServer destroy");
}

void DispatchServer::onServerConnection(const ConnectionPtr &conn){
	LOG_INFO("Server " << conn->name() << (conn->connected()? " UP" : " DOWN"));
	if (conn->connected()) {
		EventLoop *loop = server_.getNextLoop();
		string name = conn->name();
		
		TunnelPtr tunnel(new Tunnel(loop, clientAddr_, conn));
		{
			MutexLockGuard lock(mutex_);
			assert(tunnels_.find(name) == tunnels_.end());
			LOG_INFO("Server add Connection " << name);
			//FIXME
			//到这时，Client的Connection还没有连上，
			//那么这时候ServerConnection发过来的消息会被缓存，如果Client一直连不上，怎么处理呢？
			tunnels_[name] = tunnel;
		}
	}else{
		string name = conn->name();
		{
			MutexLockGuard lock(mutex_);
			assert(tunnels_.find(name) != tunnels_.end());
			LOG_INFO("Server erase Connection " << name);
			tunnels_.erase(name);
		}
	}
}

//void DispatchServer::onClientConnection(const ConnectionPtr &conn){
//	LOG_INFO("Client " << conn->name() << (conn->connected()? " UP" : " DOWN"));
//	if (conn->connected()) {
//		string name = conn->name();
//		{
//			MutexLockGuard lock(mutex_);
//			assert(clientConnections_.find(name) == clientConnections_.end());
//			LOG_INFO("Client add Connection " << name);
//			clientConnections_[name] = conn;
//		}
//	}else{
//	//这里有问题，前面的连接正常，但是Client的接连断掉了，怎么办
//		string name = conn->name();
//		{
//			MutexLockGuard lock(mutex_);
//			assert(clientConnections_.find(name) != clientConnections_.end());
//			LOG_INFO("Client erase Connection " << name);
//			clientConnections_.erase(name);
//		}
//	}
//}

int DispatchServer::onServerMessage(const ConnectionPtr &conn, const MessagePtr &msg, Timestamp time){
	long long count = __sync_add_and_fetch(&count_, 1);
	if(count % 1000 == 0)
		LOG_INFO("DispatchServer onServerMessage count " << count);
	return 0;
}

//int DispatchServer::onClientMessage(const ConnectionPtr &conn, const MessagePtr &msg, Timestamp time){
//	long long count = __sync_add_and_fetch(&count_, 1);
//	if(count % 1000 == 0)
//		LOG_INFO("DispatchServer onClientMessage count " << count);
//	return 0;
//}
//


