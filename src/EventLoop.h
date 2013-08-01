#ifndef SEVENT_EVENTLOOP_H
#define SEVENT_EVENTLOOP_H

#include <vector>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <Mutex.h>
#include <Socket.h>
#include <Thread.h>
#include <Timestamp.h>

namespace sevent{

class Channel;
class Poller;
class Server;

class EventLoop : boost::noncopyable
{
public:
	typedef boost::function<void()> Functor;
	
	EventLoop();
	~EventLoop();
	
	void loop();
	void quit();
  
	int64_t iteration() const { return iteration_; }
	
	bool looping() const {return looping_;}
	
	void runInLoop(const Functor& cb);
	void queueInLoop(const Functor& cb);
	
	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}
	
	bool isInLoopThread() const { return threadId_ == gettid(); }
	// bool callingPendingFunctors() const { return callingPendingFunctors_; }
	bool eventHandling() const { return eventHandling_; }
	
	static EventLoop* getEventLoopOfCurrentThread();
	
	void setServer(Server* svr){server_ = svr;}
	Server* getServer(){return server_;}
	
	void setTid(pid_t t){threadId_ = t;} //set threadId_ after thread start
	pid_t getTid(){return threadId_;}

	string name_;
	
private:
	void abortNotInLoopThread();

	void handleRead();  // waked up
	void doPendingFunctors();
	void printActiveChannels() const; // DEBUG

	typedef std::vector<Channel*> ChannelList;
	bool looping_;
	bool quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	
	int64_t iteration_;
	pid_t threadId_;
	Timestamp pollReturnTime_;
	
	boost::scoped_ptr<Poller> poller_;
	
	int wakeupFd_;
	boost::scoped_ptr<Channel> wakeupChannel_;
	
	ChannelList activeChannels_;
	Channel* currentActiveChannel_;

	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_; // @BuardedBy mutex_
	
	Server *server_;
};


}


#endif


