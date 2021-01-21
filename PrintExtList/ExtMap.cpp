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

#include "ExtMap.h"

CExtMap gclsExtMap;

CExtInfo::CExtInfo() : m_iCount(0)
{
}

CExtMap::CExtMap()
{
}

/**
 * @ingroup PrintExtList
 * @brief 확장자 정보를 저장한다.
 * @param pszFileName 파일 이름
 */
void CExtMap::Insert( const char * pszFileName )
{
	std::string strExt;
	EXT_MAP::iterator itMap;

	int iLen = (int)strlen( pszFileName );
	for( int i = iLen - 2; i >= 0; --i )
	{
		if( pszFileName[i] == '.' )
		{
			strExt.append( pszFileName + i + 1 );
			break;
		}
	}

	itMap = m_clsMap.find( strExt );
	if( itMap == m_clsMap.end() )
	{
		CExtInfo clsInfo;

		clsInfo.m_strExt = strExt;
		clsInfo.m_iCount = 1;
		m_clsMap.insert( EXT_MAP::value_type( strExt, clsInfo ) );
	}
	else
	{
		++itMap->second.m_iCount;
	}
}

bool cmp( const CExtInfo & a, const CExtInfo & b )
{
	return a.m_strExt.compare( b.m_strExt ) < 0 ;
}

/**
 * @ingroup PrintExtList
 * @brief 확장자별 파일 개수를 출력한다.
 */
void CExtMap::Print( )
{
	EXT_MAP::iterator itMap;
	EXT_VECTOR arrBuf( m_clsMap.size() );
	int iIndex = 0;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		arrBuf[iIndex] = itMap->second;
		++iIndex;
	}

	std::sort( arrBuf.begin(), arrBuf.end(), cmp );

	for( int i = 0; i < iIndex; ++i )
	{
		printf( "[%s] => %d\n", arrBuf[i].m_strExt.c_str(), arrBuf[i].m_iCount );
	}
}
