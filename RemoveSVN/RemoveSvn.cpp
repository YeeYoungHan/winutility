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
 * @ingroup RemoveSVN
 * @brief ���丮�� �����Ѵ�.
 * @param pszDirectory ���丮 full path
 */
void DeleteDirectory( const char * pszDirectory )
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
				DeleteDirectory( szFileName );
			}
			else
			{
				// �б� ���� �����̸� �б� ������ ������ ��, ���� �����Ѵ�.
				DWORD dwAttr = GetFileAttributes( szFileName );
				if( dwAttr & FILE_ATTRIBUTE_READONLY )
				{
					dwAttr &= ~FILE_ATTRIBUTE_READONLY;
					SetFileAttributes( szFileName, dwAttr );
				}

				if( DeleteFile( szFileName ) == FALSE )
				{
					printf( "DeleteFile(%s) error(%d)\n", szFileName, GetLastError() );
				}
			}
		}

		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);

	RemoveDirectory( pszDirectory );
}

/**
 * @ingroup RemoveSVN
 * @brief ���丮�� ���Ե� .svn ������ ��� �����Ѵ�.
 * @param pszDirectory ���丮 full path
 */
void DirectoryRemoveSVN( const char * pszDirectory )
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
				if( !strcmp( wfd.cFileName, ".svn" ) )
				{
					DeleteDirectory( szFileName );
				}
				else
				{
					DirectoryRemoveSVN( szFileName );
				}
			}
		}

		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {directory full path}\n", argv[0] );
		return 0;
	}

	DirectoryRemoveSVN( argv[1] );

	return 0;
}
