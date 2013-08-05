#include <Thread.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

#include <Logger.h>


pid_t sevent::gettid(){
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

using namespace sevent;
void ThreadData::runInThread(){
	pid_t tid = gettid();
	*tid_ = tid;
	func_();
}

void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}

Thread::Thread(const ThreadFunc& func, const string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n)
{
}

Thread::~Thread()
{
  if (started_ && !joined_)
  {
    pthread_detach(pthreadId_);
  }
}

void Thread::start()
{
  assert(!started_);
  started_ = true;
  ThreadData* data = new ThreadData(func_, name_, &tid_);
  if (pthread_create(&pthreadId_, NULL, startThread, data))
  {
    started_ = false;
    delete data; // or no delete?
    LOG_ERROR("Failed in pthread_create");
  }
}

int Thread::join()
{
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}

