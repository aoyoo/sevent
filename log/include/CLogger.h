/*
 *Module:	log
 *File:		CLogger.h
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
#ifndef __CLOGGER_HH
#define __CLOGGER_HH
#include "CommonOperation.h"
#include "CSingleton-ThreadSafe.h"
#include "CLogRecord.h"
#include "CHandler.h"
#include "Lock.h"

class CLogger
{
public:
	CLogger();
	
	CLogger(const CLogger& rhs);
	CLogger& operator=(const CLogger& rhs);
	
	virtual ~CLogger();
	
	void AddHandler(SharedHandlerPtr handler_ptr);
	void RemoveHandler(SharedHandlerPtr handler_ptr);

	void Log(CLogRecord record);
	
private:
	vector<SharedHandlerPtr> m_vHandlers;
	CThreadMutex m_Mutex;
};
#endif

