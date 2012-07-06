/*
 *Module:	
 *File:		CTime.cpp
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
 
#include "CTime.h"
Time::Time()
	:m_Usec(0)
{

}

Time Time::Now(Clock clock)
{
	if(clock==REALTIME)
	{
		struct timeval tv;
		if(gettimeofday(&tv,0)<0)
		{
			assert(0);
		}

		return Time(tv.tv_sec*int64_t(1000000)+tv.tv_usec);
	}else{
		struct timespec ts;
		if(clock_gettime(CLOCK_MONOTONIC,&ts)<0)
		{
			assert(0);
		}
		return Time(ts.tv_sec*int64_t(1000000)+ts.tv_nsec/int64_t(1000));
	}
}

Time Time::Seconds(int64_t t)
{
	return Time(t*int64_t(1000000));
}

Time Time::MillSeconds(int64_t t)
{
	return Time(t*int64_t(1000));
}

Time Time::MicroSeconds(int64_t t)
{
	return Time(t);
}

Time Time::SecondsDouble(double t)
{
	return Time(int64_t(t*1000000));
}

Time Time::MillSecondsDouble(double t)
{
	return Time(int64_t(t*1000));
}

Time Time::MicroSecondsDouble(double t)
{
	return Time(int64_t(t));
}

Time::operator timeval()const
{
	struct timeval tv;
	tv.tv_sec=static_cast<long>(m_Usec/1000000);
	tv.tv_usec=static_cast<long>(m_Usec%1000000);
	return tv;
}

int64_t Time::ToSeconds()const
{
	return m_Usec/1000000;
}

int64_t Time::ToMillSeconds()const
{
	return m_Usec/1000;
}

int64_t Time::ToMicroSeconds()const
{
	return m_Usec;
}

double Time::ToSecondsDouble()const
{
	return m_Usec/1000000.0;
}

double Time::ToMillSecondsDouble()const
{
	return m_Usec/1000.0;
}

double Time::ToMicroSecondsDouble()const
{
	return static_cast<double>(m_Usec);
}

std::string Time::ToDateTime()const
{
	time_t time=static_cast<long>(m_Usec/1000000);

	struct tm *t;
	struct tm tr;
	localtime_r(&time,&tr);
	t=&tr;

	char buf[64];
	strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",t);

	std::ostringstream os;
	os<<buf;
	return os.str();
}

std::string Time::ToDateTimeMs()const
{
	time_t time=static_cast<long>(m_Usec/1000000);

	struct tm *t;
	struct tm tr;
	localtime_r(&time,&tr);
	t=&tr;

	char buf[64];
	strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",t);

	std::ostringstream os;
	os<<ToDateTime()<<".";
	os.fill('0');
	os.width(3);
	os<<static_cast<long>(m_Usec%1000000/1000);
	return os.str();
}

std::string Time::ToDuration()const
{
	int64_t usecs = m_Usec % 1000000;
	int64_t secs = m_Usec / 1000000 % 60;
	int64_t mins = m_Usec / 1000000 / 60 % 60;
	int64_t hours = m_Usec / 1000000 / 60 / 60 % 24;
	int64_t days = m_Usec / 1000000 / 60 / 60 / 24;

	using namespace std;

	ostringstream os;
	if(days != 0)
	{
		os << days << "d ";
	}
	os << setfill('0') << setw(2) << hours << ":" << setw(2) << mins << ":" << setw(2) << secs;
	if(usecs != 0)
	{
		os << "." << setw(3) << (usecs / 1000);
	}

	return os.str();

}

Time::Time(int64_t usec) 
	:m_Usec(usec)
{

}
	
