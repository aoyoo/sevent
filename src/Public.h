#ifndef SEVENT_PUBLIC_H_
#define SEVENT_PUBLIC_H_

#include <boost/shared_ptr.hpp>

#if __BYTE_ORDER == __BIG_ENDIAN
	#define htonll(x)       (x)
	#define ntohll(x)       (x)
#else
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		#define htonll(x)     __bswap_64 (x)
		#define ntohll(x)     __bswap_64 (x)
	#endif
#endif

namespace sevent{

typedef enum{
	NOTUSE = 0,
	READY,
	BUSY,
	DEAD
}ConnectStatus;

const int IPADDRESSMAXLEN = 64;
const int IOBUFSIZE = 16384;
const int EPOLLNUMS = 16384;
const int MAXCONNECTNUM = 16384;


}


#endif

