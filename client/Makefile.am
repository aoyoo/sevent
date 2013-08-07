AM_CPPFLAGS=-I${top_srcdir}/include \
		-I${top_srcdir}/src/ 

LDADD= $(top_srcdir)/lib/liblog4cxx.a \
	$(top_srcdir)/lib/libapr-1.a \
	$(top_srcdir)/lib/libaprutil-1.a \
	$(top_srcdir)/lib/libprotobuf.a 

AM_LDFLAGS=-lpthread -lrt -lexpat -g -Wall

source_list= client.cpp socket.cpp

bin_PROGRAMS=client

client_SOURCES=${source_list}

