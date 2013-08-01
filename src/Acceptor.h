#ifndef SEVENT_ACCEPTOR_H
#define SEVENT_ACCEPTOR_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <Channel.h>
#include <Socket.h>

namespace sevent{

class EventLoop;
class InetAddress;

class Acceptor : boost::noncopyable{
public:

	Acceptor(EventLoop* loop, const InetAddress& listenAddr); //DIFF
	~Acceptor();

	void setNewConnectionCallback(const NewConnectionCallback &f){newConnectionCallback_=f;}
	
	bool listening() const {return listening_;}
	void listen();
	
private:
	void handleRead();
	
	EventLoop* loop_;
	Socket acceptSocket_;
	Channel acceptChannel_;
	NewConnectionCallback newConnectionCallback_;
	bool listening_;
	//int idleFd_; // ?this for what? //DIFF
	
};


}

#endif


