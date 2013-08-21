#include "DispatchServer.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

int main(int argc, char *argv[]){
	signal(SIGPIPE,SIG_IGN);
	int ret = VSLogger::Init("log.conf", "DispatchServer");
	LOG_INFO("DispatchServer starting...");
	
	InetAddress addr(3330);

	DispatchServer *svr = new DispatchServer("DispatchServer", addr);

	svr->start(9);
	while(1)
		sleep(5);
	delete svr;
	return 0;
}


