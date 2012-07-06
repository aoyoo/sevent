/*
 *Module:	log
 *File:		CLogger.cpp
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
 
#include "CLogger.h"

CLogger::CLogger()
{
}

CLogger::CLogger(const CLogger& rhs)
	:m_vHandlers(rhs.m_vHandlers)
{
}

CLogger& CLogger::operator=(const CLogger& rhs)
{
	m_vHandlers=rhs.m_vHandlers;
	return *this;
}

CLogger::~CLogger()
{
	m_Mutex.WriteLock();
	m_vHandlers.clear();
	m_Mutex.UnLock();
}

void CLogger::AddHandler(SharedHandlerPtr handler_ptr)
{
	vector<SharedHandlerPtr>::iterator pIter;

	if(handler_ptr!=NULL)
	{
		m_Mutex.WriteLock();
		pIter=std::find(m_vHandlers.begin(),m_vHandlers.end(),handler_ptr);

		if(pIter==m_vHandlers.end())
		{
			m_vHandlers.push_back(handler_ptr);
		}else{
			DEBUGMSG("Alread in handler list...\n");
		}
		m_Mutex.UnLock();
	}
	
	return ;
}

void CLogger::RemoveHandler(SharedHandlerPtr handler_ptr)
{
	vector<SharedHandlerPtr>::iterator pIter;
	if(handler_ptr!=NULL)
	{
		m_Mutex.WriteLock();
		pIter=find(m_vHandlers.begin(),m_vHandlers.end(),handler_ptr);
		if(pIter!=m_vHandlers.end())
		{
			m_vHandlers.erase(pIter);
		}
		m_Mutex.UnLock();
	}
	return ;
}

void CLogger::Log(CLogRecord record)
{
	m_Mutex.WriteLock();
	for(int i=0;i<m_vHandlers.size();i++)
	{
		m_vHandlers[i]->Publish(record);
	}
	m_Mutex.UnLock();
	return ;
}

