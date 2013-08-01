#ifndef SEVENT_THREAD_H
#define SEVENT_THREAD_H

#include <string>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <sys/syscall.h>

namespace sevent
{

pid_t gettid();

using std::string;
class ThreadData
{
public:
	typedef boost::function<void ()> ThreadFunc;
	ThreadFunc func_;
	string name_;
	pid_t *tid_;
	
	ThreadData(const ThreadFunc& func,
	           const string& name,
	           pid_t* tid)
	  : func_(func),
	    name_(name),
	    tid_(tid)
	{ }

	void runInThread();
};

class Thread : boost::noncopyable
{
public:
	typedef sevent::ThreadData::ThreadFunc ThreadFunc;

  explicit Thread(const ThreadFunc&, const string& name = string());
  ~Thread();

  void start();
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t getTid() const { return tid_; }

  const string& name() const { return name_; }

  //static int numCreated() { return numCreated_.get(); }

private:
  ThreadFunc func_;

  bool       started_;
  bool       joined_;
  pthread_t  pthreadId_;
  pid_t tid_; //FIXME just get tid after pthread_create
  string     name_;

  //static AtomicInt32 numCreated_;
};

}

#endif


