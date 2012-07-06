#include "server.h"

using namespace std;

int server::start()
{
	server_thread_pool->start();

	//server_event_manager->set_server(this);
	//server_event_manager->register_event(*listener_socket);
	server_event_manager->start_event_loop();
}
