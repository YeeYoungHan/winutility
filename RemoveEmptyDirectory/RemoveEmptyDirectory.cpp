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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>

/**
 * @ingroup RemoveEmptyDirectory
 * @brief 입력된 폴더 및 하위 폴더 중에서 빈 폴더들을 삭제한다.
 * @param pszDirectory 폴더 full path
 * @returns 삭제되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool RemoveEmptyDirectory( const char * pszDirectory )
{
	char			szPath[2048];
	char			szFileName[2048];
	HANDLE		hSearch;
	BOOL			bOk = TRUE;
	int				iFileCount = 0;
	WIN32_FIND_DATA wfd;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszDirectory );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		printf( "folder(%s) is not found\n", pszDirectory );
		return false;
	}
	
	while( bOk )
	{
		if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
		{
			_snprintf( szFileName, sizeof(szFileName), "%s\\%s", pszDirectory, wfd.cFileName );

			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( RemoveEmptyDirectory( szFileName ) == false )
				{
					++iFileCount;
				}
			}
			else
			{
				++iFileCount;
			}
		}

		bOk = FindNextFile( hSearch, &wfd );
	}

	FindClose( hSearch );

	if( iFileCount == 0 )
	{
		RemoveDirectory( pszDirectory );
		return true;
	}

	return false;
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {directory full path}\n", argv[0] );
		return 0;
	}

	RemoveEmptyDirectory( argv[1] );

	return 0;
}
