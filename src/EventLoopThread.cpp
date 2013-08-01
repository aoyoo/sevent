#include <EventLoop.h>
#include <EventLoopThread.h>
#include <boost/bind.hpp>

#include <Logger.h>

using namespace sevent;

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb)
	:loop_(new EventLoop), //DIFF
	exiting_(false),
	thread_(boost::bind(&EventLoopThread::threadFunc, this)),
	mutex_(),
	cond_(mutex_),
	threadInitCallback_(cb) //DIFF
{
	LOG_INFO("EventLoopThread constructor");
}

EventLoopThread::EventLoopThread()
	:loop_(new EventLoop),
	exiting_(false),
	thread_(boost::bind(&EventLoopThread::threadFunc, this)),
	mutex_(),
	cond_(mutex_)
{
	LOG_INFO("EventLoopThread constructor");
}

EventLoopThread::~EventLoopThread()
{
	LOG_INFO("EventLoopThread destructor");
	exiting_ = true;
	loop_->quit();
	thread_.join();
}

void EventLoopThread::start(){
	LOG_DEBUG("EventLoopThread " << getEventLoop()->name_ << " start");
	assert(!thread_.started());
	thread_.start();
	//DIFF
	//EventLoopThread has a EventLoop, dont need to get EventLoop in threadFunc

}

void EventLoopThread::threadFunc() //DIFF
{
	pid_t tid = sevent::gettid();
	loop_->setTid(tid);
	
	LOG_DEBUG("EventLoopThread threadFunc Loop tid " << tid);
	if(threadInitCallback_){
		LOG_DEBUG("EventLoopThread " << getEventLoop()->name_ << " InitCallBack");
		threadInitCallback_();
	}
	loop_->loop();
}


