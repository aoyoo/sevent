/*
 *Module:	log
 *File:		FileHandler.h
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
 
#ifndef __FILE_HANDLER_HH
#define __FILE_HANDLER_HH
#include "CHandler.h"
#include "CLogFile.h"

class FileHandler:public CHandler
{
public:
	FileHandler():CHandler(),m_log(NULL)
	{
		m_log=CLogFile::Instance();
	}
	~FileHandler()
	{
		if(m_log)
		{	m_log->DestroyInstance();
			m_log=NULL;
		}
	}

	void OutPut(string msg)
	{
		if(m_log==NULL)
		{
			return;
		}
		*m_log<<msg;//<<endl;
	}	
	void SetLogPath(const string &path)
	{
		m_log->SetLogPath(path);
	}
private:
	FileHandler(const FileHandler&);
	FileHandler& operator=(const FileHandler&);
private:
	CLogFile* m_log;
};
#endif

