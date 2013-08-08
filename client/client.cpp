/* sockclnt.c*/
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> /*for struct sockaddr_in*/
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h> 
#include <poll.h>

#include <iostream>

//#include "socket.h" 
using namespace std;

//const int buf_length = 10485760;

int client_port = 3330;
const char *client_ip = "127.0.0.1";

int thread_num = 4;
int socket_num = 8;

void *thread_func(void *ptr){
	cout << "thread_func thread_num " << thread_num << " socket_num " << socket_num << endl;

	int sockfd[socket_num];
	for(int i = 0; i < socket_num; ++i){
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd[i] == -1){
			perror("socket()");
			abort();
		}
		cout << "get sockfd: " << sockfd[i] << endl;
	}
	
	for(int i = 0; i < socket_num; ++i){
		cout << "connect " << sockfd[i] << endl;
		struct sockaddr_in dest_addr;
		bzero(&dest_addr, sizeof(dest_addr));
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(client_port);
		dest_addr.sin_addr.s_addr = inet_addr(client_ip);
		if (connect(sockfd[i], (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0){
			perror("connect");
			abort();
		}
	}
	
	cout << "connect success !" << endl;

	int command = 1;
	int status = 2;
	int src = 3;
	int sn = 4;
	int length = 4;
	int count = 0;
	int readCont = 0;
	
	int ret;
	struct timeval timev;
	while(1){
//	for(int i = 0; i < 1; i++){
		for (int j = 0; j != socket_num; ++j) {
			ret = write(sockfd[j], &command, 4);
			ret = write(sockfd[j], &status, 4);
			ret = write(sockfd[j], &src, 4);
			ret = write(sockfd[j], &sn, 4);
			ret = write(sockfd[j], &length, 4);
			ret = write(sockfd[j], &count, 4);
			//ret = read(sockfd[j], &readCont, 4);
			if(ret < 0){
				perror("io");
				break;
			}
			//if(count != readCont){
			//	perror("!=");
			//	break;
			//}
			__sync_add_and_fetch(&count,1);
		}
	}

//	while(1) sleep(1);

	for (int i = 0; i != socket_num; i++) {
		close(sockfd[i]);
		cout << "close: " << sockfd[i] << endl;
	}
}

int main(int argc, char *argv[])
{
	//signal(SIGPIPE,SIG_IGN);

//	if(argc != 3){
//		cout << "input thread_num & socket_num " << endl;
//		return 0;
//	}
//
//	thread_num = atoi(argv[1]);
//	socket_num = atoi(argv[2]);

	cout << "thread_num: " << thread_num << " socket_num: " << socket_num << endl;

	pthread_t tid[thread_num];

	for(int i = 0; i < thread_num; ++i)
	{
		pthread_create(&tid[i], NULL, thread_func, NULL);
	}
	
	for(int i = 0; i < thread_num; ++i)
	{
		pthread_join(tid[i], NULL);
	}
	
	return 0;
}


