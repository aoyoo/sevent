#pragma once
#include "public.h"
#include "Log.h"

class event_handler;
class event_manager;
class socket_event
{
public:
	typedef enum {
		newin, 
		read, 
		write, 
		error
	}event_type_e;
private:
	int socket_fd;
	event_type_e event_type;
	event_handler *socket_event_handler;

	event_manager *socket_event_manager;

public:
	socket_event(): socket_event_handler(NULL), socket_fd(0) {}
	socket_event(event_type_e t): socket_event_handler(NULL), event_type(t), socket_fd(0) {}
	~socket_event(){
		LOG_DEBUG_VA("socket_event %x fd: %d delete",this, socket_fd);
	/*	
		if(socket_event_handler != NULL)
			delete socket_event_handler;
		else
			LOG_ERROR_VA("delete even_handler error when ~socket_event");
		if(socket_fd != 0)
			close(socket_fd);
		else
			LOG_ERROR_VA("close fd error when ~socket_event");
	*/
	}

	void set_socket_fd(int fd){socket_fd = fd;}
	void set_event_type(event_type_e type);
	void set_event_handler(event_handler *h) {socket_event_handler = h; }
	void set_event_manager(event_manager *em) {socket_event_manager = em; }

	int get_socket_fd(){return socket_fd;}
	event_type_e get_event_type(){return event_type;}
	event_handler *get_event_handler(){return socket_event_handler;}
	event_manager *get_event_manager(){return socket_event_manager;}
};

class event_handler
{
	socket_event *event_handler_socket;
public:
	event_handler(): event_handler_socket(NULL){}
	virtual ~event_handler(){}
	event_handler(socket_event *se){event_handler_socket = se;}

	void set_socket_event(socket_event *se){
		event_handler_socket = se;
	}

	socket_event *get_socket_event(){return event_handler_socket;}
	virtual int handle_event(socket_event *se) = 0;
};

class message_server; 
class message_event_handler: public event_handler
{
	message_server *msg_server;
public:
	message_event_handler(): msg_server(NULL){}
	message_event_handler(message_server *ms): msg_server(ms){}
	void set_server(message_server *ms){msg_server = ms;}
	message_server *get_server(){return msg_server;}
};

class listener_event_handler : public message_event_handler
{
public:
	int read(socket_event *e);
	int handle_event(socket_event *e){
		return read(e);
	}
};

class message_read_event_handler : public message_event_handler
{
public:
	int read(socket_event *e);
	int handle_event(socket_event *e){
		return read(e);
	}
};

