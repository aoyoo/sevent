/*
 *Module:	log
 *File:		CLevel.h
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
#ifndef __CLEVEL_HH
#define __CLEVEL_HH
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include "Shared_Ptr.h"

enum Log_Level_T
{
	OFF_LOG_LEVEL,
	FATAL_LOG_LEVEL,
	ERROR_LOG_LEVEL,
	WARN_LOG_LEVEL,
	INFO_LOG_LEVEL,
	DEBUG_LOG_LEVEL,
	TRACE_LOG_LEVEL,
};

class CLevel//:public SharedObject
{
public:
	CLevel(Log_Level_T value=FATAL_LOG_LEVEL);
	virtual ~CLevel();
	
	int GetValue();
	std::string GetName();
	
	virtual bool Greater(CLevel level);
public:
	static const std::string Log_Level_Name[7];
private:
	std::string m_LevelName;
	Log_Level_T m_LevelValue;
};

//typedef Shared_Ptr<CLevel> SharedLevelPtr;
#endif

