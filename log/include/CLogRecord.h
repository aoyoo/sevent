/*
 *Module:	log
 *File:		CLogRecord.h
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
 
#ifndef __CLOG_RECORD_HH
#define __CLOG_RECORD_HH
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include "CLevel.h"
#include "CTime.h"

class CLogRecord
{
public:
	CLogRecord();
	CLogRecord(CLevel level);
	CLogRecord(const std::string& message,
		const std::string& filename,
		const int &line,
		CLevel level=CLevel(TRACE_LOG_LEVEL));
	
	CLogRecord(const std::string& message,
		const std::string& filename,
		const int &line,
		const std::string& threadid,
		const Time time,
		CLevel level=CLevel(TRACE_LOG_LEVEL));

	CLogRecord(CLogRecord& _rhs);
	
	virtual ~CLogRecord();

	void SetLevel(CLevel level);
	void SetMessage(const std::string& message);

	CLevel& GetLevel();
	const std::string& GetThreadId()const;
	const std::string& GetFileName( )const;
	const int& GetFileLine()const;
	const Time& GetTimeStamp()const;
	const std::string& GetMessage()const;
private:
	std::string m_ThreadID;
	std::string m_FileName;
	int m_FileLine;
	std::string m_Message;
	Time m_TimeStamp;
	
	CLevel m_Level;
};
#endif

