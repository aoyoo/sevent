#include <ThreadPool.h>

#include <time.h>
#include <sys/time.h>
#include <algorithm>

#include <boost/bind.hpp>

#include <Task.h>
#include <Logger.h>

using namespace sevent;

ThreadPool::ThreadPool(const std::string &name)
	:name_(name), 
	mutex_(),
	notEmpty_(mutex_),
	notFull_(mutex_),
	running_(false)
{
	LOG_INFO("ThreadPool constructor");
}

ThreadPool::~ThreadPool(){
	LOG_INFO("ThreadPool::~ThreadPool");
	if(running_)
		stop();
}

void ThreadPool::start(int number){
	assert(threads_.empty());
	running_ = true;
	LOG_INFO("ThreadPool " << name() << " started with number " << number);
	for (int i = 0; i < number; ++i) {
		Thread *t = new Thread(boost::bind(&ThreadPool::threadRun, this));
		threads_.push_back(new Thread(boost::bind(&ThreadPool::threadRun, this)));
		threads_[i].start();
	}
}

void ThreadPool::stop()
{
	{
	MutexLockGuard lock(mutex_);
	running_ = false;
	notEmpty_.notifyAll();
	}
	//for_each(threads_.begin(), threads_.end(), boost::bind(&sevent::Thread::join, _1); //FIXME
	//for(std::vector<Thread *>::iterator iter = _threads.begin(); iter != _threads.end(); ++iter) {
	//	(*iter)->join();
	//}
}

int ThreadPool::addTask(const TaskPtr &t){
	int ret = 0;
	{
	MutexLockGuard lock(mutex_);
	tasks_.push_back(t);
	notEmpty_.notify();
	}
	return 0;
}

TaskPtr ThreadPool::getNextTask(){
	MutexLockGuard lock(mutex_);
	while(tasks_.empty() && running_) {
		notEmpty_.wait();
	}
	TaskPtr t;
	if(!tasks_.empty()) {
		t = tasks_.front();
		tasks_.pop_front();
	}
	return t;
}

int ThreadPool::threadRun(){
	LOG_INFO("ThreadPool " << name() << " workThread start");
	
	while (running_) {
		TaskPtr t = getNextTask();
		int ret = 0;
		if(t)
			ret = t->execute();
		if (ret < 0) {
			LOG_ERROR("ThreadPool " << name() << "task execution error");
		}
	}

	LOG_INFO("ThreadPool stoped");
	return 0;
}



