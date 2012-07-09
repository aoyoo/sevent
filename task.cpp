#include "task.h"
#include "thread.h"
#include "socket_event.h"
#include "event_manager.h"
#include "message.h"
#include "Log.h"

int task::start(thread_pool *tp)
{
	//LOG_DEBUG_VA("task add task of thread pool %x", tp);
	tp->add_task(this);
	return 0;
}

int task::execute(){
	socket_event *se = NULL;
	int i = 0;
	int fd = get_socket_event()->get_socket_fd();
	se = get_socket_event()->get_event_manager()->find_event_by_fd(fd);
	if(se == NULL){
		LOG_ERROR_VA("get socket_event failed, maybe client close");
		delete (get_message()->get_socket_event()); //delete the se;
		return -1;
	}else{
		LOG_DEBUG_VA("get socket_event %x",se);
	}
	i = task_msg->response();
	if(i != 0){
		LOG_ERROR_VA("task execute error");
		delete (get_message()->get_socket_event()); //delete the se;
		return -1;
	}

}
