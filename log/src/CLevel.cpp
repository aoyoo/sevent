/*
 *Module:	log
 *File:		CLevel.cpp
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
 
#include "CLevel.h"

const std::string CLevel::Log_Level_Name[7]={
		"OFF",
		"FATAL",
		"ERROR",
		"WARN",
		"INFO",
		"DEBUG",
		"TRACE",
	};

CLevel::CLevel(Log_Level_T value)
		:m_LevelName(CLevel::Log_Level_Name[value]),m_LevelValue(value)
{
}

CLevel::~CLevel()
{
}

int CLevel::GetValue()
{
	return m_LevelValue;
}

std::string CLevel::GetName()
{
	return m_LevelName;
}

bool CLevel::Greater(CLevel level)
{
	if(level.GetValue()==OFF_LOG_LEVEL||m_LevelValue==OFF_LOG_LEVEL)
	{
		return false;
	}
	
	return ((m_LevelValue<=level.GetValue())?true:false);
}

