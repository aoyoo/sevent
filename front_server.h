#pragma once
#include "socket.h"
#include "server.h"

class front_server : public message_server
{
	std::vector<address> extract_nodes;
public:
	void add_extract_node(const address &addr) { extract_nodes.push_back(addr); }
	int setup();
};

