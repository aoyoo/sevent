AM_CPPFLAGS=-I${top_srcdir}/include \
		-I${top_srcdir}/src/ 

LDADD= $(top_srcdir)/lib/liblog4cxx.a \
	$(top_srcdir)/lib/libapr-1.a \
	$(top_srcdir)/lib/libaprutil-1.a \
	$(top_srcdir)/lib/libprotobuf.a 

AM_LDFLAGS=-lpthread -lrt -lexpat -g -Wall

source_list= Buffer.cpp Socket.cpp SocketsOps.cpp Thread.cpp ThreadPool.cpp Timestamp.cpp\
	 Acceptor.cpp Channel.cpp Connection.cpp EventLoop.cpp EventLoopThread.cpp EventLoopThreadPool.cpp InetAddress.cpp Message.cpp Poller.cpp Server.cpp Task.cpp Logger.cpp

exec_source_list = CalcServer.cpp

noinst_LIBRARIES=libsevent.a
bin_PROGRAMS=calcserver

libsevent_a_SOURCES=${source_list}
calcserver_SOURCES=${source_list} ${exec_source_list}

