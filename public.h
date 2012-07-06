#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <vector>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h> /*for struct sockaddr_in*/
#include <arpa/inet.h>

#define EXTRACT_PORT 50001
#define EXTRACT_IP "127.0.0.1"

#define MAXEPOLLSIZE 20
#define MAXBUF 128

static bool is_big_endian()
{
	unsigned int i = 0x1;
	unsigned char *p = reinterpret_cast<unsigned char *>(&i);
	return p[0] == 0;
}

static void reverse_bytes(unsigned char *bytes, int nbytes) 
{
	int left = 0, right = nbytes - 1;
	while (left < right) {
		unsigned char c = bytes[left];
		bytes[left++] = bytes[right];
		bytes[right--] = c;
	}
}

static unsigned long long htonll(unsigned long long ll)
{
	if (!is_big_endian())
		reverse_bytes(reinterpret_cast<unsigned char *>(&ll), sizeof(unsigned long long));
	return ll;
}

static unsigned long long ntohll(unsigned long long ll)
{
	if (!is_big_endian())
		reverse_bytes(reinterpret_cast<unsigned char *>(&ll), sizeof(unsigned long long));
	return ll;
}


