/*
 *Module:	
 *File:		CSingleton-ThreadSafe.h
 *Created:	2011/8
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

#ifndef __SINGLETON_THREAD_SAFETY_HH
#define __SINGLETON_THREAD_SAFETY_HH
#include<iostream>
#include "Lock.h"
using namespace std;

#ifndef MEMORY_BARRIER
#define MEMORY_BARRIER() 0
#endif

template<typename T>
class CSingleton_Ts
{
public:
	static T* Instance()
	{
		int &m_flag = GetFlag();
		CThreadMutex *&m_pThMutex = GetMutex();
		T *&m_pInstance = GetT();

		if(m_flag==0)
		{
			m_pThMutex->WriteLock();
			if(m_pInstance==NULL)
			{
				m_pInstance=new T;
				m_flag=1;
				MEMORY_BARRIER();
			}
			m_pThMutex->UnLock();
		}
		
		return m_pInstance;
	}

	static void DestroyInstance()
	{ 
	
		int &m_flag = GetFlag();
		CThreadMutex *&m_pThMutex = GetMutex();
		T *&m_pInstance = GetT();

		if(m_flag!=0)
		{
			m_pThMutex->WriteLock();
			if(m_pInstance!=NULL)
			{
				delete m_pInstance;
				m_pInstance = NULL;
				m_flag=0;
				MEMORY_BARRIER();
			}
			m_pThMutex->UnLock();
		}
	}
protected:
	CSingleton_Ts()
	{

	}
	virtual ~CSingleton_Ts()
	{
		CThreadMutex *m_pThMutex = GetMutex();
		if(m_pThMutex!=NULL)
		{
			delete m_pThMutex;
			//m_pThMutex=NULL;
		}
	}
private:
	CSingleton_Ts(const CSingleton_Ts& source)
	{

	}
private:
	static T *&GetT()
	{
		static T* m_pInstance = NULL;
		return m_pInstance;
	}
	static int &GetFlag()
	{
		static int m_flag = 0;
		return m_flag;
	}
	static CThreadMutex *&GetMutex()
	{
		static CThreadMutex* m_pThMutex = new CThreadMutex;
		return m_pThMutex;
	}
};

#endif

