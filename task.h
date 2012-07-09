#pragma once
#include <queue>
#include <list>

class thread_pool;
class message;
class socket_event;
class task
{
	socket_event *task_se;
	message *task_msg;
public:
	task(): task_msg(NULL){}
	task(message *msg): task_msg(msg){}

	void set_socket_event(socket_event *se){task_se = se;}
	void set_message(message *msg){task_msg = msg;}

	socket_event *get_socket_event(){return task_se;}
	message *get_message(){return task_msg;}

	int start(thread_pool *tp);
	int execute();
};


