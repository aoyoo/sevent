#ifndef SEVENT_MESSAGE_H
#define SEVENT_MESSAGE_H

#include <iostream>
#include <sstream>
#include <string>
#include <boost/enable_shared_from_this.hpp>

#include <google/protobuf/message.h>
#include "Public.h"

namespace sevent{

const int MESSAGE_HEAD_LEN = 20;

#pragma pack(push)
#pragma pack(1)
class MessageHead
{
public:
	MessageHead();
	MessageHead(const MessageHead &h);
	MessageHead(const char *c);

	int command;
	int status;
	int src;
	int sn;
	int length;
	
	void ntoh();
	void hton();

	std::string toString(){
		std::stringstream ss;
		ss << "command " << command << " status " << status << " src " << src << " sn " << sn << " length " << length;
		return ss.str();
	}
};
#pragma pack(pop)

class Message : public boost::enable_shared_from_this<Message>
{
public:
	Message();
	Message(const char* c);
	Message(const MessageHead &h);
	
	const MessageHead &getHead(){return head_;}
	
	int pack();
	int unpack();
	
	int getTotalLen(){
		return (sizeof(MessageHead) + head_.length);
	}
	
	std::string toString(){
		return head_.toString();
	}
	
private:
	MessageHead head_;
	std::string buf_;
	google::protobuf::Message *pb_;
};

}

#endif


