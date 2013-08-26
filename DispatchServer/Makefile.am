AM_CPPFLAGS=-I${top_srcdir}/include \
	-I${top_srcdir}/sevent/ \
	-I${top_srcdir}/DispatchServer/ 

LDADD= $(top_srcdir)/lib/liblog4cxx.a \
	$(top_srcdir)/lib/libapr-1.a \
	$(top_srcdir)/lib/libaprutil-1.a \
	$(top_srcdir)/lib/libprotobuf.a \
	$(top_srcdir)/lib/libxml2.a \
	$(top_srcdir)/sevent/libsevent.a 

AM_LDFLAGS=-lpthread -lrt -lexpat -g -Wall 

source_list= DispatchServer.cpp Tunnel.cpp
dispatchServer_list=main.cpp

bin_PROGRAMS = dispatchServer

dispatchServer_SOURCES = ${source_list} ${dispatchServer_list}


