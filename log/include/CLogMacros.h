/*
 *Module:	log
 *File:		CLogMacros.h
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
 
#ifndef __CLOG_MACROS_HH
#define __CLOG_MACROS_HH
#include<iostream>
#include<ostream>
#include<sstream>
#include<iomanip>
#include <stdarg.h>
#include "CLogger.h"

#define LOG_MACRO_BODY(logger,logRecord,logLevel) \
	do{	\
		std::ostringstream os;\
		os<<logRecord;\
		CLogRecord lRecord(os.str(),__FILE__,__LINE__,CLevel(logLevel##_LOG_LEVEL));\
		logger.Log(lRecord);\
	}while(0)

#ifndef LOG_FATAL
#define LOG_FATAL(logger,logRecord) \
	LOG_MACRO_BODY(logger, logRecord, FATAL)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(logger,logRecord) \
	LOG_MACRO_BODY(logger, logRecord, ERROR)
#endif

#ifndef LOG_WARN
#define LOG_WARN(logger,logRecord) \
	LOG_MACRO_BODY(logger, logRecord, WARN)
#endif
#ifndef LOG_INFO
#define LOG_INFO(logger,logRecord) \
	LOG_MACRO_BODY(logger, logRecord, INFO)
#endif
#ifndef LOG_DEBUG
#define LOG_DEBUG(logger,logRecord) \
	LOG_MACRO_BODY(logger, logRecord, DEBUG)
#endif
#ifndef LOG_TRACE
#define LOG_TRACE(logger,logRecord)	\
	LOG_MACRO_BODY(logger, logRecord, TRACE)
#endif

#include "CLogManager.h"

#define LOG_LEVEL_VA(level, fmt, ...) \
	do {								\
		char s[1024];						\
		snprintf(s, 1023, fmt"\n", ##__VA_ARGS__);		\
		CLogRecord rcd(s, __FILE__, __LINE__, CLevel(level));	\
		CLogManager::Instance()->GetLogger().Log(rcd);		\
	} while (0)

#define LOG_FATAL_VA(fmt, ...) LOG_LEVEL_VA(FATAL_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOG_ERROR_VA(fmt, ...) LOG_LEVEL_VA(ERROR_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOG_WARN_VA(fmt, ...) LOG_LEVEL_VA(WARN_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOG_INFO_VA(fmt, ...) LOG_LEVEL_VA(INFO_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_VA(fmt, ...) LOG_LEVEL_VA(DEBUG_LOG_LEVEL, fmt, ##__VA_ARGS__)
#define LOG_TRACE_VA(fmt, ...) LOG_LEVEL_VA(TRACE_LOG_LEVEL, fmt, ##__VA_ARGS__)

#endif

