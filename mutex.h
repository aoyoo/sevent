#pragma once
#include <pthread.h>
#include <iostream>
#include "Log.h"
using namespace std;
class mutex
{
	pthread_mutex_t _mutex;
public:
	mutex() {pthread_mutex_init(&_mutex, NULL);}
	~mutex() {pthread_mutex_unlock(&_mutex);}
	void lock()
	{
		//LOG_DEBUG_VA("mutex lock");
		pthread_mutex_lock(&_mutex);
	}
	void unlock()
	{
		//LOG_DEBUG_VA("mutex unlock");
		pthread_mutex_unlock(&_mutex);
	}
};
