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
	CalcServer(const std::string name, const InetAddress &addr)
		:server_(name, addr), count(0)
	{
	}
	
	int taskFunc(const ConnectionPtr c, const MessagePtr msg, Timestamp time){
		LOG_DEBUG("CalcServer taskFunc start!");
	
		__sync_fetch_and_add(&count, 1);
		//c->send(&count, 4);
	
		LOG_DEBUG("CalcServer taskFunc over!");
		return 0;
	}
	
	void start(int num){server_.setThreadNum(num);server_.start();}
	void setMessageCallback(const MessageCallback& cb){server_.setMessageCallback(cb);}

private:
	Server server_;

	long long count;
};


