#include <fcntl.h>
#include <iostream>
#include "front_message.h"
#include "buffer.h"
#include "message.h"
using namespace std;

int main(int argc, char **argv)
{
	timeval beg, end;
	gettimeofday(&beg, 0);

	int sockfd;
	struct sockaddr_in dest_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
	{
		perror("socket()");
		exit(1);
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(41234);
	dest_addr.sin_addr.s_addr = inet_addr("10.1.0.11");
	bzero(&(dest_addr.sin_zero), 8);

	int ret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
	if (ret == -1)
	{
		perror("connect()");
		exit(1);
	}

	char msg[52] = "0123456789";
	int len = strlen(msg);
	//int sent_bytes = send(sockfd, &msg, 52 , 0);

	front_message req;
	front_response_message res;
	srand(time(0));
	int id = rand()%250;
	int size = rand()%128;
	cout << "req id: " << id <<"; size: " << size << endl;
	req.set_id(id);
	req.set_size(size);
/*	
	buffer buf1,buf2;
	req.pack(buf1);

	buf2 = buf1;
	char temp_c[52];
	buf2.drain_data(temp_c, 52);
	
	//req.unpack(buf2);

	octet_istream ois(buf2);
	int i1,i2,i3;
	ois >> i1 >> i2 >> i3;
*/
	ret = send_msg(sockfd, req, 4000);
	ret = recv_msg(sockfd, res, 4000);
	cout << "res id:" << res.get_id() << "; size: " << res.get_size() << endl;
	//res.show();
	gettimeofday(&end, 0);
	cout << "vs serve time: " << end.tv_usec - beg.tv_usec + (end.tv_sec - beg.tv_sec) * 1000000 << " us" << endl; 
	/* 关闭连接*/
	close(sockfd);
	return 0;
}

