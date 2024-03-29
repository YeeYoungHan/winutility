/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#pragma once

#include "AccessLog.h"
#include <map>

typedef __int64 int64_t;

class CAccessLogStatMonth
{
public:
	CAccessLogStatMonth() : m_iSize(0)
	{}

	int64_t m_iSize;
};

// key = month/year
typedef std::map< std::string, CAccessLogStatMonth > ACCESS_LOG_STAT_MONTH_MAP;

class CAccessLogStat
{
public:
	CAccessLogStat();

	void Add( CAccessLog & clsLog );
	void PrintMonthStat( const char * pszOutputFolder );

private:
	ACCESS_LOG_STAT_MONTH_MAP m_clsMonthMap;
};
