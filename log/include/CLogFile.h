/*
 *Module:	log
 *File:		CLogFile.h
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
 
#ifndef __CLOG_FILE__HH
#define __CLOG_FILE_HH
#include <fstream>
#include <iostream>
#include <ostream>
#include "CSingleton-ThreadSafe.h"
#include "CommonOperation.h"
#include "CTime.h"

using namespace std;

#ifndef TIME_BUF_SIZE
#define TIME_BUF_SIZE 0x40
#endif
#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "./"
#endif
#ifndef LOG_PATH_MODE
#define LOG_PATH_MODE 0755
#endif

class CLogFile:public CSingleton_Ts<CLogFile>
{
public:
	CLogFile()
		:m_pLogFile(NULL)
		,m_strLogPath(LOG_FILE_PATH)
	{
	//	CreateLogPath();
	}

	~CLogFile()
	{
		Close();
		if(m_pLogFile!=NULL)
		{
			delete m_pLogFile;
		}
	}

	void SetLogPath(const string &path)
	{
		m_strLogPath=path+"/";
		m_strLogPath+=LOG_FILE_PATH;
		CreateLogPath();
	}
	template <typename T>
	friend ostream &operator<<(CLogFile& log_os,T *t)
	{
		string str_time=Time::Now().ToDateTime();
		
		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	friend ostream &operator<<(CLogFile& log_os,int t)
	{
		string str_time=Time::Now().ToDateTime();
		
		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	
	friend ostream &operator<<(CLogFile& log_os,const long& t)
	{
		string str_time=Time::Now().ToDateTime();

		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	
	friend ostream &operator<<(CLogFile& log_os,const long long& t)
	{
		string str_time=Time::Now().ToDateTime();

		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}

	friend ostream &operator<<(CLogFile& log_os,const float& t)
	{
		string str_time=Time::Now().ToDateTime();

		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	friend ostream &operator<<(CLogFile& log_os,const double& t)
	{		
		string str_time=Time::Now().ToDateTime();
		
		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	friend ostream &operator<<(CLogFile& log_os,const char& t)
	{
		string str_time=Time::Now().ToDateTime();

		log_os.Open();
		clog<<"["<<str_time<<"] "<<t;
		
		return clog;
	}
	friend ostream &operator<<(CLogFile& log_os,const string& t)
	{		
		string str_time=Time::Now().ToDateTime();

		log_os.Open();
		
		clog<<"["<<str_time<<"] "<<t;
		clog.flush();
		
		return clog;
	}

protected:

private:
	void Open()
	{
		string logfile=m_strLogPath;
		logfile+=GenerateFileName();

		if(NULL==m_pLogFile)
		{
			m_pLogFile=new ofstream();
		}
		
		if(logfile!=m_strLogFile)
		{
			m_Mutex.WriteLock();
			if(logfile!=m_strLogFile)
			{
				DEBUGMSG("%s\n",logfile.c_str());
				Close();
				m_pLogFile->open(logfile.c_str(),ios::app);
				m_StreamBuf=clog.rdbuf(m_pLogFile->rdbuf());
				m_strLogFile=logfile;
			}
			m_Mutex.UnLock();
		}

		return ;
	}

	void Close()
	{
		if(m_pLogFile!=NULL)
		{
			if(m_pLogFile->is_open())
			{
				m_pLogFile->flush();
				clog.rdbuf(m_StreamBuf);
				m_pLogFile->close();
			}
		}
	}
	
	string GenerateFileName()
	{
		char buf[MAX_PATH+1]={0};

		time_t now_time;
		tm now_ts;

		now_time=time(NULL);
		localtime_r(&now_time,&now_ts);
		
		strftime(buf,MAX_PATH,"%Y%m%d.log",&now_ts);

		return string(buf);
	}

	void CreateLogPath()
	{
		if(!Access(m_strLogPath.c_str()))
		{
			Mkdir(m_strLogPath.c_str(),LOG_PATH_MODE);
		}
	}
private:
	ofstream*m_pLogFile;
	string m_strLogFile;
	string m_strLogPath;
	CThreadMutex m_Mutex;
	streambuf* m_StreamBuf;
};
#endif

