#include <signal.h>
#include "server.h"
#include "front_server.h"

using namespace std;

/*
int start_servers(server *srv){
	cout << "start_server!" << endl;
	srv->start();
}
*/

int main(int argc, char **argv)
{
	string path = "./logs";
	string log_level = "DEBUG";
	if (!init_logger(path, log_level)) {
		cout << "logger init failed log[@path | @level]" << endl;
		return -1;
	}
	signal(SIGPIPE,SIG_IGN);
	front_server *srv = new front_server;
	
	address addr("10.1.0.11", 41234);
	srv->set_listener_address(addr);
	LOG_INFO_VA("main server start");
	srv->setup();
	srv->startup();
	srv->waitend();
	//start_servers(srv);
	return 0;
}

