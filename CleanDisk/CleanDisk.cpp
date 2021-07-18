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

#include <stdio.h>
#include <windows.h>
#include <string>

typedef  unsigned __int64 uint64_t;

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {disk}\n", argv[0] );
		printf( "        %s d:\\\n", argv[0] );
		return 0;
	}

	std::string strFileName = argv[1];
	strFileName.append( "cleandisk.txt" );

	HANDLE	hFile;
	char		szBuf[8192];
	DWORD		dwWriteWantLen, dwWriteLen;

	memset( szBuf, 0, sizeof(szBuf) );

	hFile = CreateFile( strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		printf( "file(%s) open error(%d)\n", strFileName.c_str(), GetLastError() );
		return 0;
	}

	while( 1 )
	{
		dwWriteWantLen = sizeof(szBuf);

		if( WriteFile( hFile, szBuf, dwWriteWantLen, &dwWriteLen, NULL ) == FALSE )
		{
			break;
		}
	}

	CloseHandle( hFile );

	DeleteFile( strFileName.c_str() );

	return 0;
}
