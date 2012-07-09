#include "front_message.h"

int front_message::response()
{
	int i = 0;
	LOG_DEBUG_VA("front_message response id: %d, size: %d", id, size);

	front_response_message msg;
	msg.set_id(id + 1);
	msg.set_size(size + 1);

	LOG_DEBUG_VA("send response by %d",get_socket());
	i = send_msg(get_socket(), msg, 2000);
	if(i == -1){
		LOG_ERROR_VA("front_message send result error");
		return -1;
	}else{
		LOG_DEBUG_VA("front_message response fd: %d", get_socket());
		return 0;
	}
}

int front_response_message::response()
{
	cout << "response" << endl;
	return 0;
}

