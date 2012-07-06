/*
 *Module:	log
 *File:		CFilter.h
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
 
#ifndef __CFILTER_HH
#define __CFILTER_HH
#include "Shared_Ptr.h"

class CLogRecord;

class CFilter:public SharedObject
{
public:
	CFilter()
	{
	}
	
	virtual ~CFilter()
	{
	}
	
	virtual bool IsLogable(CLogRecord record)=0;
private:
	CFilter(const CFilter&);
	CFilter& operator=(const CFilter&);
};

typedef Shared_Ptr<CFilter> SharedFilterPtr;
#endif

