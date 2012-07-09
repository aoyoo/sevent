#pragma once
#include "public.h"
#include "buffer.h"
#include "thread.h"
#include "socket_event.h"
//#include "event_manager.h"
#include "Log.h"
using namespace std;

#pragma pack(push)
#pragma pack(1)
class head{
public:

//	char syn;
	int version;
	int command;
	char password[8];
	int type;
	int status;
	int src;
	int dest;
	unsigned long long sn_high;
	unsigned long long sn_low;
	int length;

	head()
	{
//		syn = 'a';
		version = 421;
		command = 0;
		memset(password ,'a',8);
		type = 0;
		status = 0;
		src = 0;
		dest = 0;
		sn_high = 0;
		sn_low = 0;
		length = 0;
	}
	
	void ntoh()
	{
		version = ntohl(version);
		command = ntohl(command);
		type = ntohl(type);
		status = ntohl(status);
		src = ntohl(src);
		dest = ntohl(dest);
		length = ntohl(length);
		sn_high = ntohll(sn_high);
		sn_low = ntohll(sn_low);
	}

	void show(){
		LOG_DEBUG_VA("the head version:%d, cmd:%d, length:%d", version, command, length);
	}
};
#pragma pack(pop)

class octet_ostream
{
	buffer buf;
public:
	octet_ostream &operator<< (int i) {int oi = htonl(i); buf.append_data(&oi, sizeof(int)); return *this; }
	octet_ostream &operator<< (char c) {buf.append_data(&c, sizeof(char)); return *this;}
	octet_ostream &operator<< (const char *cstr) { buf.append_data(cstr, strlen(cstr) + 1); return *this; }
	octet_ostream &operator<< (unsigned long long ll) { unsigned long long nll =  htonll(ll); buf.append_data(&nll, sizeof(nll)); return *this;}
	//octet_ostream &operator<< (head &h);
	octet_ostream &append_data(const void *data, int length) { buf.append_data(data, length); return *this; }

	buffer &get_buffer() {return buf;}
};

class octet_istream 
{
	buffer buf;
public:
	octet_istream(buffer b): buf(b){}
	octet_istream &operator>> (char &c) { buf.drain_data(&c, sizeof(char)) ; return *this; }
	octet_istream &operator>> (int &i) {buf.drain_data(&i, sizeof(int)); i = ntohl(i);return *this; }
	octet_istream &operator>> (unsigned long long &ll) { buf.drain_data(&ll, sizeof(long long));  ll = ntohll(ll);  return *this; }
	//octet_istream & drain_data(void *data, int length) { _buf->drain_data(data, length); return *this; }
};

octet_ostream &operator<<(octet_ostream &oos, head &h);

class socket_event;
class message
{
	socket_event *message_socket;
	//keep the socket for this message
	head message_head;
	//buffer  message_buffer;
	//the head &buf just used to send message
public:
	void set_head(head h){message_head = h;}
	void set_socket_event(socket_event *se){message_socket = se;}
	void set_length(int i){message_head.length = i;}
	//void set_syn(char c){message_head.syn = c;}
	
	socket_event *get_socket_event(){return message_socket;}
	int get_socket(){return message_socket->get_socket_fd();}
	head &get_head(){return message_head;}
	//buffer &get_buffer(){return message_buffer;}
	
	virtual int pack(buffer &buf) = 0;
	virtual int unpack(buffer &buf) = 0;

	virtual int response(){};
	//int append_message_head();
	//int append_message_data(char *bgn, int len);
};

class message_builder_base
{
public:
	virtual message *build_message(const head &msg_head, buffer &buf) = 0;
};

template <class msg_type>
class message_builder : public message_builder_base
{
public:
	message *build_message(const head &msg_head, buffer &buf)
	{
		msg_type *msg = new msg_type();
		msg->set_head(msg_head);
		if (msg->unpack(buf) != 0) {
			LOG_ERROR_VA("messsage unpack failed");
			delete msg;
			return 0;
		}
		return msg;
	}
};

/*
template <class msg_type>
class message_builder 
{
public:
	message *build_message(const head &msg_head, buffer &buf)
	{
		msg_type *msg = new msg_type;
		msg->set_head(msg_head);
		if (msg->unpack(buf) != 0) {
			LOG_ERROR_VA("messsage unpack failed");
			delete msg;
			return 0;
		}
		return msg;
	}
};
*/

