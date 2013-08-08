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
using namespace std;

class CalcServer{
public:
	EchoServer(const std::string name, const InetAddress &addr)
		:server_(name, addr), count(0)
	{
	}
	int taskFunc(const ConnectionPtr c, const MessagePtr msg, Timestamp time){
		LOG_DEBUG("test server working!!!");
		__sync_fetch_and_add(&count, 1);
		c->send(&count, 4);
		return 0;
	}
	
	void start(int num){server_.setThreadNum(num);server_.start();}
	
	Server server_;

	long long count;
};

int main(){
	signal(SIGPIPE,SIG_IGN);
	int ret = VSLogger::Init("log.conf", "VS40");
	LOG_INFO("test server starting...");
	
	InetAddress addr(3330);
	EchoServer *svr = new EchoServer("EchoServer", addr);
	svr->server_.setMessageCallback(boost::bind(&EchoServer::taskFunc, svr, _1, _2, _3));
	svr->start(4);
	while(1)
		sleep(5);
	return 0;
}


