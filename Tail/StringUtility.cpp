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

#include "Tail.h"

/**
 * @ingroup SipPlatform
 * @brief UTF8 ���ڿ��� ANSI ���ڿ��� ��ȯ�Ѵ�.
 * @param pszUtf8		UTF8 ���ڿ� (input)
 * @param strOutput ANSI ���ڿ� (output)
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool Utf8ToAnsi( const char * pszUtf8, std::string & strOutput )
{
	BSTR    strWide = NULL;
	char*   pszAnsi = NULL;
	int     iLength;
	int			iUtf8Length = (int)strlen(pszUtf8) + 1;
	
	iLength = MultiByteToWideChar( CP_UTF8, 0, pszUtf8, iUtf8Length, NULL, NULL );
	if( iLength == 0 )
	{
		return false;
	}

	strWide = SysAllocStringLen( NULL, iLength );
	if( strWide == NULL )
	{
		return false;
	}

	MultiByteToWideChar( CP_UTF8, 0, pszUtf8, iUtf8Length, strWide, iLength );

	iLength = WideCharToMultiByte( CP_ACP, 0, strWide, -1, NULL, 0, NULL, NULL );
	if( iLength == 0 )
	{
		SysFreeString( strWide );
		return false;
	}

	pszAnsi = new char[iLength];
	if( pszAnsi == NULL )
	{
		SysFreeString( strWide );
		return false;
	}

	WideCharToMultiByte( CP_ACP, 0, strWide, -1, pszAnsi, iLength, NULL, NULL );
	strOutput = pszAnsi;
	
	SysFreeString( strWide );
	delete [] pszAnsi;
	
	return true;
}
