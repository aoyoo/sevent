#pragma once
#include <queue>
#include <map>
#include "public.h"
#include "socket_event.h"
#include "mutex.h"
using std::queue;
using std::map;

class server;  //define in server.h

class event_manager{
	
	int listener_socket;
	message_server *manager_server;

	//queue<socket_event *> ready_events;
	//map<int, socket_event *> all_events;

	mutex ready_mutex;
	mutex all_mutex;

	int epfd;
	bool loop_stop;
public:
	event_manager();
	
	void set_listener_socket(int i){listener_socket = i;}
	int  get_listener_socket(){return listener_socket;}
	
	void set_server(message_server *svr) {manager_server = svr;}
	message_server *get_server() {return manager_server; }

	int setnonblocking(int sockfd);
	void start_event_loop();
};

