#pragma once
#include <queue>
#include <map>
#include "public.h"
#include "socket_event.h"
#include "mutex.h"
using std::queue;
using std::map;

class server;  //define in server.h
class socket_event;
class event_handler;
class event_manager{
	
	socket_event *listen;
	message_server *manager_server;

	queue<socket_event *> ready_events;
	queue<socket_event *> listen_events;
	map<int, socket_event *> all_events;

	mutex ready_mutex;
	mutex listen_mutex;
	mutex all_mutex;

	int epfd;
	bool loop_stop;
public:
	event_manager();
	~event_manager(){
		if(ready_events.empty()){
			//how to delete all the ready_events when kill -2
			//queue hasn't iterator
			//for(queue<socket_event *>::iterator it = ready_events.begin(); it != ready_events.end(); it++)
			//	delete *it;
		}
	}
	
	int insert_event_by_fd(socket_event *se);
	int delete_event_by_fd(int fd);
	int delete_event_by_fd(socket_event *se);
	socket_event *find_event_by_fd(int fd);

	int register_listen_event(socket_event *se);
	socket_event *get_listen_event(){return listen;}
	void set_listen_event(socket_event *se){listen = se;}
		
	int register_event(socket_event *se);
	int unregister_event(socket_event *se);

	int wait_for_events();
	socket_event *get_ready_event();

	void set_server(message_server *svr) { manager_server = svr;}
	message_server *get_server() { return manager_server; }

	void start_event_loop();
	int dispatch(socket_event *se)
	{
		return se->get_event_handler()->handle_event(se);
		//event_handler *eh = se->get_event_handler();
		//return eh->handle_event(se);
	}
};

