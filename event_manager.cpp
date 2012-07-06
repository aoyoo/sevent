#include "event_manager.h"
#include "Log.h"
#include "server.h"
#include "socket_event.h"
/*
setnonblocking - 设置句柄为非阻塞方式
*/
int event_manager::setnonblocking(int sockfd)
{
	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1) {
		return -1;
	}
	return 0;
}

void event_manager::start_event_loop()
{
	LOG_INFO_VA("start_event_loop");
	epoll_event evs[2048];
	epoll_event ev;
	address addr;
	socklen_t length = addr.length();
	int n = 0;

	char temp_buf[128];
	while(!loop_stop){
		n = epoll_wait(epfd, evs, 2048, -1);
		if(n < 0 && errno == EINTR){
			LOG_ERROR_VA("epoll wait EINTR");
			//what EINTR means
			continue;
		}
		if (n < 0){
			LOG_ERROR_VA("epoll wait < 0");
		}
		if (n == 0){
			LOG_ERROR_VA("epoll wait == 0");
		}
		
		LOG_DEBUG_VA("epoll wait get %d events", n);
		for (int i = 0; i != n; i++) {
			if(evs[i].data.fd == get_listener_socket()){
				LOG_DEBUG_VA("epoll newin");
				while(1){
					int new_sock = accept(get_listener_socket(), &addr, &length);
					if (new_sock <= 0) {
						LOG_ERROR_VA("new sock fd error :%d",new_sock);
						break;
					}
					
					setnonblocking(new_sock);
					ev.events = EPOLLIN;
					ev.data.fd = new_sock;
					if (epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &ev) < 0) {
						LOG_ERROR_VA("add socket %d to epoll failed",new_sock);
						continue;
					}
					LOG_DEBUG_VA("add epoll fd: %d",new_sock);
					//LOG_DEBUG_VA("regeister fd: %d ,socket_event %x",new_se->get_socket_fd(), new_se);
				}
			}else if(evs[i].events & EPOLLIN) {
				int sock = evs[i].data.fd;
				LOG_DEBUG_VA("epoll in: %d", evs[i].data.fd);
				int buf_num;
				char *buf;
				
				if(get_server()->get_memory_block(buf_num, buf) == -1){
					LOG_ERROR_VA("get memory block for head failed");
					close(sock);
					continue;
				}else{
					int ret = tcp_read_ms(sock, buf, 128, tcp_read_timeout);
					if(ret == 0){
						LOG_ERROR_VA("client close");
						close(sock);
						continue;
					}
					if (ret < 0){
						LOG_ERROR_VA("read error: %d",ret);
						close(sock);
						continue;
						//return -1;
					}

					task *tsk = new task(sock);
					tsk->set_buf(buf);
					LOG_DEBUG_VA("new task by fd: %d; task: %x, buf",sock, tsk, buf);
					tsk->start(get_server()->get_thread_pool());
				}
			}else if (evs[i].events & EPOLLOUT) {
				LOG_ERROR_VA("epoll write");
			}else if (evs[i].events & EPOLLERR) {
				LOG_ERROR_VA("epoll error :%d", evs[i].data.fd);
			}else{
				//some else EPOLL 
				LOG_ERROR_VA("epoll unknown");
			}
			
		}
	}
}

event_manager::event_manager()
{
	loop_stop = false;
	listen = NULL;
	epfd = epoll_create(2048);
	LOG_DEBUG_VA("event_manager epoll fd: %d",epfd);
}

