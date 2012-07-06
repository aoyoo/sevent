#pragma once
#include <map>
#include <string>
#include "public.h"
#include "thread.h"
#include "buffer.h"
#include "message.h"
#include "socket.h"
#include "socket_event.h"
#include "event_manager.h"

using namespace std;

class server{
	string name;

	socket_event *listener_socket;
	//socket_event *message_read_socket;

	event_manager *server_event_manager;
	thread_pool *server_thread_pool;

	head server_head;
	buffer server_buf;
	//the head & buf used to receive data

	int server_type;
	int count;
public:
	server(){
		listener_socket = NULL;
		server_event_manager = new event_manager;
		server_thread_pool = new thread_pool;
	}

	void set_listener_socket(socket_event *se){listener_socket = se;}
	
/*
	void set_message_read_socket(socket_event se){
		message_read_socket = se;
	}
*/
	socket_event *get_listener_socket(){return listener_socket;}
	//socket_event get_message_read_socket(){return message_read_socket;}
	event_manager *get_event_manager(){return server_event_manager;}
	thread_pool *get_thread_pool(){return server_thread_pool;}

	head &get_head(){return server_head;}
	buffer &get_buffer(){return server_buf;}


	int start();
};

class message_server : public server, private pthread
{
	map<int, message_builder_base *> msg_builder_map;
	
	address listener_addr;

public:
	void set_listener_address(address addr){listener_addr = addr;}
	void register_message_builder(int command, message_builder_base *mb){
		msg_builder_map.insert(map<int, message_builder_base *>::value_type(command, mb));
	}

	message_builder_base *find_message_builder(int command){
		map<int, message_builder_base *>::iterator it = msg_builder_map.find(command);
		if (it == msg_builder_map.end())
			return NULL;
		return it->second;
	}
/*
	int startup(){
		LOG_INFO_VA("message server start");
		int svr_sock = socket(PF_INET, SOCK_STREAM, 0);
		if (svr_sock < 0) {
			LOG_ERROR_VA("create server socket failed");
			return -1;
		}
		int flag = 1;
		setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
		
		if (bind(svr_sock, &listener_addr, listener_addr.length())  == -1) {
			LOG_ERROR_VA("bind address failed");
			return -1;
		}
		
		int sockflag = fcntl(svr_sock, F_GETFL, 0);
		fcntl(svr_sock, F_SETFL, sockflag | O_NONBLOCK);
		
		if (listen(svr_sock, 100) < 0) {
			LOG_ERROR_VA("listen failed");
			return -1;
		}

		socket_event *listener = new socket_event(socket_event::read);
		set_listener_socket(listener);

		listener->set_socket_fd(svr_sock);

		listener_event_handler *eh = new listener_event_handler;

		eh->set_server(this);
		eh->set_socket_event(listener);

		listener->set_event_handler(eh);
		listener->set_event_manager(get_event_manager());
			
		//server_event_manager->register_event(*listener);
		get_event_manager()->register_event(*listener);
		get_event_manager()->set_server(this);
			
		server::start();
	}
*/
	int pthread_func(){
		LOG_INFO_VA("message server start");
		int svr_sock = socket(PF_INET, SOCK_STREAM, 0);
		if (svr_sock < 0) {
			LOG_ERROR_VA("create server socket failed");
			return -1;
		}
		int flag = 1;
		setsockopt(svr_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
		
		if (bind(svr_sock, &listener_addr, listener_addr.length())  == -1) {
			LOG_ERROR_VA("bind address failed");
			return -1;
		}
		
		int sockflag = fcntl(svr_sock, F_GETFL, 0);
		fcntl(svr_sock, F_SETFL, sockflag | O_NONBLOCK);
		
		if (listen(svr_sock, 100) < 0) {
			LOG_ERROR_VA("listen failed");
			return -1;
		}

		socket_event *listener = new socket_event(socket_event::read);
		set_listener_socket(listener);

		listener->set_socket_fd(svr_sock);

		listener_event_handler *eh = new listener_event_handler;

		eh->set_server(this);
		eh->set_socket_event(listener);

		listener->set_event_handler(eh);
		listener->set_event_manager(get_event_manager());
			
		//server_event_manager->register_event(*listener);
		get_event_manager()->register_listen_event(listener);
		get_event_manager()->set_server(this);
			
		server::start();
	}

	int startup(){
		return pthread::start();
	}

	int waitend(){
		return pthread::wait();
	}

};


