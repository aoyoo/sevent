/*
 *Module:	log
 *File:		CLogRecord.cpp
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
#include "CLogRecord.h"
#include "CLevel.h"

CLogRecord::CLogRecord()
	:m_Message(""),
	m_FileName(""),
	m_FileLine(0),
	m_TimeStamp(Time::Now())
{
	char buf[64]={0};
	sprintf(buf,"%ld",pthread_self());
	m_ThreadID=std::string(buf);
}

CLogRecord::CLogRecord(CLevel level)
	:m_Message(""),
	m_FileName(""),
	m_FileLine(0),
	m_Level(level),
	m_TimeStamp(Time::Now())
{
	char buf[64]={0};
	sprintf(buf,"%ld",pthread_self());
	m_ThreadID=std::string(buf);
}

CLogRecord::CLogRecord(const std::string& message,
		const std::string& filename,const int &line,CLevel level)
	:m_Message(message),
	m_FileName(filename),
	m_FileLine(line),
	m_Level(level),
	m_TimeStamp(Time::Now())
{
	char buf[64]={0};
	sprintf(buf,"%ld",pthread_self());
	m_ThreadID=std::string(buf);
}

CLogRecord::CLogRecord(const std::string& message,
		const std::string& filename,const int &line,
		const std::string& threadid,
		const Time time,
		CLevel level)
	:m_Message(message),
	m_FileName(filename),
	m_FileLine(line),
	m_ThreadID(threadid),
	m_TimeStamp(time),
	m_Level(level)
{
}

CLogRecord::CLogRecord(CLogRecord& _rhs)
	:m_Message(_rhs.GetMessage()),
	m_FileName(_rhs.GetFileName()),
	m_FileLine(_rhs.GetFileLine()),
	m_Level(_rhs.GetLevel()),
	m_ThreadID(_rhs.GetThreadId()),
	m_TimeStamp(_rhs.GetTimeStamp())
{
}

CLogRecord::~CLogRecord()
{}

void CLogRecord::SetLevel(CLevel level)
{
	m_Level=level;
}

void CLogRecord::SetMessage(const std::string& message)
{
	m_Message=message;
}

const std::string& CLogRecord::GetThreadId()const
{
	return m_ThreadID;
}

CLevel& CLogRecord::GetLevel()
{
	return m_Level;
}

const std::string& CLogRecord::GetFileName( )const
{
	return m_FileName;
}

const int& CLogRecord::GetFileLine()const
{
	return m_FileLine;
}

const Time& CLogRecord::GetTimeStamp()const
{
	return m_TimeStamp;
}

const std::string& CLogRecord::GetMessage()const
{
	return m_Message;
}

