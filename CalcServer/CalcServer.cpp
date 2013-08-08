#include "CalcServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

int main(){
	signal(SIGPIPE,SIG_IGN);
	int ret = VSLogger::Init("log.conf", "VS40");
	LOG_INFO("test server starting...");
	
	InetAddress addr(3330);
	CalcServer *svr = new CalcServer("CalcServer", addr);
	svr->setMessageCallback(boost::bind(&CalcServer::taskFunc, svr, _1, _2, _3));
	svr->start(4);
	while(1)
		sleep(5);
	return 0;
}


