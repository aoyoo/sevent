#include "Message.h"
#include <arpa/inet.h>
#include <string.h>

using namespace sevent;

MessageHead::MessageHead()
	:command(0),
	status(0),
	src(998),
	sn(123456789),
	length(-1)
{}

MessageHead::MessageHead(const MessageHead &h){
	command = h.command;
	status = h.status;
	src = h.src;
	sn = h.sn; 
	length = h.length;
}

MessageHead::MessageHead(const char *c){
	command = *(int *)c;
	status = *(int *)(c+4);
	src = *(int *)(c+8);
	sn = *(int *)(c+12);
	length = *(int *)(c+16);
}

void MessageHead::ntoh(){
	command = ntohl(command);
	status = ntohl(status);
	src = ntohl(src);
	sn = ntohl(sn);
	length = ntohl(length);
}

void MessageHead::hton(){
	command = htonl(command);
	status = htonl(status);
	src = htonl(src);
	sn = htonl(sn);
	length = htonl(length);
}

Message::Message()
	:pb_(NULL){
}

Message::Message(const MessageHead &h)
	:head_(h), pb_(NULL){
}

Message::Message(const char* c)
	:head_(c), pb_(NULL){
	//head_.ntoh();
	buf_.assign(c+sizeof(MessageHead), head_.length);
}

int Message::pack(){
	
	if(pb_ == NULL){
		return -1;
	}
	
}

int Message::unpack(){
	
	if(pb_ == NULL){
		return -1;
	}
	
}







