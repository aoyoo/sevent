#include "message.h"
#include "event_manager.h"

using namespace std;
octet_ostream &operator<<(octet_ostream &oos, head &h){
	oos << h.version << h.command;
	oos.append_data(h.password, 8);
	oos << h.type << h.status << h.src << h.dest << h.sn_high << h.sn_low << h.length;
	return oos;
}

