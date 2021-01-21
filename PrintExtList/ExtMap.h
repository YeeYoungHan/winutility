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

#include <map>
#include <string>
#include <vector>
#include <algorithm>

/**
 * @ingroup PrintExtList
 * @brief 확장자 정보 저장 클래스
 */
class CExtInfo
{
public:
	CExtInfo();

	std::string m_strExt;
	int					m_iCount;
};

typedef std::map< std::string, CExtInfo > EXT_MAP;
typedef std::vector< CExtInfo > EXT_VECTOR;

/**
 * @ingroup PrintExtList
 * @brief 확장자 정보 저장 자료구조 클래스
 */
class CExtMap
{
public:
	CExtMap();

	void Insert( const char * pszFileName );
	void Print( );

	EXT_MAP m_clsMap;
};

extern CExtMap gclsExtMap;
