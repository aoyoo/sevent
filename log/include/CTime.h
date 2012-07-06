/*
 *Module:	
 *File:		CTime.h
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
 
#ifndef __TIME_HH
#define __TIME_HH
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<assert.h>
#include<string>
#include<iostream>
#include<ostream>
#include<sstream>
#include<iomanip>

class Time
{
public:
	Time();

	enum Clock{REALTIME,MONOTONIC};
	static Time Now(Clock clock=REALTIME);
	
	static Time Seconds(int64_t);
	static Time MillSeconds(int64_t);
	static Time MicroSeconds(int64_t);

	static Time SecondsDouble(double);
	static Time MillSecondsDouble(double);
	static Time MicroSecondsDouble(double);

	operator timeval()const;

	int64_t ToSeconds()const;
	int64_t ToMillSeconds()const;
	int64_t ToMicroSeconds()const;

	double ToSecondsDouble()const;
	double ToMillSecondsDouble()const;
	double ToMicroSecondsDouble()const;

	std::string ToDateTime()const;
	std::string ToDateTimeMs()const;
	std::string ToDuration()const;
	//std::string ToDurationMs()const;

	Time operator-(const Time& rhs)const
	{
		return Time(m_Usec-rhs.m_Usec);
	}

	Time operator+(const Time& rhs)const
	{
		return Time(m_Usec+rhs.m_Usec);
	}

	Time& operator+=(const Time& rhs)
	{
		m_Usec+=rhs.m_Usec;
		return *this;
	}

	Time& operator-=(const Time& rhs)
	{
		m_Usec-=rhs.m_Usec;
		return *this;
	}

	bool operator<(const Time& rhs)const
	{
		return m_Usec<rhs.m_Usec;
	}

	bool operator<=(const Time& rhs)const
	{
		return m_Usec<=rhs.m_Usec;
	}

	bool operator>(const Time& rhs)const
	{
		return m_Usec>rhs.m_Usec;
	}

	bool operator>=(const Time& rhs)const
	{
		return m_Usec>=rhs.m_Usec;
	}

	bool operator==(const Time& rhs)const
	{
		return m_Usec==rhs.m_Usec;
	}

	bool operator!=(const Time& rhs)const
	{
		return m_Usec!=rhs.m_Usec;
	}

	double operator/(const Time& rhs)const
	{
		return (double)m_Usec/(double)rhs.m_Usec;
	}

	Time& operator*=(int rhs)
	{
		m_Usec*=rhs;
		return *this;
	}

	Time operator*(int rhs)const
	{
		Time t;
		t.m_Usec=m_Usec*rhs;
		return t;
	}

	Time& operator/=(int rhs)
	{
		m_Usec/=rhs;
		return *this;
	}

	Time operator/(int rhs)const
	{
		Time t;
		t.m_Usec=m_Usec/rhs;
		return t;
	}

	Time& operator*=(int64_t rhs)
	{
		m_Usec*=rhs;
		return *this;
	}

	Time operator*(int64_t rhs)const
	{
		Time t;
		t.m_Usec=m_Usec*rhs;
		return t;
	}

	Time& operator/=(int64_t rhs)
	{
		m_Usec/=rhs;
		return *this;
	}

	Time operator/(int64_t rhs)
	{
		Time t;
		t.m_Usec=m_Usec/rhs;
		return t;
	}

	Time& operator*=(double rhs)
	{
		m_Usec=static_cast<int64_t>(static_cast<double>(m_Usec)*rhs);
		return *this;
	}

	Time operator*(double rhs)const
	{
		Time t;
		t.m_Usec=static_cast<int64_t>(static_cast<double>(m_Usec)*rhs);
		return t;
	}

	Time& operator/=(double rhs)
	{
		m_Usec=static_cast<int64_t>(static_cast<double>(m_Usec)/rhs);
	}
	Time operator/(double rhs)
	{
		Time t;
		t.m_Usec=static_cast<int64_t>(static_cast<double>(m_Usec)/rhs);
		return t;
	}
protected:

private:
	Time(int64_t);
	int64_t m_Usec;
 };
#endif

