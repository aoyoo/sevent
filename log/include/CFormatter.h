/*
 *Module:	log
 *File:		CFormatter.h
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
 
#ifndef __CFORMATTER_HH
#define __CFORMATTER_HH
#include<string>
#include "Shared_Ptr.h"

class CLogRecord;

class CFormatter:public SharedObject
{
public:
	CFormatter()
	{

	}
	virtual ~CFormatter()
	{

	}

	virtual std::string FormatLog(CLogRecord record)=0;
private:
	CFormatter(const CFormatter&);
	CFormatter& operator=(const CFormatter&);
};

typedef Shared_Ptr<CFormatter> SharedFormatterPtr;

#endif

