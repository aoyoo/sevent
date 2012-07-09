#pragma once
#include <pthread.h>
#include <queue>
#include <list>
#include "mutex.h"
#include "event.h"
#include "task.h"
#include "socket_event.h"

class pthread
{
	static void *func(void *obj)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		pthread *o = static_cast<pthread*>(obj);
		o->pthread_func();
	}

	pthread_t pid;

public:
	pthread(): pid(-1){}
	virtual ~pthread(){}

	int start(){
		return pthread_create(&pid, NULL, func, this);
		//the arg "this" !!!
	}
	
	int cancel(){
		if(pid != -1){
			pthread_cancel(pid);
			//is it right??
			pid = -1;
		}
	}

	int wait() 
	{
		if(pid != -1)
		  pthread_join(pid, 0);
		pid = -1;
	}

	virtual int pthread_func(){}
};

class thread_pool
{
	std::queue<task *> task_queue;

	//std::list<pthread *> thread_list;
	std::vector<pthread *> thread_vec;

	int max_num_threads;
	mutex task_mutex;
	event task_event;
	bool thread_pool_stop;
	
	class pool_work_thread : public pthread
	{
		thread_pool *wtp;
	public:
		pool_work_thread(): wtp(NULL){}
		pool_work_thread(thread_pool *tp): wtp(tp){}
		~pool_work_thread(){};
		//void stop();
		int pthread_func();
	};

public:
	thread_pool(): max_num_threads(1), thread_pool_stop(false) {}; 
	~thread_pool(){};//need to add  
	void set_max_threads(int i) { max_num_threads = i; }
	
	task *get_task();
	int add_task(task *thd);

	int start();
	int stop();
};

