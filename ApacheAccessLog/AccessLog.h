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

#define _CRT_SECURE_NO_WARNINGS

#include <string>

class CAccessLog
{
public:
	CAccessLog();

	bool Parse( const char * pszLog );

	std::string m_strClientIp;
	std::string m_strDate;
	std::string m_strTime;
	std::string m_strPath;
	int m_iStatus;
	int m_iSize;

private:
	void Clear();
	bool ParseDate( const char * pszDate );
	bool ParseRequest( const char * pszRequest );
};
