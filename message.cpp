#include "message.h"
#include "event_manager.h"

using namespace std;
octet_ostream &operator<<(octet_ostream &oos, head &h){
	oos << h.version << h.command;
	oos.append_data(h.password, 8);
	oos << h.type << h.status << h.src << h.dest << h.sn_high << h.sn_low << h.length;
	return oos;
}

int message_task::execute(){
	socket_event *se = NULL;
	int i = 0;
	//se = find_event_by_fd(get_message());
	se = find_event_by_fd(get_message()->get_socket());
	if(se == NULL){
		LOG_ERROR_VA("get socket_event failed, maybe client close");
		delete (get_message()->get_socket_event()); //delete the se;
		return -1;
	}
	i = task_message->response();
	if(i != 0){
		LOG_ERROR_VA("task execute error");
		delete (get_message()->get_socket_event()); //delete the se;
		return -1;
	}
}

