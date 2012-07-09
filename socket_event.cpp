#include "socket_event.h"
#include "socket.h"
#include "server.h"
#include "message.h"
#include "event_manager.h"
#include "Log.h"

int tcp_read_timeout = 2000;

void socket_event::set_event_type(event_type_e type)
{
	event_type = type;
	//the follew event_handler has tp init the new event_handler with sock_event
/*	
	switch(event_type)
	{
	case newin:
	{
		LOG_ERROR_VA("error a newin");
		//set_event_handler(new listener_event_handler);
		break;
	}
	case read:
	{
		LOG_DEBUG_VA("a new message_read_event_handler");
		//set_event_handler(new message_read_event_handler);
		break;
	}
	default:
	  break;
	}
*/	
}

//this means a new socket in
int listener_event_handler::read(socket_event *se)
{
	LOG_DEBUG_VA("listener_event_handler read");
	address addr;
	socklen_t length = addr.length();
	
	//if epoll_wait get the fd == listener,it's also trigger a EPOLLIN!!!
	int new_sock = accept(se->get_socket_fd(), &addr, &length);
	if (new_sock < 0) {
		if ((errno == EAGAIN || errno == EWOULDBLOCK)) {
			return 0;
		//what is it!!!
		} else {
			return -1;
		}
	}
	
	socket_event *new_se = new socket_event;
	
	new_se->set_event_type(socket_event::read);
	new_se->set_socket_fd(new_sock);
	
	message_read_event_handler *eh = new message_read_event_handler;
	
	eh->set_socket_event(new_se);
	eh->set_server(get_server());
	
	new_se->set_event_handler(eh);
	
	new_se->set_event_manager(get_server()->get_event_manager());
	
	get_server()->get_event_manager()->register_event(new_se);
	
	LOG_DEBUG_VA("regeister fd: %d ,socket_event %x",new_se->get_socket_fd(), new_se);
	//if this error,remenber release new_se/new message_read_event_handler
	
	return 0;
}

int message_read_event_handler::read(socket_event *se)
{
	LOG_DEBUG_VA("message_read_event_handler read");
	//server *svr = get_socket_event()->get_event_manager()->get_server();
	
	//head msg_head = get_server()->get_head();
	//buffer msg_buf = get_server()->get_buffer();
	head msg_head;
	buffer msg_buf;
	char read_buf[128];
	int sock = se->get_socket_fd();
	//int ret = tcp_read_ms(sock, read_buf, sizeof(msg_head), tcp_read_timeout);
	int ret = tcp_read_ms(sock, &msg_head, sizeof(msg_head), tcp_read_timeout);
	
	if (ret <= 0 || ret != sizeof(msg_head)){
		LOG_ERROR_VA("message read head error, %d",ret); 
		return -1;
	}
	//msg_head.show();
	msg_head.ntoh();
	msg_head.show();
	
	ret = tcp_read_ms(sock, msg_buf, msg_head.length, tcp_read_timeout);
		
	//LOG_DEBUG_VA("message body received: %d",ret);
	if (ret != msg_head.length) {
		LOG_ERROR_VA("message body receive failed");
		return -1;
	}
	
	message_builder_base *msg_builder = get_server()->find_message_builder(msg_head.command);
	if(msg_builder == NULL){
		LOG_ERROR_VA("find message builder failed");
		return -1;
	}
	
	message *msg = msg_builder->build_message(msg_head, msg_buf);
	
	msg->set_socket_event(se);
//	task *tsk = new message_task(msg);
	task *tsk = new task(msg);
	tsk->set_socket_event(se);
	LOG_DEBUG_VA("new task by fd: %d; socket_event: %x; msg: %x; task: %x", se->get_socket_fd(), se, msg, tsk);
	tsk->start(get_server()->get_thread_pool());
	return 0;
}

