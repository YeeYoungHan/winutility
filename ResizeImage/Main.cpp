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
#include "ResizeImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void DirectoryResizeImage( const char * pszDirectory, int iWidth, int iHeight )
{
	char			szPath[2048];
	char			szFileName[2048];
	HANDLE		hSearch;
	BOOL			fisOk = TRUE;
	WIN32_FIND_DATA wfd;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszDirectory );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		printf( "folder(%s) is not found\n", pszDirectory );
		return;
	}
	
	while( fisOk )
	{
		if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
		{
			_snprintf( szFileName, sizeof(szFileName), "%s\\%s", pszDirectory, wfd.cFileName );

			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				DirectoryResizeImage( szFileName, iWidth, iHeight );
			}
			else
			{
				ResizeImage( szFileName, szFileName, iWidth, iHeight );
			}
		}

		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);
}

int _tmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("AfxWinInit error\n"));
		nRetCode = 1;
	}
	else
	{
		if( argc != 5 )
		{
			printf( "[Usage] %s {input image filename} {output image filename} {width} {height}\n", argv[0] );
		}
		else
		{
			const char * pszInput = argv[1];
			const char * pszOutput = argv[2];
			int iWidth = atoi( argv[3] );
			int iHeight = atoi( argv[4] );

			if( !strcmp( pszInput, "-d" ) )
			{
				DirectoryResizeImage( pszOutput, iWidth, iHeight );
			}
			else
			{
				ResizeImage( pszInput, pszOutput, iWidth, iHeight );
			}
		}
	}

	return nRetCode;
}
