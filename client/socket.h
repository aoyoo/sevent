#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <string.h>
using std::string;

class address : public sockaddr
{
public:
	address()
	{
		sockaddr *sa= this; // work without cast?
		struct sockaddr_in *sai = reinterpret_cast<sockaddr_in *>(sa);
		memset(sai, 0, sizeof(sockaddr));
	}
	address(const string &ip, int port) 
	{
		sockaddr *sa= this; // work without cast?
		struct sockaddr_in *sai = reinterpret_cast<sockaddr_in *>(sa);
		memset(sai, 0, sizeof(sockaddr));
		sai->sin_family = AF_INET;
		sai->sin_port = htons(port);
		sai->sin_addr.s_addr = inet_addr(ip.c_str());
	}
	//sockaddr *operator &() { return static_cast<sockaddr *>(this); };
	string ip() const
	{
		const sockaddr_in *sai = reinterpret_cast<const sockaddr_in *>(static_cast<const sockaddr *>(this));
		return inet_ntoa(sai->sin_addr);
	}
	int port() const
	{
		const sockaddr_in *sai = reinterpret_cast<const sockaddr_in *>(static_cast<const sockaddr *>(this));
		return ntohs(sai->sin_port);
	}
	int length() const {return sizeof(sockaddr); }
};
/*
class address 
{
	sockaddr _addr;
public:
	address()
	{
		memset(&_addr, 0, sizeof(sockaddr));
	}
	address(const address &rhs) { memcpy(&_addr, &rhs._addr, sizeof(rhs._addr)); }
	address &operator=(const address &rhs) { memcpy(&_addr, &rhs._addr, sizeof(rhs._addr)); }
	address(const string &ip, int port)
	{
		memset(&_addr, 0, sizeof(sockaddr));
		sockaddr_in *sai = reinterpret_cast<sockaddr_in *>(&_addr);
		sai->sin_family = AF_INET;
		sai->sin_port = htons(port);
		sai->sin_addr.s_addr = inet_addr(ip.c_str());
	}
	string ip() const 
	{
		const sockaddr_in *sai = reinterpret_cast<const sockaddr_in *>(&_addr);
		return inet_ntoa(sai->sin_addr);
	}
	int port() const 
	{
		const sockaddr_in *sai = reinterpret_cast<const sockaddr_in *>(&_addr);
		return ntohs(sai->sin_port);
	}
	const sockaddr *addr() const { return &_addr; }
	sockaddr *addr() { return &_addr; }
	int length() const { return sizeof(sockaddr); }
};
*/
int tcp_read_ms(int sock, void *data, int length, int msecs);
int tcp_write_ms(int sock, const void *data, int length, int msecs);
int check_alive(int sock);
