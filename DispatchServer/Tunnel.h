#ifndef TUNNEL_H
#define TUNNEL_H

#include <stdio.h>
#include <signal.h>
#include <map>
#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Server.h"
#include "Client.h"
#include "Connection.h"
#include "Message.h"
#include "Mutex.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Callbacks.h"


using namespace sevent;
using namespace std;


class Tunnel : public boost::enable_shared_from_this<Tunnel>,
               boost::noncopyable{
public:
	Tunnel(sevent::EventLoop* loop, const sevent::InetAddress& serverAddr, const sevent::ConnectionPtr& serverConn);
	~Tunnel();

	void connect()
	{
		client_->connect();
	}

	void disconnect()
	{
		client_->disconnect();
	}

	void onConnection(const sevent::ConnectionPtr &conn);
	int onMessage(const sevent::ConnectionPtr &conn, const sevent::MessagePtr &msg, sevent::Timestamp time);

	void onHighWaterMark(const ConnectionPtr& conn, size_t bytesToSent);

	static void onHighWaterMarkWeak(const boost::weak_ptr<Tunnel>& wkTunnel, 
                                  const ConnectionPtr& conn, 
                                  size_t bytesToSent);

private:

	InetAddress addr_;
	
	ConnectionPtr serverConn_;
	boost::scoped_ptr<Client> client_;

	long long count_;
};

#endif

