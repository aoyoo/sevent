#ifndef SEVENT_MUTEX_H
#define SEVENT_MUTEX_H

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace sevent {

class MutexLock:boost::noncopyable {
public:
	MutexLock() {
		int ret = pthread_mutex_init(&mutex_, NULL);
		 assert(ret == 0);
		 (void)ret;
	}
	~MutexLock() {
		pthread_mutex_destroy(&mutex_);
	}
	void lock() {
		pthread_mutex_lock(&mutex_);
	}

	void unlock() {
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t *getPthreadMutex() {	/* non-const */
		return &mutex_;
	}

private:
	friend class Condition;
	pthread_mutex_t mutex_;
};

class MutexLockGuard:boost::noncopyable {
public:
	explicit MutexLockGuard(MutexLock & mutex)
	:mutex_(mutex) {
		mutex_.lock();
	}
	~MutexLockGuard() {
		mutex_.unlock();
	}

private:
	MutexLock & mutex_;
};

}

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif


