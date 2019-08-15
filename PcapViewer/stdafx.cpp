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
#include <sys/stat.h>

/**
 * @ingroup PcapViewer
 * @brief 숫자가 저장된 문자열을 HEX 만 저장된 문자열로 변환한다.
 * @param pszInput	숫자가 저장된 문자열
 * @param iInputLen	pszInput 변수의 길이
 * @param strOutput [out] HEX 만 저장된 문자열
 */
void StringToHex( const char * pszInput, int iInputLen, std::string & strOutput )
{
	char szHex[3];

	strOutput.clear();

	for( int i = 0; i < iInputLen; ++i )
	{
		snprintf( szHex, sizeof(szHex), "%02x", (unsigned char)pszInput[i] );
		strOutput.append( szHex );
	}
}

/**
 * @ingroup PcapViewer
 * @brief 문자열이 출력 가능한지 검사한다.
 * @param pszText		문자열
 * @param iTextLen	문자열 길이
 * @returns 문자열이 출력 가능하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsPrintString( const char * pszText, int iTextLen )
{
	for( int i = 0; i < iTextLen; ++i )
	{
		if( isprint( (unsigned char)pszText[i] ) == 0 ) 
		{
			if( pszText[i] == 0x0d || pszText[i] == 0x0a ) continue;

			return false;
		}
	}

	return true;
}

/**
 * @ingroup PcapViewer
 * @brief 파일이 존재하는지 검사한다.
 * @param pszFileName 파일 이름
 * @returns 파일이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}
