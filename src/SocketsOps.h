#ifndef SEVENT_SOCKETSOPS_H
#define SEVENT_SOCKETSOPS_H

#include <arpa/inet.h>

namespace sevent
{
namespace sockets
{

///
/// Creates a non-blocking socket file descriptor,
/// abort if any error.
int createNonblockingOrDie();

int  connect(int sockfd, const struct sockaddr_in& addr);
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
void listenOrDie(int sockfd);
int  accept(int sockfd, struct sockaddr_in* addr);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readTimeout(int sockfd, void *buf, size_t count); //add read timeout
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);
ssize_t writeTimeout(int sockfd, const void *buf, size_t count); //add write timeout
void close(int sockfd);
void shutdownWrite(int sockfd);

void toIpPort(char* buf, size_t size,
              const struct sockaddr_in& addr);
void toIp(char* buf, size_t size,
          const struct sockaddr_in& addr);
void fromIpPort(const char* ip, uint16_t port,
                  struct sockaddr_in* addr);

int getSocketError(int sockfd);

struct sockaddr_in getLocalAddr(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);

}
}

#endif


