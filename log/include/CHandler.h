/*
 *Module:	log
 *File:		CHandler.h
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
 
#ifndef __CHANDLER_HH
#define __CHANDLER_HH
#include<string>
#include<iostream>
#include<vector>
#include "CFormatter.h"
#include "CLevelFilter.h"
#include "Shared_Ptr.h"

using namespace std;

class CHandler:public SharedObject
{
public:
	CHandler()
		:m_FilterPtr(NULL),
		m_FormatterPtr(NULL)
	{
	}
	~CHandler()
	{
	}

	virtual void OutPut(string msg)=0;
	
	void Publish(CLogRecord record)
	{
		//DEBUGMSG("record:%s,%d\n",record.GetLevel().GetName().c_str(),record.GetLevel().GetValue());
		bool bLog=false;

		if(m_FilterPtr!=NULL)
		{
			bLog=m_FilterPtr->IsLogable(record);
		}

		if(bLog)
		{
			string msg;
			if(m_FormatterPtr!=NULL)
			{
				msg=m_FormatterPtr.Get()->FormatLog(record);
			}else{
				msg=record.GetMessage();
			}
			OutPut(msg);
		}

		return ;
	}

	void SetFormatter(SharedFormatterPtr formatter_ptr)
	{
		m_FormatterPtr=formatter_ptr;
	}
	
	SharedFormatterPtr GetFormatter()
	{
		return m_FormatterPtr;
	}

	void SetFilter(SharedFilterPtr filter_ptr)
	{
		m_FilterPtr=filter_ptr;
	}
	
	SharedFilterPtr GetFilter()
	{
		return m_FilterPtr;
	}
private:
	CHandler(const CHandler&);
	CHandler& operator=(const CHandler& );
private:
	SharedFormatterPtr m_FormatterPtr;
	SharedFilterPtr m_FilterPtr;
};

typedef Shared_Ptr<CHandler> SharedHandlerPtr;
#endif

