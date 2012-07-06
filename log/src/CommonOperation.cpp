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

#include "CommonOperation.h"

bool Access(const char *pPath)
{
	if(NULL==pPath)
		return false;
	return (0==access(pPath,F_OK));
}
/*
bool Mkdir(const char* pPath,int mode)
{
	if(NULL==pPath)
		return false;
	return (0==mkdir(pPath,mode));
}
*/
//mkdir -p 
bool Mkdir(const char *pPath,int mode)
{
        if(!Access(pPath))
        {
                if(pPath!=NULL&&strlen(pPath)>0)
                {
                        char path_name[PATH_MAX];
                        strcpy(path_name,pPath);
                        char *p=strrchr(path_name,'/');
                        if(p!=NULL)
                        {
                                *p='\0';
                                Mkdir(path_name,mode);
                        }
                }
        }else{
                return true;
        }
        return (0==mkdir(pPath,mode));
}

int GetFileSize(const char * pFileName)
{
	struct stat f_stat;
	if(stat(pFileName,&f_stat)==0)
		return f_stat.st_size;
	return -1;
}

bool GetSelfExecPath(char *pBuf,int nBufSize)
{
	int nSize=0;
	char buf[PATH_MAX+1]={0};

	if(NULL==pBuf||nBufSize<=0)
		return false;
	nSize=readlink ("/proc/self/exe",buf,MAX_PATH);

	char *p=strrchr(buf,'/');

	if(p!=NULL)
	{
		*p='\0';	
		memset(pBuf,0,nBufSize);
		strcpy(pBuf,buf);
	}
	return !(-1==nSize);
}

bool GetSelfExecName(char * pBuf, int nBufSize)
{
	int nSize=0;
	char buf[PATH_MAX+1]={0};
	
	if(NULL==pBuf||nBufSize<=0)
		return false;

	nSize=readlink ("/proc/self/exe",buf,PATH_MAX);

	char *p=strrchr(buf,'/');

	if(p!=NULL)
	{
		strncpy(pBuf,p+1,strlen(p));
		return true;
	}
	return false;
}

bool IsBigEndEndian()
{
	unsigned short n=0x1234;
	if(*((unsigned char*)&n)==0x12)
		return true;
	return false;
}

int SetMaxOpenFile(int maxfile)
{
	struct rlimit rlim;
	if(getrlimit(RLIMIT_NOFILE,&rlim)!=0)
	{
		DEBUGMSG("failed to getrlimit number of files.\n");
		return -1;
	}else{
		rlim.rlim_cur=MAX(rlim.rlim_cur,maxfile);
		rlim.rlim_max=MAX(rlim.rlim_max,rlim.rlim_cur);
		if(setrlimit(RLIMIT_NOFILE,&rlim)!=0)
		{
			DEBUGMSG("failed to set rlimit for open files.\n");
			return -1;
		}
	}
	return 0;
}

