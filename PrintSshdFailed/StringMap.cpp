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

#include "StringMap.h"
#include <algorithm>

void CStringMap::Insert( const char * pszKey )
{
	STRING_MAP::iterator itMap;

	itMap = m_clsMap.find( pszKey );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( STRING_MAP::value_type( pszKey, 1 ) );
	}
	else
	{
		++itMap->second;
	}
}

void CStringMap::Print( )
{
	STRING_MAP::iterator itMap;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		printf( "%s => %d\n", itMap->first.c_str(), itMap->second );
	}
}

static bool Compare( CStringInt & clsA, CStringInt & clsB )
{
	if( clsA.m_iCount > clsB.m_iCount ) return true;

	return false;
}

void CStringMap::PrintTop( int iTopCount )
{
	STRING_VECTOR arrBuf( m_clsMap.size() );
	STRING_MAP::iterator itMap;
	int iIndex = 0;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		arrBuf[iIndex].m_strName = itMap->first;
		arrBuf[iIndex].m_iCount = itMap->second;

		++iIndex;
	}

	std::sort( arrBuf.begin(), arrBuf.end(), Compare );

	if( iTopCount > (int)m_clsMap.size() )
	{
		iTopCount = (int)m_clsMap.size();
	}

	for( int i = 0; i < iTopCount; ++i )
	{
		printf( "%s => %d\n", arrBuf[i].m_strName.c_str(), arrBuf[i].m_iCount );
	}
}

int CStringMap::GetCount( )
{
	return m_clsMap.size();
}
