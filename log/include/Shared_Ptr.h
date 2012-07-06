/*
 *Module:	
 *File:		Shared_Ptr.h
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
 
#ifndef __SHARED_PTR_HH
#define __SHARED_PTR_HH
#include<iostream>
#include<assert.h>
#include<algorithm>
#include "Lock.h"

class SharedObject
{
public:
	void AddRef() const
	{
		m_Mutex.WriteLock();
		assert(m_count>=0);
		++m_count;
		m_Mutex.UnLock();
	}
	void RemoveRef() const
	{
		bool bDestroy=false;
		m_Mutex.WriteLock();
		assert(m_count>0);
		if(--m_count==0)
		{
			bDestroy=true;
		}
		m_Mutex.UnLock();

		if(bDestroy)
		{
			delete this;
		}
	}
protected:
	SharedObject()
		:m_count(0)
	{
	}
	
	SharedObject(const SharedObject& rhs)
		:m_count(0)
	{
	}
	
	virtual ~SharedObject()
	{
		assert(m_count==0);
	}
	SharedObject& operator=(const SharedObject&)
	{
		return *this;
	}
private:
	mutable CThreadMutex m_Mutex;
	mutable int m_count;
};

template<typename T>
class Shared_Ptr
{
public:
	explicit Shared_Ptr(T* p=NULL)
		:m_Pointee(p)
	{
		AddRef();
	}

	Shared_Ptr(const Shared_Ptr& rhs)
		:m_Pointee(rhs.m_Pointee)
	{
		AddRef();
	}

	~Shared_Ptr()
	{
		RemoveRef();
	}

	bool operator==(const Shared_Ptr& rhs)const
	{
		return (m_Pointee==rhs.m_Pointee);
	}

	bool operator!=(const Shared_Ptr& rhs)const
	{
		return (m_Pointee!=rhs.m_Pointee);
	}

	bool operator==(const T* rhs)const
	{
		return (m_Pointee==rhs);
	}

	bool operator!=(const T* rhs)const
	{
		return (m_Pointee!=rhs);
	}

	T* operator->()const
	{
		assert(m_Pointee);
		return m_Pointee;
	}

	T& operator*()const
	{
		assert(m_Pointee);
		return *m_Pointee;
	}

	Shared_Ptr& operator=(const Shared_Ptr& rhs)
	{
		return this->operator=(rhs.m_Pointee);
	}

	Shared_Ptr& operator=(T* rhs)
	{
                Shared_Ptr<T> (rhs).swap (*this);
                return *this;	
	}

	T* Get()const
	{
		return m_Pointee;
	}

	void swap (Shared_Ptr & other) throw ()
	{
		std::swap (m_Pointee, other.m_Pointee);
	}

	bool operator ! () const
	{
		return ! m_Pointee;
	}
private:
	void AddRef()
	{
		if(m_Pointee!=NULL)
		{
			m_Pointee->AddRef();
		}
		
		return ;
	}

	void RemoveRef()
	{
		if(m_Pointee!=NULL)
		{
			m_Pointee->RemoveRef();
		}
	}
private:
	T* m_Pointee;
};
#endif

