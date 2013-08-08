#include <Poller.h>
#include <Channel.h>
#include <Logger.h>

#include <boost/static_assert.hpp>

#include <assert.h>
#include <errno.h>

using namespace sevent;

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

Poller::Poller(EventLoop* loop)
  : ownerLoop_(loop),
    epollfd_(::epoll_create(1)),
    events_(kInitEventListSize)
{
	if (epollfd_ < 0)
	{
		LOG_ERROR("Poller::Poller");
		exit(1);
	}
}

Poller::~Poller()
{
	LOG_INFO("Poller::Poller destructor");
  ::close(epollfd_);
}

Timestamp Poller::poll(int timeoutMs, ChannelList* activeChannels)
{
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE(numEvents << " events happended");
    fillActiveChannels(numEvents, activeChannels);
    if (static_cast<size_t>(numEvents) == events_.size())
    {
      events_.resize(events_.size()*2); //动态的增加events_ 数组大小，且没有缩小
    }
  }
  else if (numEvents == 0)
  {
    //LOG_TRACE("nothing happended");
  }
  else
  {
    LOG_ERROR("Poller::poll()" << strerror(errno));
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
  assert(static_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
#ifndef NDEBUG
    int fd = channel->fd();
		LOG_DEBUG(fd << " get Event " << events_[i].events);
    ChannelMap::const_iterator it = channels_.find(fd);
    assert(it != channels_.end());
    assert(it->second == channel);
#endif
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

void Poller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG_TRACE("updateChannel fd = " << channel->fd() << " events = " << channel->events());
  const int index = channel->index();
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    int fd = channel->fd();
    if (index == kNew)
    {
      assert(channels_.find(fd) == channels_.end());
      channels_[fd] = channel;
    }
    else // index == kDeleted
    {
      assert(channels_.find(fd) != channels_.end());
      assert(channels_[fd] == channel);
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    // update existing one with EPOLL_CTL_MOD/DEL
    int fd = channel->fd();
    (void)fd;
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(index == kAdded);
    if (channel->isNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void Poller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  int fd = channel->fd();
  LOG_TRACE("fd = " << fd);
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());
  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  size_t n = channels_.erase(fd);
  (void)n; //??
  assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}

void Poller::update(int operation, Channel* channel)
{
  struct epoll_event event;
  bzero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
  {
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_ERROR("epoll_ctl op=" << operation << " fd=" << fd); //DIFF why ERROR
    }
    else
    {
      LOG_ERROR("epoll_ctl op=" << operation << " fd=" << fd);
    }
  }
}


