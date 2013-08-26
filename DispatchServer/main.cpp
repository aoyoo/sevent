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
	
	InetAddress serverAddr(3330), clientAddr(3331);

	DispatchServer *svr = new DispatchServer("DispatchServer", serverAddr, clientAddr);

	svr->start(1);
	while(1)
		sleep(5);
	delete svr;
	return 0;
}


