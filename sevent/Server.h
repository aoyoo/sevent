#ifndef SEVENT_SERVER_H
#define SEVENT_SERVER_H

#include "Callbacks.h"
#include "Connection.h"
#include "Message.h"

#include <map>
#include <string>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace sevent{

class Acceptor;
class InetAddress;
class ThreadPool;
class EventLoop;
class EventLoopThread;
class ThreadPool;

class Server : boost::noncopyable 
{
public:
	
	Server(const std::string &name, const InetAddress &addr); 
	~Server();
	
	void start();
	void stop();
	bool started() const {return started_;}
	
	void setThreadNum(int threadNum = 1){
		threadNum_ = threadNum;
	}
	
	void setMessageCallback(const MessageCallback& f){messageThreadFunc_ = f;}
	MessageCallback getMessageThreadFunc(){return messageThreadFunc_;}
	void setConnectionCallback(const ConnectionCallback &f){connectionCallback_ = f;}

	ThreadPool *getThreadPool();

	std::string &name() {return name_;}
private:
  /// not thread safe, but in loop
  void newConnection(int sockfd, const InetAddress& peeraddr);
  /// thread safe.
  void removeConnection(const ConnectionPtr& conn);
  /// not thread safe, but in loop
  void removeConnectionInLoop(const ConnectionPtr& conn);
  typedef std::map<string, ConnectionPtr> connectionMap;

  string hostport_;
  string name_;

  boost::scoped_ptr<EventLoopThread> eventThread_;
  boost::scoped_ptr<Acceptor> acceptor_; // for eventThread_
  boost::scoped_ptr<ThreadPool> workThreadPool_;
  int threadNum_;

	ConnectionCallback connectionCallback_;
  MessageCallback messageThreadFunc_;
  bool started_;
  int nextConnId_;
  typedef std::map<std::string, ConnectionPtr> ConnectionMap;
  ConnectionMap connections_;

};

}


#endif

