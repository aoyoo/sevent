/*
 *Module:	
 *File:		Shared_Ptr.h
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
 
#ifndef __LOG_HH
#define __LOG_HH
#include "CLogManager.h"
#include "CLogMacros.h"
#include "ConsoleFormatter.h"
#include "FileFormatter.h"
#include "FileHandler.h"
#include "ConsoleHandler.h"
#include <string>

bool init_logger(const std::string& path, const std::string &level);
#endif

