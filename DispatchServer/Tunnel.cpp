#include "Tunnel.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

Tunnel::Tunnel(sevent::EventLoop* loop, const sevent::InetAddress& serverAddr, const sevent::ConnectionPtr& serverConn)
	:client_(new Client(loop, serverAddr, serverConn->name())), addr_(serverAddr), serverConn_(serverConn), count_(0)
{
	client_->setConnectionCallback(boost::bind(&Tunnel::onConnection, this, _1));
	client_->setMessageCallback(boost::bind(&Tunnel::onMessage, this, _1, _2, _3));
	serverConn_->setHighWaterMarkCallback(
				      boost::bind(&Tunnel::onHighWaterMarkWeak, boost::weak_ptr<Tunnel>(shared_from_this()), _1, _2),
							10*1024*1024);
	LOG_INFO("Tunnel");
}

Tunnel::~Tunnel(){
	LOG_INFO("~Tunnel");
}

void Tunnel::onConnection(const sevent::ConnectionPtr &conn)
{
	LOG_DEBUG("Tunnel " << conn->name() << (conn->connected() ? " UP" : " DOWN"));
	if (conn->connected())
	{
		conn->setHighWaterMarkCallback(
					boost::bind(&Tunnel::onHighWaterMarkWeak, boost::weak_ptr<Tunnel>(shared_from_this()), _1, _2),
					10*1024*1024);
		serverConn_->setContext(conn);
		if (serverConn_->inputBuffer()->readableBytes() > 0)
		{
			//conn->send(serverConn_->inputBuffer());  //FIXME
		}
	}
	else
	{
		client_->setConnectionCallback(0);
		client_->setMessageCallback(0);
		if (serverConn_)
		{
			serverConn_->setContext(boost::any());
			serverConn_->shutdown(); //之后serverConn就不可以写了
		}
	}
}

int Tunnel::onMessage(const sevent::ConnectionPtr &conn, const sevent::MessagePtr &msg, sevent::Timestamp time){
	long long count = __sync_add_and_fetch(&count_, 1);
	if(count % 1000 == 0)
		LOG_INFO("Tunnel onMessage count " << count);
	return 0;
}

void Tunnel::onHighWaterMark(const ConnectionPtr& conn, size_t bytesToSent)
{
	LOG_INFO("onHighWaterMark " << conn->name() << " bytes " << bytesToSent);
	disconnect();
}

void Tunnel::onHighWaterMarkWeak(const boost::weak_ptr<Tunnel>& wkTunnel, 
                                 const ConnectionPtr& conn, 
                                 size_t bytesToSent)
{
	LOG_ERROR("onHighWaterMark " << conn->name() << " bytes " << bytesToSent);
	boost::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
	if (tunnel)
	{
		tunnel->onHighWaterMark(conn, bytesToSent);
	}
}


