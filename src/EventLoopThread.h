#ifndef SEVENT_EVENTLOOPTHREAD_H
#define SEVENT_EVENTLOOPTHREAD_H

#include <Condition.h>
#include <Mutex.h>
#include <Thread.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace sevent{

class EventLoop;

class EventLoopThread : boost::noncopyable{
public:
	typedef boost::function<void()> ThreadInitCallback;
	
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
	void stop(){;} //FIXME
	
private:
	void threadFunc();
	
	boost::scoped_ptr<EventLoop> loop_; //DIFF
	bool exiting_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
	
	ThreadInitCallback threadInitCallback_; //DIFF this for possible listen
};




}

#endif


