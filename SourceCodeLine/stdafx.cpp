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

#include "stdafx.h"

void CommaSepString( std::string & strText )
{
	int iLen = (int)strText.length();
	int iCount = 0;

	for( int i = iLen - 1; i > 0; --i )
	{
		if( i % 3 == 0 )
		{
			strText.insert( iLen - i + iCount, "," );
			++iCount;
		}
	}
}

bool GetFileExt( const char * pszFilePath, std::string & strExt )
{
	strExt.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

	if( pszFilePath[iLen-1] == '\\' ) return false;

	for( int i = iLen - 2; i >= 0; --i )
	{
		if( pszFilePath[i] == '\\' )
		{
			break;
		}

		if( pszFilePath[i] == '.' )
		{
			strExt.append( pszFilePath + i + 1 );
			return true;
		}
	}

	return false;
}
