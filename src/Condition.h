#ifndef SEVENT_CONDITION_H
#define SEVENT_CONDITION_H

#include <Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <errno.h>

namespace sevent{

class Condition : boost::noncopyable {
public: 
	explicit Condition(MutexLock & mutex)
	:mutex_(mutex) {
		pthread_cond_init(&pcond_, NULL);
	}
	~Condition() {
		pthread_cond_destroy(&pcond_);
	}
	void wait() {
		MutexLockGuard guard(mutex_);
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}
	
	// returns true if time out, false otherwise.
	bool waitForSeconds(int seconds) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec += seconds;
		MutexLockGuard guard(mutex_);
		return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
	}

	void notify() {
		pthread_cond_signal(&pcond_);
	}

	void notifyAll() {
		pthread_cond_broadcast(&pcond_);
	}

private:
	MutexLock & mutex_;
	pthread_cond_t pcond_;
};

}

#endif

