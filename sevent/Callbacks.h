#ifndef SEVENT_CALLBACK_H_
#define SEVENT_CALLBACK_H_

#include "Timestamp.h"
#include "InetAddress.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace sevent{


class Connection;
class Message;
class Task;
typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef boost::shared_ptr<Message> MessagePtr;
typedef boost::shared_ptr<Task> TaskPtr;

//in Connection
typedef boost::function<int (const ConnectionPtr&,
                              const MessagePtr&,
                              Timestamp)> MessageCallback;
typedef boost::function<void (const ConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const ConnectionPtr&)> CloseCallback;
typedef boost::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

//in Channel
typedef boost::function<void()> EventCallback;
typedef boost::function<void(Timestamp)> ReadEventCallback;

//for EventLoopThread
typedef boost::function<void()> ThreadInitCallback;

}


#endif


