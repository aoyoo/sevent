#include "Server.h"
#include "Connection.h"
#include "Message.h"
#include "InetAddress.h"
#include "Logger.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;

class EchoServer{
public:
	EchoServer(const std::string name, const InetAddress &addr)
		:server_(name, addr)
	{
	}
	int taskFunc(const ConnectionPtr c, const MessagePtr msg, Timestamp time){
		LOG_INFO("test server working!!!");
	}
	
	void start(int workThreadNum, int ioThreadNum){server_.start();}
	
	Server server_;
};

int main(){
	signal(SIGPIPE,SIG_IGN);
	int ret = VSLogger::Init("log.conf", "VS40");
	LOG_INFO("test server starting...");
	
	InetAddress addr(3330);
	EchoServer *svr = new EchoServer("EchoServer", addr);
	svr->server_.setTaskCallback(boost::bind(&EchoServer::taskFunc, svr, _1, _2, _3));
	svr->start(1,1);
	while(1)
		sleep(5);
	return 0;
}


