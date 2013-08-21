#ifndef SEVENT_CLIENT_H
#define SEVENT_CLIENT_H

#include <boost/noncopyable.hpp>

#include "Mutex.h"
#include "Connection.h"

namespace sevent
{

class Connector;
typedef boost::shared_ptr<Connector> ConnectorPtr;

class Client : boost::noncopyable
{
 public:
  Client(EventLoop* loop,
            const InetAddress& serverAddr,
            const string& name);
  ~Client();  // force out-line dtor, for scoped_ptr members.

  void connect();
  void disconnect();
  void stop();

  void send(const void* message, size_t len);

  ConnectionPtr connection() const //DIFF why lock?
  {
    MutexLockGuard lock(mutex_);
    return connection_;
  }

  EventLoop* getLoop() const { return loop_; }
  bool retry() const; //TODO 
  void enableRetry() { retry_ = true; }

  // Set message callback.
  // Not thread safe.
  void setConnectionCallback(const ConnectionCallback &f){connectionCallback_ = f;}
  void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

 private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const ConnectionPtr& conn);

  EventLoop* loop_;
  ConnectorPtr connector_; // avoid revealing Connector
  const string name_;

	ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;

  bool retry_;   // atmoic
  bool connect_; // atomic
  // always in loop thread
  int nextConnId_;
  mutable MutexLock mutex_;
  ConnectionPtr connection_; // @BuardedBy mutex_
};


}

#endif

