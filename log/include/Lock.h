/*
 *Module:	
 *File:		Lock.h
 *Created:	2011/9
  *Authors:	focus <focus.su@taotaosou.com>
 *
 *Copyright (c) 2011- www.taotaosou.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 */ 

#ifndef __LOCK_HH
#define __LOCK_HH
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include<error.h>
#include<stdint.h>

class CThreadMutex
{
public:
	CThreadMutex()
	{
		//pthread_mutexattr_t attr;
		//pthread_mutexattr_init(&attr);
		//if use this the pthread_cond_wait can't be block...
		//pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
		
		//if(pthread_mutex_init(&m_mMutex,&attr)!=0)
		if(pthread_mutex_init(&m_mMutex,NULL)!=0)
		{
			perror("pthread_mutex_init:");
		}
		//pthread_mutexattr_destroy(&attr);
	}
	~CThreadMutex()
	{
		pthread_mutex_destroy(&m_mMutex);
	}
	
	int ReadLock()
	{
		return pthread_mutex_lock(&m_mMutex);
	}

	int WriteLock()
	{
		return pthread_mutex_lock(&m_mMutex);
	}

	int TryReadLock()
	{
		return pthread_mutex_trylock(&m_mMutex);
	}

	int TryWriteLock()
	{
		return pthread_mutex_trylock(&m_mMutex);
	}

	int UnLock()
	{
		return pthread_mutex_unlock(&m_mMutex);
	}

protected:
	pthread_mutex_t m_mMutex;
};

enum ELockMode
{
	DEFAULT_PRIORITY,
	WRITE_PRIORITY,
	READ_PRIORITY
};

class CRWLock
{
public:
	CRWLock(ELockMode mode=READ_PRIORITY)
	{
		pthread_rwlockattr_t attr;
		pthread_rwlockattr_init(&attr);
		pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
		if(mode==READ_PRIORITY)
		{
			pthread_rwlockattr_setkind_np(&attr,PTHREAD_RWLOCK_PREFER_READER_NP);
		}else if(mode==WRITE_PRIORITY){
			pthread_rwlockattr_setkind_np(&attr,PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);			
		}
		pthread_rwlock_init(&m_RWLock,&attr);
		
		pthread_rwlockattr_destroy(&attr);
	}
	~CRWLock()
	{
		pthread_rwlock_destroy(&m_RWLock);
	}
	int ReadLock()
	{
		return pthread_rwlock_rdlock(&m_RWLock);
	}

	int WriteLock()
	{
		return pthread_rwlock_wrlock(&m_RWLock);
	}

	int TryReadLock()
	{
		return pthread_rwlock_tryrdlock(&m_RWLock);
	}

	int TryWriteLock()
	{
		return pthread_rwlock_trywrlock(&m_RWLock);
	}

	int UnLock()
	{
		return pthread_rwlock_unlock(&m_RWLock);
	}
protected:

private:
	pthread_rwlock_t m_RWLock;
};

class CThreadCond:public CThreadMutex
{
public:
	CThreadCond()
	{
		if(pthread_cond_init(&m_cond,NULL)!=0)
			perror("pthread_cond_init");
	}
	~CThreadCond()
	{
		pthread_cond_destroy(&m_cond);
	}

	int Wait()
	{
		int n;
		n=pthread_cond_wait(&m_cond,&m_mMutex);
		return n;
	}

	int TimeWait(int millsec=0)
	{
		if(0==millsec)
			return Wait();

		struct timeval cur_time;
		struct timespec abs_time;
		gettimeofday(&cur_time,NULL);

		int64_t time_us=static_cast<int64_t>(cur_time.tv_sec)
					*1000000
					+static_cast<int64_t>(cur_time.tv_usec)
					+static_cast<int64_t>(millsec)
					*1000;
		abs_time.tv_sec=static_cast<int>(time_us/1000000);
		abs_time.tv_nsec=static_cast<int>(time_us%1000000)*1000;
		return pthread_cond_timedwait(&m_cond,&m_mMutex,&abs_time);
	}

	int Signal()
	{
		return pthread_cond_signal(&m_cond);
	}

	int BroadCast()
	{
		return pthread_cond_broadcast(&m_cond);
	}
private:
	pthread_cond_t m_cond;
};
#endif

