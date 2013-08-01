#ifndef SEVENT_TASK_H_
#define SEVENT_TASK_H_

#include "Callbacks.h"
#include "Timestamp.h"

#include <string>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace sevent{

class Task : public boost::enable_shared_from_this<Task>
{
public:
	Task(ConnectionPtr, MessagePtr, const TaskCallback & f);
	~Task();
	
	int execute();
	
	//Connection* getConnection(){return _connection;}
	//Message* getMessage(){return _message;}
	//Timestamp getCreateTime(){return _time;}
	
private:
	ConnectionPtr connection_;
	MessagePtr message_;
	
	Timestamp time_;
	
	TaskCallback taskFunc_;
};


}

#endif


