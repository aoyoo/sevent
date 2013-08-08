#include <EventLoop.h>
#include <Mutex.h>
#include <Channel.h>
#include <Poller.h>
#include <SocketsOps.h>
#include <Logger.h>

#include <boost/bind.hpp>

#include <signal.h>
#include <sys/eventfd.h> 

using namespace sevent;

__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0){
		LOG_ERROR("Failed in eventfd");
		abort();
	}
	return evtfd;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread(){
  return t_loopInThisThread;
}

EventLoop::EventLoop()
	:looping_(false),
	quit_(false),
	eventHandling_(false),
	callingPendingFunctors_(false),
	iteration_(0),
	poller_(new Poller(this)),
	wakeupFd_(createEventfd()),
	wakeupChannel_(new Channel(this, wakeupFd_)),
	currentActiveChannel_(NULL),
	server_(NULL)
{
	t_loopInThisThread = this;
}

EventLoop::~EventLoop(){
	LOG_INFO("EventLoop destructor");
	t_loopInThisThread = NULL;
}

void EventLoop::loop(){
	wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));
	wakeupChannel_->enableReading();
	
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;
	LOG_DEBUG("EventLoop " << name_ << " start loop");
	
	while(!quit_){
		activeChannels_.clear();
		pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
		++iteration_;
		eventHandling_ = true;
		for (ChannelList::iterator it = activeChannels_.begin();it != activeChannels_.end(); ++it){
			currentActiveChannel_ = *it;
			currentActiveChannel_->handleEvent(pollReturnTime_);
		}
		currentActiveChannel_ = NULL;
		eventHandling_ = false;
		doPendingFunctors();
	}
	LOG_DEBUG("EventLoop " << name_ << " stop loop");
	looping_ = false;
}

void EventLoop::quit(){
	quit_ = true;
	if (!isInLoopThread()){
		wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	LOG_DEBUG("EventLoop " << name_ << " runInLoop");
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(cb);
  }
}

void EventLoop::queueInLoop(const Functor& cb)
{
	LOG_DEBUG("EventLoop " << name_ << " queueInLoop");
	{
	MutexLockGuard lock(mutex_);
	pendingFunctors_.push_back(cb);
	}
	
	if (!isInLoopThread() || callingPendingFunctors_)
	{
	  wakeup();
	}
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  if (eventHandling_)
  {
    assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  }
  poller_->removeChannel(channel);
}

void EventLoop::abortNotInLoopThread()
{
  LOG_ERROR("EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " << gettid());
	exit(1);
}

void EventLoop::wakeup()
{
	LOG_DEBUG("EventLoop::wakeup");
	uint64_t one = 1;
	ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
	  LOG_ERROR("EventLoop::wakeup() writes " << n << " bytes instead of 8");
	}
}

void EventLoop::handleRead()
{
	LOG_DEBUG("EventLoop::handlerRead");
  uint64_t one = 1;
  ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR("EventLoop::handleRead() reads " << n << " bytes instead of 8");
  }
}

void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
  MutexLockGuard lock(mutex_);
  functors.swap(pendingFunctors_);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors_ = false;
}

void EventLoop::printActiveChannels() const
{
  for (ChannelList::const_iterator it = activeChannels_.begin();
      it != activeChannels_.end(); ++it)
  {
    const Channel* ch = *it;
    LOG_TRACE("{" << ch->reventsToString() << "} ");
  }
}




