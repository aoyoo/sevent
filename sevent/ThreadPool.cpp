#include <time.h>
#include <sys/time.h>
#include <algorithm>
#include <boost/bind.hpp>

#include "ThreadPool.h"
#include "Task.h"
#include "Logger.h"

namespace sevent{

ThreadPool::ThreadPool(): _started(false){
	LOG_INFO("ThreadPool constructor");
	_taskSize = 0;
	pthread_cond_init(&_cond, NULL);
	pthread_mutex_init(&_mutex, NULL);
}

ThreadPool::ThreadPool(const std::string name):_name(name), _started(false){
	LOG_INFO("ThreadPool constructor");
	_taskSize = 0;
	pthread_cond_init(&_cond, NULL);
	pthread_mutex_init(&_mutex, NULL);
}

ThreadPool::~ThreadPool(){
	LOG_INFO("ThreadPool destructor");
	pthread_cond_destroy(&_cond);
	pthread_mutex_destroy(&_mutex);
}

void ThreadPool::clean(){
	if (_started) {
		stop();
	}
	//delete all thread, task
}

void ThreadPool::start(int number){
	assert(!_started);
	assert(_threads.empty());
	_started = true;
	_threadsNum = number;
	LOG_INFO("ThreadPool " << name() << " started with number " << _threadsNum);
	for (int i = 0; i < _threadsNum; ++i) {
		Thread *t = new Thread(boost::bind(&ThreadPool::threadRun, this));
		_threads.push_back(t);
		t->start();
	}
	return;
}

int ThreadPool::wait(){
	for(std::vector<Thread *>::iterator iter = _threads.begin(); iter != _threads.end(); ++iter) {
		(*iter)->join();
	}
	return 0;
}

int ThreadPool::stop(){
	_started = false;
	pthread_cond_broadcast(&_cond);
//	for_each(_threads.begin(), _threads.end(),[](Thread * t) {t->join();});
	for(std::vector<Thread *>::iterator iter = _threads.begin(); iter != _threads.end(); ++iter) {
		(*iter)->join();
	}
	return 0;
}

int ThreadPool::addTask(const TaskPtr &t){
	
	int ret = 0;
	pthread_mutex_lock(&_mutex);

	_tasks.push(t);
	_taskSize++;
	LOG_DEBUG("ThreadPool " << name() << " current task size " << _taskSize);
	pthread_mutex_unlock(&_mutex);
	pthread_cond_signal(&_cond);
	return ret;
}

TaskPtr ThreadPool::getNextTask(){
	pthread_mutex_lock(&_mutex);
	while(_tasks.empty() && _started) {
		pthread_cond_wait(&_cond, &_mutex);
	}
	TaskPtr t;
	if(!_tasks.empty()) {
		t = _tasks.front();
		_tasks.pop();
		_taskSize--;
	}
	pthread_mutex_unlock(&_mutex);
	return t;
}

int ThreadPool::threadRun(){
	LOG_INFO("ThreadPool " << name() << " workThread start");
	
	while (_started) {
		TaskPtr t = getNextTask();
		int ret = t->execute();
		if (ret < 0) {
			LOG_ERROR("ThreadPool " << name() << "task execution error");
		}
	}

	LOG_INFO("ThreadPool stoped");
	return 0;
}


}

