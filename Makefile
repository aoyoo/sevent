target = sevent
srcs = front_message.cpp front_server.cpp buffer.cpp message.cpp event_manager.cpp thread.cpp task.cpp server.cpp socket.cpp socket_event.cpp main.cpp safe_time.cpp 
libs = log/lib/liblog.a
subdirs = log
INCLUDE = -Ilog/include/  -I./
LIBRARY = -lpthread -lssl -lrt
CPPFLAGS += -g #-fpic 

objs=$(patsubst %.cpp, %.o, $(srcs))

all = $(target)

$(target) : $(objs)
	for dir in $(subdirs); do make -C $$dir ; done
	g++ $(CPPFLAGS) $(INCLUDE) -o $(target) $^ $(libs) $(LIBRARY)

%.o : %.cpp 
	g++ $(INCLUDE) $(CPPFLAGS) $< -c -o $@

.PHONY : clean
clean:
	rm -f *.o
	rm -f $(target)
#for dir in $(subdirs); do make -C $$dir clean; done

