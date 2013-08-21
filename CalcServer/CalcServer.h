#include "Server.h"
#include "Connection.h"
#include "Message.h"
#include "InetAddress.h"
#include "Logger.h"

#include <stdio.h>
#include <signal.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace sevent;
using namespace std;

class CalcServer{
public:
	CalcServer(const std::string name, const InetAddress &addr);
	~CalcServer();
	
	void set_cf_path(const string &p) { _cf_path = p; }
	void set_gf_path(const string &p) { _gf_path = p; }
	void set_lf_path(const string &p) { _lf_path = p; }
	void set_tl_path(const string &p) { _tl_path = p; }
	void set_npx_path(const string &p) { _npx_path = p; }
	void set_iq_path(const string &p) { _iq_path = p; }
	void set_is_path(const string &all, const string &shoe) { _is_all_path = all; _is_shoe_path = shoe;}
	void set_ai_path(const string &p) { _ai_path = p; }
	void set_tj_path(const string &p) { _tj_path = p; }

	void set_cf3_path(const string &p) { _cf3_path = p; }
	void set_sf_path(const string &p) { _sf_path = p; }

	void start(int num){server_.setThreadNum(num);server_.start();}
	void setMessageCallback(const MessageCallback& cb){server_.setMessageCallback(cb);}

	int setup();
	void onConnection(const ConnectionPtr & conn);
	int taskFunc(const ConnectionPtr &c, const MessagePtr &msg, Timestamp time);

private:
	Server server_;

	long long count;
	string _cf_path;
	string _gf_path;
	string _lf_path;
	string _tl_path;
	string _npx_path;
	string _iq_path;
	string _is_all_path;
	string _is_shoe_path;
	string _ai_path;
	string _tj_path;
	
	string _cf3_path;
	string _sf_path;
	

};


