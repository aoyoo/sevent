#ifndef SEVENT_EVENTLOOPTHREAD_H
#define SEVENT_EVENTLOOPTHREAD_H

#include <Condition.h>
#include <Mutex.h>
#include <Thread.h>
#include <Callbacks.h>

#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace sevent{

class EventLoop;

class EventLoopThread : boost::noncopyable{
public:
	
	EventLoopThread(const ThreadInitCallback& cb);
	EventLoopThread();
	~EventLoopThread();
	
	void setInitFunc(const ThreadInitCallback& cb){
		threadInitCallback_ = cb;
	}
	
	EventLoop *getEventLoop(){return loop_.get();} //DIFF
	//scoped_ptr can use as? http://www.boost.org/doc/libs/1_53_0/libs/smart_ptr/scoped_ptr.htm
	
	//EventLoop* startLoop();
	void start();
	void stop();
	
private:
	void threadFunc();
	
	boost::scoped_ptr<EventLoop> loop_;
	bool exiting_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
	
	ThreadInitCallback threadInitCallback_; //maybe for possible listen
};




}

#endif


