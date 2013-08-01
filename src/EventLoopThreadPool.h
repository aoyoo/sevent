#ifndef SEVENT_EVENTLOOPTHREADPOOL_H
#define SEVENT_EVENTLOOPTHREADPOOL_H

#include <Condition.h>
#include <Mutex.h>

#include <vector>
#include <string>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace sevent{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : boost::noncopyable{
public:
	
	EventLoopThreadPool(); //DIFF
	~EventLoopThreadPool();
	
	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
	void start();
	void stop(){;}
	
	EventLoop* getNextLoop();

	std::string name_;
private:
	bool started_;
	int numThreads_;
	int next_;
	
	boost::ptr_vector<EventLoopThread> threads_;
	std::vector<EventLoop*> loops_; //??
};





}


#endif


