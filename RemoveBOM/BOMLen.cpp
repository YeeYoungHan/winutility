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

#include "RemoveBOM.h"

/**
 * @ingroup RemoveBOM
 * @brief BOM 파일인지 검사한 후, BOM 파일이면 BOM 헤더 길이를 리턴한다.
 * @param pszBuf		파일의 최초 5byte 를 저장한 변수
 * @param iReadSize pszBuf 의 길이
 * @returns BOM 파일이면 BOM 헤더 길이를 리턴하고 그렇지 않으면 0 을 리턴한다.
 */
int GetBOMLen( unsigned char * pszBuf, int iReadSize )
{
	// 참고자료 : https://ko.wikipedia.org/wiki/%EB%B0%94%EC%9D%B4%ED%8A%B8_%EC%88%9C%EC%84%9C_%ED%91%9C%EC%8B%9D

	int		iBOMLen = 0;

	if( iReadSize >= 2 )
	{
		if( pszBuf[0] == 0xFE && pszBuf[1] == 0xFF )
		{
			// UTF-16 (BE)
			iBOMLen = 2;
		}
		else if( pszBuf[0] == 0xFF && pszBuf[1] == 0xFE )
		{
			// UTF-16 (LE)
			iBOMLen = 2;
		}
	}

	if( iBOMLen == 0 && iReadSize >= 3 )
	{
		if( pszBuf[0] == 0xEF && pszBuf[1] == 0xBB && pszBuf[2] == 0xBF )
		{
			// UTF-8
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0xF7 && pszBuf[1] == 0x64 && pszBuf[2] == 0x4C )
		{
			// UTF-1
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0x0E && pszBuf[1] == 0xFE && pszBuf[2] == 0xFF )
		{
			// SCSU
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0xFB && pszBuf[1] == 0xEE && pszBuf[2] == 0x28 )
		{
			// BOCU-1
			iBOMLen = 3;
		}
	}

	if( iBOMLen == 0 && iReadSize >= 4 )
	{
		if( pszBuf[0] == 0x00 && pszBuf[1] == 0x00 && pszBuf[2] == 0xFE && pszBuf[3] == 0xFF )
		{
			// UTF-32 (BE)
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0xFF && pszBuf[1] == 0xFE && pszBuf[2] == 0x00 && pszBuf[3] == 0x00 )
		{
			// UTF-32 (LE)
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0x2B && pszBuf[1] == 0x2F && pszBuf[2] == 0x76 )
		{
			// UTF-7
			if( pszBuf[3] == 0x38 )
			{
				iBOMLen = 4;
				if( iReadSize == 5 && pszBuf[4] == 0x2D )
				{
					iBOMLen = 5;
				}
			}
			else if( pszBuf[3] == 0x39 || pszBuf[3] == 0x2B || pszBuf[3] == 0x2F )
			{
				iBOMLen = 4;
			}
		}
		else if( pszBuf[0] == 0xDD && pszBuf[1] == 0x73 && pszBuf[2] == 0x66 && pszBuf[3] == 0x73 )
		{
			// UTF-EBCDIC
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0x84 && pszBuf[1] == 0x31 && pszBuf[2] == 0x95 && pszBuf[3] == 0x33 )
		{
			// GB-18030
			iBOMLen = 4;
		}
	}

	return iBOMLen;
}
