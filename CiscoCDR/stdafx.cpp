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

STRING_LIST gclsColList;

void SplitString( const char * pszText, STRING_LIST & clsList, char cSep )
{
	int iStartPos = -1, i;

	clsList.clear();

	for( i = 0; pszText[i]; ++i )
	{
		if( pszText[i] == cSep )
		{
			if( iStartPos >= 0 )
			{
				std::string strTemp;

				strTemp.append( pszText + iStartPos, i - iStartPos );
				clsList.push_back( strTemp );
			}

			iStartPos = i + 1;
		}
		else if( i == 0 )
		{
			iStartPos = 0;
		}
	}

	if( iStartPos >= 0 )
	{
		std::string strTemp;

		strTemp.append( pszText + iStartPos, i - iStartPos );
		clsList.push_back( strTemp );
	}
}

void ReplaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );
	size_t iAfterLen = strlen( pszAfter );

	while( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		iPos = strCallId.find( pszBefore, iPos + iAfterLen );
	}
}
