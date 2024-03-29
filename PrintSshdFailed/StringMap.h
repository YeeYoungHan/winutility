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

#ifndef _STRING_MAP_H_
#define _STRING_MAP_H_

#include <string>
#include <vector>
#include <map>

class CStringInt
{
public:
	std::string m_strName;
	int					m_iCount;
};

typedef std::map< std::string, int > STRING_MAP;
typedef std::vector< CStringInt > STRING_VECTOR;

class CStringMap
{
public:
	void Insert( const char * pszKey );
	void Print( );
	void PrintTop( int iTopCount );
	int GetCount( );

	STRING_MAP m_clsMap;
};

#endif
