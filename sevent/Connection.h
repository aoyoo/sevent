#ifndef SEVENT_CONNECTION_H
#define SEVENT_CONNECTION_H

#include "Callbacks.h"
#include "Public.h"
#include "Mutex.h"
#include "Timestamp.h"
#include "Buffer.h"
#include "InetAddress.h"

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace sevent
{

class Channel;
class EventLoop;
class Socket;

using std::string;

class Connection;

class Connection : boost::noncopyable,
                      public boost::enable_shared_from_this<Connection>
{
 public:
  Connection(EventLoop* loop,
                const string& name,
                int sockfd,
                const InetAddress& localAddr,
                const InetAddress& peerAddr);
  ~Connection();

  EventLoop* getLoop() const { return loop_; }
  const string& name() const { return name_; }
  const InetAddress& localAddress() { return localAddr_; }
  const InetAddress& peerAddress() { return peerAddr_; }
  bool connected() const { return state_ == kConnected; }

  void send(const void* message, size_t len);
  void send(Buffer* message);
  //void send(const Message *msg); //DIFF for future

  void shutdown();
  void setTcpNoDelay(bool on);

  void setContext(const boost::any& context)
  { context_ = context; }

  const boost::any& getContext() const
  { return context_; }

  boost::any* getMutableContext()
  { return &context_; }


  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

  /// Internal use only.
  void setCloseCallback(const CloseCallback& cb)
  { closeCallback_ = cb; }

  // called when TcpServer accepts a new connection
  void connectEstablished();   // should be called only once
  // called when TcpServer has removed me from its map
  void connectDestroyed();  // should be called only once

  Buffer* inputBuffer()
  { return &inputBuffer_; }

 private:
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
  void handleRead(Timestamp receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();
  //void sendInLoop(string&& message);
  void sendInLoop(const string& message);
  void sendInLoop(const void* message, size_t len);
  void shutdownInLoop();
  void setState(StateE s) { state_ = s; }

  EventLoop* loop_;
  string name_;
  StateE state_;  // FIXME: use atomic variable
  //we don't expose those classes to client.

  boost::scoped_ptr<Socket> socket_;
  boost::scoped_ptr<Channel> channel_;
  InetAddress localAddr_;
  InetAddress peerAddr_;
  MessageCallback messageCallback_;
  CloseCallback closeCallback_;
  Buffer inputBuffer_;
  Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
  boost::any context_;
  // FIXME: creationTime_, lastReceiveTime_
  //        bytesReceived_, bytesSent_
};

}


#endif

