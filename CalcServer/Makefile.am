AM_CPPFLAGS=-I${top_srcdir}/include \
		-I${top_srcdir}/sevent/ 

LDADD= $(top_srcdir)/lib/liblog4cxx.a \
	$(top_srcdir)/lib/libapr-1.a \
	$(top_srcdir)/lib/libaprutil-1.a \
	$(top_srcdir)/lib/libprotobuf.a \
	$(top_srcdir)/sevent/libsevent.a

AM_LDFLAGS=-lpthread -lrt -lexpat -g -Wall

source_list= CalcServer.cpp 

bin_PROGRAMS = calcServer

calcServer_SOURCES = ${source_list} ${notifyserver_list}

