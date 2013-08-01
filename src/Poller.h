#ifndef SEVENT_POLLER_H
#define SEVENT_POLLER_H

#include <Timestamp.h>
#include <EventLoop.h>

#include <vector>
#include <map>
#include <boost/noncopyable.hpp>

#include <sys/epoll.h>
#include <poll.h>

namespace sevent
{

class Channel;

class Poller : boost::noncopyable
{
public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
 ~Poller();

 Timestamp poll(int timeoutMs, ChannelList* activeChannels);

  void updateChannel(Channel* channel);

 void removeChannel(Channel* channel);

 // static Poller* newDefaultPoller(EventLoop* loop);

  void assertInLoopThread()
  {
    ownerLoop_->assertInLoopThread();
  }

 private:
  EventLoop* ownerLoop_;

  static const int kInitEventListSize = 16;
  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;
  typedef std::map<int, Channel*> ChannelMap;

  int epollfd_;
  EventList events_;
  ChannelMap channels_; //DIFF 这个有什么呢？

};

}


#endif

