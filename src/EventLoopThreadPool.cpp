#include <EventLoopThreadPool.h>
#include <EventLoopThread.h>
#include <EventLoop.h>
#include <Logger.h>

using namespace sevent;

//EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop) //DIFF
EventLoopThreadPool::EventLoopThreadPool()
	:started_(false),
	numThreads_(0),
	next_(0)
{
	LOG_INFO("EventLoopThreadPool constructor");
}

EventLoopThreadPool::~EventLoopThreadPool(){
	LOG_INFO("EventLoopThreadPool destructor");
}

//void EventLoopThreadPool::start(const ThreadInitCallback& cb) //DIFF
void EventLoopThreadPool::start()
{
	LOG_INFO("EventLoopThreadPool start " << numThreads_);
	assert(!started_);
	//baseLoop_->assertInLoopThread();// DIFF
	assert(numThreads_); //DIFF
	
	started_ = true;
	for (int i = 0; i < numThreads_; ++i){
		EventLoopThread* t = new EventLoopThread;
		stringstream nameSS;
		nameSS << name_ << i;
		t->getEventLoop()->name_ = nameSS.str(); 
		threads_.push_back(t);
		t->start(); //DIFF //correct??
	}
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
	assert(!threads_.empty());
	next_ = next_ % threads_.size();
	EventLoop *loop = threads_[next_].getEventLoop();
	++next_;
	LOG_DEBUG("EventLoopThreadPool::getNextLoop " << loop->name_);
	return loop;
}



