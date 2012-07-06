#include "thread.h"
#include "Log.h"

int task::start(thread_pool *tp)
{
	//LOG_DEBUG_VA("task add task of thread pool %x", tp);
	tp->add_task(this);
	return 0;
}

int thread_pool::start()
{
	LOG_INFO_VA("thread_pool start");
	//create all threads
	for(int i = 0; i < max_num_threads; i++)
	{
		pthread *p = new pool_work_thread(this);
		thread_vec.push_back(p);
	}

	//start all the threads
	for (vector<pthread *>::iterator it = thread_vec.begin(); it != thread_vec.end(); it++) 
	{
		(*it)->start();
	}
}

int thread_pool::add_task(task *tsk)
{
	LOG_DEBUG_VA("thread_pool %x add task %x",this, tsk);
	//need mutex?
	//i think there just one thread can add_task!!
	//is the listener thread
	task_mutex.lock();
	task_queue.push(tsk);
	LOG_DEBUG_VA("%d task in queue",task_queue.size());
	task_mutex.unlock();

	//send a signal
	task_event.signal();
}

task *thread_pool::get_task()
{
	task *t = NULL;
	task_mutex.lock();
	if (!task_queue.empty()) {
		t = task_queue.front();
		task_queue.pop();
	}
	task_mutex.unlock();
	return t;
}

int thread_pool::stop()
{
	//the diffence beteew this and ~thread_pool()
}

int thread_pool::pool_work_thread::pthread_func()
{
	if(wtp == NULL){
		LOG_ERROR_VA("pool_work_thread don't belong to a thread_pool");
		return -1;
	}
	while(!(wtp->thread_pool_stop))
	{
		task *tsk = wtp->get_task();
		if(!tsk)
		{
			LOG_DEBUG_VA("get no task, wait...");
			//wait()
			//wait for the signal
			wtp->task_event.wait();
		}
		else
		{
			LOG_DEBUG_VA("get task %x, going to execute it", tsk);
			tsk->execute();
			delete tsk;
		}
	}
}

