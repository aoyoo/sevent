#include "front_server.h"
#include "front_message.h"

int front_server::setup()
{
	register_message_builder(front_message::msg_id, new message_builder<front_message>());
	return 0;
}

