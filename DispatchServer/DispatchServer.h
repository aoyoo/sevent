#include "Server.h"
#include "Client.h"
#include "Connection.h"
#include "Message.h"
#include "Mutex.h"
#include "InetAddress.h"
#include "Logger.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <map>
#include <string>

using namespace sevent;
using namespace std;

class DispatchServer{
public:
	typedef boost::shared_ptr<Client> ClientPtr;
	DispatchServer(const std::string name, const InetAddress &addr);
	~DispatchServer();
	
	void start(int num){server_.setThreadNum(num);server_.start();}
	void setMessageCallback(const MessageCallback& cb){server_.setMessageCallback(cb);}

	void onConnection(const ConnectionPtr &conn);
	int onMessage(const ConnectionPtr &conn, const MessagePtr &msg, Timestamp time);

private:
	Server server_;
	InetAddress addr_;

	MutexLock mutex_;
	map<string, ClientPtr>connections_;

	long long count;
};


