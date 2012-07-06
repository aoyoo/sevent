/*
 *Module:	log
 *File:		CLogManager.h
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
 
#ifndef __CLOG_MANAGER_HH
#define __CLOG_MANAGER_HH
#include "CSingleton-ThreadSafe.h"
#include "CLogger.h"
class CLogManager:public CSingleton_Ts<CLogManager>
{
public:
	CLogManager()
	{
	}
	~CLogManager()
	{
	}

	void AddHandler(SharedHandlerPtr handler_ptr)
	{
		m_Logger.AddHandler(handler_ptr);
	}

	void RemoveHandler(SharedHandlerPtr handler_ptr)
	{
		m_Logger.RemoveHandler(handler_ptr);
	}
	
	CLogger& GetLogger()
	{
		return m_Logger;
	}
private:
	CLogger m_Logger;
};
#endif

