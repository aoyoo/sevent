#include "event_manager.h"
#include "Log.h"
#include "server.h"
#include "socket_event.h"

int event_manager::wait_for_events()
{
	epoll_event evs[256];
	LOG_INFO_VA("epoll wait for event...");
	int n = 0;
	do {
		n = epoll_wait(epfd, evs, 256, -1);
		if(n < 0 && errno == EINTR){
			//what EINTR means
			continue;
		}
		if (n < 0){
			LOG_ERROR_VA("epoll wait < 0");
			return -1;
		}
		if (n == 0){
			LOG_ERROR_VA("epoll wait == 0");
			return 0;
		}
		break;
	} while (1);
	
	LOG_DEBUG_VA("epoll wait get %d events", n);
	for (int i = 0; i != n; i++) {
		//if(evs[i].data.fd == get_server()->get_listener_socket()->get_socket_fd()){
		if(evs[i].data.fd == listen->get_socket_fd()){
			LOG_DEBUG_VA("epoll newin: %d", listen->get_socket_fd());
			while(1){
				address addr;
				socklen_t length = addr.length();
	
				int new_sock = accept(listen->get_socket_fd(), &addr, &length);
				if (new_sock <= 0) {
					LOG_ERROR_VA("new sock fd error :%d",new_sock);
					break;
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
			}
		/*
			socket_event *se = get_listen_event();
			if(se == NULL){
				LOG_ERROR_VA("get manager listen failed");
				continue;
				//return -1;
			}
			se->set_event_type(socket_event::newin);
			ready_mutex.lock();
			ready_events.push(se);
			ready_mutex.unlock();
		*/
		}else if(evs[i].events & EPOLLIN) {
			LOG_DEBUG_VA("epoll in: %d", evs[i].data.fd);
			socket_event *se = find_event_by_fd(evs[i].data.fd);
			if(se == NULL){
				LOG_ERROR_VA("find event by fd failed");
				continue;
				//return -1;
			}
			se->set_event_type(socket_event::read);
			//ready_mutex.lock();
			ready_events.push(se);
			LOG_DEBUG_VA("ready events num: %d", ready_events.size());
			//ready_mutex.unlock();
		}else if (evs[i].events & EPOLLOUT) {
			LOG_ERROR_VA("epoll write");
		}else if (evs[i].events & EPOLLERR) {
			LOG_ERROR_VA("epoll error :%d", evs[i].data.fd);
		}else{
			//some else EPOLL 
			LOG_ERROR_VA("epoll unknown");
		}
		
	}
	return n;
}

int event_manager::insert_event_by_fd(socket_event *se)
{
	if(find_event_by_fd(se->get_socket_fd()) == NULL){

		all_mutex.lock();
		all_events.insert(std::map<int ,socket_event *>::value_type(se->get_socket_fd(),se));
		all_mutex.unlock();
		LOG_DEBUG_VA("insert fd: %d, event: %x",se->get_socket_fd(), se);
	}else{
		LOG_ERROR_VA("event fd is exist");
		//if the sock_fd is exist, what to do?
		return -1;
	}
	
}

int event_manager::delete_event_by_fd(int fd)
{
	//if one fd <---> several socket_events, how to do!!!
	all_mutex.lock();
	std::map<int ,socket_event *>::iterator it = all_events.find(fd);
	if(it == all_events.end()){
		all_mutex.unlock();
		LOG_ERROR_VA("delete event by fd error");
		return -1;
	}else{
		all_events.erase(it);
		//need to delete the se?
		all_mutex.unlock();
		return 0;
	}
}

socket_event *event_manager::find_event_by_fd(int fd)
{
	all_mutex.lock();
	LOG_DEBUG_VA("find event by fd: %d",fd);
	std::map<int ,socket_event *>::iterator it = all_events.find(fd);
	if(it == all_events.end()){
		all_mutex.unlock();
		return NULL;
	}else{
		socket_event *se = it->second;
		all_mutex.unlock();
		//unlock, then return iterator, it's no safe?
		//return it->second;
		return se;
	}
}

int event_manager::register_listen_event(socket_event *se)
{
	LOG_DEBUG_VA("event_manager listen register");
	epoll_event ee;
	int op = EPOLL_CTL_ADD;
	ee.events = EPOLLET | EPOLLIN;
	ee.data.fd = se->get_socket_fd();

	if(ee.data.fd == -1)
		return -1;
	if(epoll_ctl(epfd, op, se->get_socket_fd(), &ee) != 0)
		return -1;
	
	set_listen_event(se);
	insert_event_by_fd(se);
	return 0;
}

int event_manager::register_event(socket_event *se)
{
	epoll_event ee;
	int op = EPOLL_CTL_ADD;
	ee.events = EPOLLET;
	ee.data.fd = se->get_socket_fd();

	if(ee.data.fd == -1)
		return -1;
	if(se->get_event_type() == socket_event::read)
		ee.events |= EPOLLIN;
	if(se->get_event_type() == socket_event::write)
		ee.events |= EPOLLOUT;
	if(se->get_event_type() == socket_event::error)
		ee.events |= EPOLLERR;
	//when client close its socket, EPOLLIN and EPOLL_something will be trigger, so ,consider about this

	if(epoll_ctl(epfd, op, se->get_socket_fd(), &ee) != 0)
		return -1;

	insert_event_by_fd(se);
	return 0;
}

int event_manager::unregister_event(socket_event *se)
{
	int op = EPOLL_CTL_DEL;
	epoll_event ee;
	ee.events = 0;
	if(se->get_event_type() == socket_event::read)
		ee.events &= (~EPOLLIN);
	if(se->get_event_type() == socket_event::write)
		ee.events &= (~EPOLLOUT);
	if(se->get_event_type() == socket_event::error)
		ee.events &= (~EPOLLERR);

	if(epoll_ctl(epfd, op, se->get_socket_fd(), &ee) < 0)
		return -1;

	int ret = delete_event_by_fd(se->get_socket_fd());
	if(ret < 0){
		LOG_ERROR_VA("delete event by fd error when unregister");
		return ret;
	}else
	  return 0;
}

socket_event *event_manager::get_ready_event()
{
	int fd_num = 0;
	//ready_mutex.lock();
	while(ready_events.empty()){
		//ready_mutex.unlock();
		fd_num = wait_for_events();
		if(fd_num <= 0){
			LOG_ERROR_VA("wait_for_events error");
			return NULL;
		}
	}
	//ready_mutex.lock();
	socket_event *se = ready_events.front();
	if(!se){
		LOG_ERROR_VA("can't get ready event");
		//ready_mutex.unlock();
		return NULL;
	}
	ready_events.pop();
	//ready_mutex.unlock();
	//while xxx.pop, remeber delete the resource
	return se;
}

event_manager::event_manager()
{
	loop_stop = false;
	listen = NULL;
	epfd = epoll_create(2048);
	LOG_DEBUG_VA("event_manager epoll fd: %d",epfd);
}

void event_manager::start_event_loop()
{
	socket_event *se;
	while(!loop_stop){
		se = get_ready_event();
		if(se != NULL){
			LOG_DEBUG_VA("get ready event %x", se);
			int ret = dispatch(se);
			if(ret < 0){
				LOG_ERROR_VA("dispatch failed");
				unregister_event(se);
			}
			
			//unregister_event(se);
			//delete se;
			/*
			unregister_event(*se);
			if(delete_event_by_fd(se->get_socket_fd()) == -1 )
				LOG_ERROR_VA("delete event by fd failed");
			else
				delete se;
			*/
			//can't delete, it must be use in task!!!
			//when thread_pool handle the task
			//then pthread delete the socket_event
			//delete se;
			
		}else{
			LOG_ERROR_VA("get ready event error");
		}
	}
}

