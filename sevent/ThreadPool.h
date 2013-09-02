#ifndef SEVENT_THREADPOOL_H
#define SEVENT_THREADPOOL_H

#include <queue>
#include <vector>
#include <string>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <Condition.h>
#include <Mutex.h>
#include <Thread.h>
#include <Callbacks.h>

namespace sevent{

class ThreadPool : boost::noncopyable{
public:

	explicit ThreadPool(const std::string &name);
	~ThreadPool();

	bool isRunning() const {
		return running_;
	} 

	void start(int number);
	void stop();

	void setName(std::string name){name_ = name;}
	std::string &name(){return name_;}

	int addTask(const TaskPtr &t);

	std::string name_;
private:
	int threadRun();

	TaskPtr getNextTask();

	MutexLock mutex_;
	Condition notEmpty_;
	Condition notFull_;

	bool running_;

	std::deque<TaskPtr> tasks_;

	boost::ptr_vector<Thread> threads_;

};


}

#endif

