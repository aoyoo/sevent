/*
 *Module:	
 *File:		ConsoleFormatter.h
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
#ifndef __CONSOLE_FORMATTER_HH
#define __CONSOLE_FORMATTER_HH
#include "CFormatter.h"

class ConsoleFormatter:public CFormatter
{
public:
	ConsoleFormatter()
	{}
	~ConsoleFormatter()
	{}
	
	std::string FormatLog(CLogRecord record)
	{
		std::string log;
		log.append("["+record.GetTimeStamp().ToDateTime()+"] ");
		log.append(record.GetLevel().GetName()+" ");

		if(!record.GetFileName().empty()&&record.GetFileLine()>0)
		{
			log.append(record.GetFileName());
			char buf[1024]={0};
			sprintf(buf,":%d ",record.GetFileLine());
			log.append(buf);
		}

		log.append("thread:"+record.GetThreadId()+" ");
		log.append(record.GetMessage());
		return log;
	}
private:
	ConsoleFormatter(const ConsoleFormatter&);
	ConsoleFormatter& operator=(const ConsoleFormatter&);
};
#endif

