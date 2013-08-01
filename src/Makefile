target=test

srcs=Buffer.cpp Socket.cpp SocketsOps.cpp Thread.cpp ThreadPool.cpp Timestamp.cpp\
	 Acceptor.cpp Channel.cpp Connection.cpp EventLoop.cpp EventLoopThread.cpp EventLoopThreadPool.cpp InetAddress.cpp Message.cpp Poller.cpp Server.cpp Task.cpp \
	 test.cpp Logger.cpp

libs= ../lib/liblog4cxx.a ../lib/libaprutil-1.a ../lib/libapr-1.a \
			../lib/libprotobuf.a

subdirs= 

INCLUDE=-I./ -I../include
LIBRARY=-lpthread -lrt -lexpat
CPPFLAGS=-g -Wall
#CPPFLAGS=-g -Wall -Wextra -Werror -Wconversion -Wshadow

objs=$(patsubst %.cpp, %.o, $(srcs))

all : $(target)

$(target) : $(objs) 
	for dir in $(subdirs); do make -C $$dir ; done
#ar cr $(target) $^ $(libs)
	g++ $(INCLUDE) $(CPPFLAGS) -o $(target) $^ $(libs) $(LIBRARY) 

%.o : %.cpp
	g++ $(INCLUDE) $(CPPFLAGS) $< -c -o $@

.PHONY : clean, clena
clean:
	rm -f $(objs)
	rm -f $(target)
clena:
	rm -f $(objs)
	rm -f $(target)

