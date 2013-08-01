#ifndef SEVENT_CHANNEL_H
#define SEVENT_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>

#include "Callbacks.h"
#include "Timestamp.h"
#include "EventLoop.h"

namespace sevent{
using std::string;

class Channel : boost::noncopyable{
public:
	Channel(EventLoop *loop, int fd); 
	~Channel(); 

	void handleEvent(Timestamp receiveTime);

	void setReadCallback(const ReadEventCallback& cb){ readCallback_ = cb; }
	void setWriteCallback(const EventCallback& cb){ writeCallback_ = cb; }
	void setCloseCallback(const EventCallback& cb){ closeCallback_ = cb; }
	void setErrorCallback(const EventCallback& cb){ errorCallback_ = cb; }

	void tie(const boost::shared_ptr<void>&);

	int fd() const { return fd_; }
	int events() const { return events_; }
	void set_revents(int revt) { revents_ = revt; } 
	bool isNoneEvent() const { return events_ == kNoneEvent; }

	void enableReading() { events_ |= kReadEvent; update(); }
	void enableWriting() { events_ |= kWriteEvent; update(); }
	void disableWriting() { events_ &= ~kWriteEvent; update(); }
	void disableAll() { events_ = kNoneEvent; update(); }
	bool isWriting() const { return events_ & kWriteEvent; }

	// for Poller
	int index() { return index_; }
	void set_index(int idx) { index_ = idx; }

	// for debug
	string reventsToString() const;
	
	EventLoop* ownerLoop() { return loop_; }
	void remove();
	
private:
	void update();
	void handleEventWithGuard(Timestamp receiveTime);
	
	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;
	
	EventLoop* loop_;
	const int  fd_;
	int        events_;
	int        revents_;
	int        index_;

	boost::weak_ptr<void> tie_;
	bool tied_;
	bool eventHandling_;
	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
};



}

#endif


