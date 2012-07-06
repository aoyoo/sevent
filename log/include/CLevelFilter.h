/*
 *Module:	log
 *File:		CLevelFilter.h
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
 
#ifndef __CLEVEL_FILTER_HH
#define __CLEVEL_FILTER_HH
#include "CFilter.h"
#include "CLevel.h"
#include "CLogRecord.h"

class CLevelFilter:public CFilter
{
public:
	CLevelFilter()
	{
	}
	virtual ~CLevelFilter()
	{
	}

	virtual bool IsLogable(CLogRecord record)
	{
		return record.GetLevel().Greater(GetLevel());
	}
	
	CLevel GetLevel()
	{
		return m_Level;
	}
	
	void SetLevel(CLevel level)
	{
		m_Level=level;
	}
private:
	CLevel m_Level;
};
#endif

