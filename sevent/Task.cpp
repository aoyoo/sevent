#include "Connection.h"
#include "Message.h"
#include "Task.h"
#include "Logger.h"

#include <sys/time.h>

namespace sevent{

Task::Task(ConnectionPtr c, MessagePtr msg, const MessageCallback& f)
	:connection_(c), message_(msg), taskFunc_(f){
	//time_ = new Timestamp;
}

Task::~Task(){
	LOG_DEBUG("Task::~Task");
}

int Task::execute(){
	//LOG_INFO("Task " << (long)this << " with Connection " << (long)(connection_) << " start execute");
	struct timeval taskStartTime;
	gettimeofday(&taskStartTime, NULL);
	int ret = taskFunc_(connection_, message_, time_);
	struct timeval taskEndTime;
	gettimeofday(&taskEndTime, NULL);
	//LOG_INFO("Task " << (long)this << " with Connect: " << (long)(_connect) << " execute time: " << (taskEndTime.tv_sec - taskStartTime.tv_sec)*1000000 + taskEndTime.tv_usec - taskStartTime.tv_usec << " us");
	return ret;
}

}

