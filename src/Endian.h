#ifndef SEVENT_ENDIAN_H
#define SEVENT_ENDIAN_H

#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string>
#include <vector>

#if __BYTE_ORDER == __BIG_ENDIAN
	#define htonll(x)       (x)
	#define ntohll(x)       (x)
#else
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define htonll(x)     __bswap_64 (x)
		#define ntohll(x)     __bswap_64 (x)
	#endif
#endif

namespace sevent
{
namespace sockets
{

// the inline assembler code makes type blur,
// so we disable warnings for a while.
#if __GNUC_MINOR__ >= 6
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

inline uint64_t hostToNetwork64(uint64_t host64)
{
  return htonll(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
  return htonl(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
  return htons(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
  return ntohll(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  return ntohl(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
  return ntohs(net16);
}

}

}

#endif


