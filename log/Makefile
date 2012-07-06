CPPFLAGS = -g -I./include  
src_dir = ./src
srcs = $(wildcard $(src_dir)/*.cpp)
objs = $(patsubst %.cpp, %.o, $(srcs))

all : liblog.a

liblog.a : $(objs)
	ar crv  lib/$@ $^

%.o : %.cpp
	g++ $(CPPFLAGS) $< -c -o $@

.PHONY : test clean

test :
	g++ $(CPPFLAGS) -Llib -lrt -o testlog log-test.cpp  -llog

clean:
	rm -f $(objs)
	rm -f testlog
	rm lib/liblog.a
	
