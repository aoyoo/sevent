#ifndef SEVENT_THREADPOOL_H
#define SEVENT_THREADPOOL_H

#include "Thread.h"
#include "Callbacks.h"

#include <queue>
#include <vector>
#include <string>

namespace sevent{

class ThreadPool {
public:
	explicit ThreadPool();
	explicit ThreadPool(const std::string name);
	~ThreadPool();

	bool started() const {
		return _started;
	} 
	void start(int number);
	int wait();
	int stop();
	void clean();
	int addTask(const TaskPtr &t);

	void setName(std::string name){_name = name;}
	std::string &name(){return _name;}
private:
	int threadRun();

	TaskPtr getNextTask();

	pthread_cond_t _cond;
	pthread_mutex_t _mutex;

	std::string _name;
	bool _started;

	std::queue <TaskPtr> _tasks;
	int _taskSize;
	std::vector <Thread *>_threads;
	int _threadsNum;

	ThreadPool & operator =(const ThreadPool & other);
	ThreadPool(const ThreadPool & other);
};


}

#endif

