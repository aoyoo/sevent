#ifndef DISPATCHSERVER_H
#define DISPATCHSERVER_H

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <string>

#include "Server.h"
#include "Client.h"
#include "Connection.h"
#include "Message.h"
#include "Mutex.h"
#include "InetAddress.h"
#include "Logger.h"

#include "Tunnel.h"

typedef boost::shared_ptr<Tunnel> TunnelPtr;
typedef boost::shared_ptr<sevent::Client> ClientPtr;

class DispatchServer{
public:
	DispatchServer(const std::string name, const sevent::InetAddress &ServerAddr, const sevent::InetAddress &clientAddr);
	~DispatchServer();
	
	void start(int num){server_.setThreadNum(num);server_.start();}
	void setMessageCallback(const sevent::MessageCallback& cb){server_.setMessageCallback(cb);}

	void onServerConnection(const sevent::ConnectionPtr &conn);
	int onServerMessage(const sevent::ConnectionPtr &conn, const sevent::MessagePtr &msg, sevent::Timestamp time);

private:
	sevent::Server server_;
	sevent::InetAddress clientAddr_;
	
	sevent::MutexLock mutex_;

	std::map<std::string, TunnelPtr> tunnels_;
	
	long long count_;
};

#endif

