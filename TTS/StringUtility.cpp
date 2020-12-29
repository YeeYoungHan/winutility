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

#include "TTS.h"

bool WcharToChar( const WCHAR * pszWchar, std::string & strOutput )
{
	int iLength = WideCharToMultiByte( CP_ACP, 0, pszWchar, -1, NULL, 0, NULL, NULL );
	if( iLength == 0 )
	{
		return false;
	}

	char * pszAnsi = new char[iLength];
	if( pszAnsi == NULL )
	{
		return false;
	}

	WideCharToMultiByte( CP_ACP, 0, pszWchar, -1, pszAnsi, iLength, NULL, NULL );
	strOutput = pszAnsi;
	
	delete [] pszAnsi;

	return true;
}

bool CharToWchar( const char * pszChar, WCHAR * pszOutput, int iOutputSize )
{
	MultiByteToWideChar( CP_ACP, 0, pszChar, -1, pszOutput, iOutputSize );

	return true;
}
