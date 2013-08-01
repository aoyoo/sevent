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
class EventLoopThreadPool;

class Server : boost::noncopyable 
{
public:
	
	Server(const std::string &name, const InetAddress &addr); 
	//DIFF 1.Server has EventLoop, 
	//2.Server has a EventLoopThread for Acceptor
	//3.Server has EventLoopThreadPool for io & work
	~Server();
	
	void start();
	void stop();
	bool started() const {return started_;}
	
	int setThreadNum(int threadNum = 1){
		threadNum_ = threadNum;
	}
	
	void setTaskCallback(const TaskCallback& f){taskThreadFunc_ = f;}
	TaskCallback getTaskThreadFunc(){return taskThreadFunc_;}
	
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

  boost::scoped_ptr<EventLoopThread> listenLoopThread_;
  boost::scoped_ptr<Acceptor> acceptor_; // for listenLoopThread_
  boost::scoped_ptr<EventLoopThreadPool> ioLoopThreadPool_;
  int threadNum_;

  //MessageCallback messageCallback_;
  TaskCallback taskThreadFunc_;
  bool started_;
  int nextConnId_;
  typedef std::map<std::string, ConnectionPtr> ConnectionMap;
  ConnectionMap connections_;

};

}

#endif

