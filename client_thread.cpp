#include <signal.h>
#include <fcntl.h>
#include <iostream>
#include "front_message.h"
#include "buffer.h"
#include "message.h"
#include "Log.h"

#define THREAD_NUM 50
#define CALC_NUM   10

address addr("10.1.0.11", 41234);
using namespace std;

int cor_num , err_num ;
int g_id, g_size;
timeval beg;
void *send_thread(void *ptr)
{
	timeval end;
	int ret;
	int sockfd;
	struct sockaddr_in dest_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <= 0) 
	{
		LOG_ERROR_VA("socket error");
		pthread_exit(0);
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(41234);
	dest_addr.sin_addr.s_addr = inet_addr("10.1.0.11");
	bzero(&(dest_addr.sin_zero), 8);

	ret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
	if (ret == -1)
	{
		LOG_ERROR_VA("connect error");
		pthread_exit(0);
	}

	for(int i = 0; i < CALC_NUM ;i++)
	{
		front_message req;
		front_response_message res;
		srand(time(0));
		//int id = rand()%250;
		//int size = rand()%128;
		int id = __sync_add_and_fetch(&g_id, 1);
		int size = __sync_add_and_fetch(&g_size, 1);
		LOG_DEBUG_VA("req id: %d; size: %d socket: %d", id, size, sockfd);
		req.set_id(id);
		req.set_size(size);
		
		ret = send_msg(sockfd, req, 2000);
		ret = recv_msg(sockfd, res, 2000);
		LOG_DEBUG_VA("res id: %d; size: %d socket: %d", res.get_id(), res.get_size(), sockfd);
		//res.show();
	}
	//close(sockfd);
}

void sig_stop(int signo)
{
	timeval stop_end;
	gettimeofday(&stop_end, 0);
	cout << "stop servers in SIGINT"<< endl;
	exit(0);
}

int main(int argc, char **argv)
{
	timeval main_end;
	cor_num = 0;
	err_num = 0;
	g_id = 1;
	g_size = 1;
	gettimeofday(&beg, 0);

	init_logger("./logc", "TRACE");

	signal(SIGINT, sig_stop);

	pthread_t tid[THREAD_NUM];

	for(int i = 0;i < THREAD_NUM;i++)
	{
		pthread_create(&tid[i], NULL, send_thread, NULL);
	}
	
	for(int i = 0;i < THREAD_NUM;i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	return 0;
}

