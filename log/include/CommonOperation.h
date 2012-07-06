/*
 *Module:	
 *File:		CommonOperation.h
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
 
#ifndef __COMMON_OPERATION_HH
#define __COMMON_OPERATION_HH
#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<sys/time.h>
#include<sys/resource.h>

//#define DEBUG

#ifndef MAX_PATH
#define MAX_PATH 0x400
#endif


#ifdef DEBUG
	#define DEBUGMSG(x...)printf("[%s %d %s]  ", __FILE__, __LINE__, __FUNCTION__);printf(x);
#else
	#define DEBUGMSG(x...)
#endif

#ifdef DEBUG
	#define PERRORMSG(x) perror(x);
#else
	#define PERRORMSG(x)
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef Tranverse16
#define Tranverse16(X)	(\
							(((uint16_t)(X) & 0xff00) >> 8) |\
							(((uint16_t)(X) & 0x00ff) << 8))
#endif

#ifndef Tranverse24
#define Tranverse24(X)	(\
							(((uint32_t)(X)&0x0000ff)<<16)| \
							(((uint32_t)(X)&0x00ff00))| \
							(((uint32_t)(X)&0xff0000)>>16))
#endif

#ifndef Tranverse32
#define Tranverse32(X)	(\
							(((uint32_t)(X) & 0xff000000) >> 24) | \
							(((uint32_t)(X) & 0x00ff0000) >> 8) | \
							(((uint32_t)(X) & 0x0000ff00) << 8) | \
							(((uint32_t)(X) & 0x000000ff) << 24))
#endif

bool Access(const char *pPath);

bool Mkdir(const char* pPath,int mode);

int GetFileSize(const char* pFileName);

bool GetSelfExecPath(char *pBuf,int nBufSize);

bool GetSelfExecName(char *pBuf,int nBufSize);

bool IsBigEndEndian();

int SetMaxOpenFile(int maxfile);

#endif

