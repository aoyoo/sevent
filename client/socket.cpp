#include "system.h"
#include "socket.h"
#include <iostream>
using namespace std;

static int tcp_poll(pollfd *fds, int nfds, int timeout)
{
	int val;
again:
	val = poll(fds, nfds, timeout);
	if (val < 0) {
		if (errno == EINTR) {
			goto again;
		}
	}

	return val;
}

static int tcp_read(int fd, void *ptr, size_t nbytes, struct timeval *tv)
{
	pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	int ret = tcp_poll(fds, 1, tv->tv_sec * 1000 + tv->tv_usec / 1000);
	if (ret > 0 && (fds[0].revents & POLLIN))
		return read(fd, ptr, nbytes);
	return -1;
}

static ssize_t tcp_write(int fd, void *ptr, size_t nbytes, struct timeval *tv)
{
	pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLOUT;

	int ret = tcp_poll(fds, 1, tv->tv_sec * 1000 + tv->tv_usec / 1000);

	if (ret > 0 && (fds[0].revents & POLLOUT))
		return write(fd, ptr, nbytes);	
	return -1;
}

int tcp_read_ms(int fd, void *ptr1, int nbytes, int msecs)
{
	ssize_t  nread;
	int 	 sockflag;
	size_t   nleft = nbytes;
	char *ptr = (char *)ptr1;
	struct  timeval tv;
	struct	timeval *tp;

	if(msecs >= 0)
	{
        tv.tv_sec = msecs / 1000;
        tv.tv_usec = (msecs % 1000) * 1000;
		tp = &tv;
	}
	else
	{
		tp = NULL;
	}

	sockflag = fcntl(fd, F_GETFL, 0);
	if(sockflag < 0)
	{
	        return -1;
	}
	
	if(!(sockflag & O_NONBLOCK))
	{
		if(fcntl(fd, F_SETFL, sockflag | O_NONBLOCK) < 0)
		{
		        return -1;
		}
	}

	while(nleft > 0)
	{
	  again:
		nread = tcp_read(fd, ptr, nleft, tp);
		if(nread < 0)
		{
			if((nread == -1) && (errno == EINTR))
			{
				goto again;
			}
			else if(nread == -2)
			{
				errno = ETIMEDOUT;
				return -1;
			}
			else if(nread == -3)
			{
				errno = EIO;
				return -1;
			}
			else
			{
				return -1;
			}
		} 
		else if(nread == 0)
		{
			break;
		}
		else 
		{
			ptr += nread;
			nleft -= nread;
		}
	}

	return (ssize_t)(nbytes - nleft);
}

int tcp_write_ms(int fd, const void *data, int length, int msecs)
{
	int  sockflag;
	ssize_t  n;
	size_t nleft = length;
	char *ptr = (char *)data;
	struct  timeval tv;
	struct  timeval *tp;

	if(msecs >= 0)
	{
		tv.tv_sec = msecs / 1000;
		tv.tv_usec = (msecs % 1000) * 1000;
		tp = &tv;
	}
	else
	{
		tp = NULL;
	}

	sockflag = fcntl(fd, F_GETFL, 0);
	if(sockflag < 0)
	{
		return -1;
	}

	if(!(sockflag & O_NONBLOCK))
	{
		if(fcntl(fd, F_SETFL, sockflag | O_NONBLOCK) < 0)
		{
			return -1;
		}
	}

	while(nleft > 0)
	{
		n = tcp_write(fd, ptr, (size_t)nleft, tp);
		if((n == -1) && (errno == EINTR))
		{
			continue;
		}
		if(n <= 0)
		{
			if(n == -2)
			{
				errno = ETIMEDOUT;
			}

			if(!(sockflag & O_NONBLOCK))
			{
				if(fcntl(fd, F_SETFL, sockflag) < 0)
				{
					return -1;
				}
			}

			return -1;
		}
		nleft -= n;
		ptr += n;
	}
	if(!(sockflag & O_NONBLOCK))
	{
		if(fcntl(fd, F_SETFL, sockflag) < 0)
		{
			return -1;
		}
	}

	return (ssize_t)(length - nleft);
}

int check_alive(int sockno)
{
	if (sockno <= 0)
		return 0;
	
	pollfd fds[1];
	fds[0].fd = sockno;
	fds[0].events = POLLOUT;
	
	int ret = poll(fds, 1, 0);
	if(ret < 0) {
		return ret;
	}
	if(fds[0].revents & POLLOUT)
		return 1;
	return 0;
}

