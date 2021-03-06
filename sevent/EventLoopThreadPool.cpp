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
}

EventLoopThreadPool::~EventLoopThreadPool(){
}

//void EventLoopThreadPool::start(const ThreadInitCallback& cb) //DIFF
void EventLoopThreadPool::start()
{
	LOG_INFO("EventLoopThreadPool start with thread number " << numThreads_);
	assert(!started_);
	//baseLoop_->assertInLoopThread();// DIFF
	assert(numThreads_); //DIFF
	
	started_ = true;
	for (int i = 0; i < numThreads_; ++i){
		EventLoopThread* t = new EventLoopThread;
		stringstream nameSS;
		nameSS << name_ << i;
		t->getEventLoop()->name_ = nameSS.str(); 
		//t->setInitFunc() //TODO user set some init func for every thread
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



