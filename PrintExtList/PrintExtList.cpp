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
#include <stdio.h>
#include <windows.h>

/**
 * @ingroup PrintExtList
 * @brief 폴더에 포함된 파일들을 CExtMap 에 저장한다.
 * @param pszFolder 폴더 full path
 */
void SearchFolder( const char * pszFolder )
{
	int				iLineCount = 0;
	char			szPath[2048];
	HANDLE		hSearch;
	WIN32_FIND_DATA wfd;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszFolder );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL	bFind = TRUE;
	
		while( bFind )
		{
			if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
			{
				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					_snprintf( szPath, sizeof(szPath), "%s\\%s", pszFolder, wfd.cFileName );
					SearchFolder( szPath );
				}
				else
				{
					gclsExtMap.Insert( wfd.cFileName );
				}
			}

			bFind = FindNextFile( hSearch, &wfd );
		}

		FindClose( hSearch );
	}
}

/**
 * @ingroup PrintExtList
 * @brief main 함수
 * @param argc 
 * @param argv 
 * @returns 0 을 리턴한다.
 */
int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {folder path}\n", argv[0] );
		return 0;
	}

	SearchFolder( argv[1] );

	gclsExtMap.Print();

	return 0;
}
