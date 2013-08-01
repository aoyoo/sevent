target = client
srcs = client.cpp socket.cpp
libs = 
subdirs= 
INCLUDE= 
LIBRARY= -lpthread
CPPFLAGS=-g 
all : $(target)
objs=$(patsubst %.cpp, %.o, $(srcs))

$(target) : $(objs) 
	for dir in $(subdirs); do make -C $$dir ; done
	g++ $(CPPFLAGS) $(INCLUDE) -o $(target) $^ $(libs) $(LIBRARY) 
%.o : %.cpp
	g++ $(INCLUDE) $(CPPFLAGS) $< -c -o $@

.PHONY : clean
clean:
	for dir in $(subdirs); do make -C $$dir clean; done
	rm -f $(objs)
	rm -f $(target)
