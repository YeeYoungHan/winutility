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

#include "ApacheAccessLog.h"

void Execute( CAccessLogStat & clsStat, const char * pszLogFileName )
{
	FILE * fd = fopen( pszLogFileName, "r" );
	if( fd == NULL )
	{
		printf( "fopen(%s) error\n", pszLogFileName );
		return;
	}

	char szBuf[8192];
	CAccessLog clsLog;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		if( clsLog.Parse( szBuf ) )
		{
			clsStat.Add( clsLog );
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );
}

int main( int argc, char * argv[] )
{
	if( argc < 3 )
	{
		printf( "[Usage] %s {apache access log folder} {output folder}\n", argv[0] );
		return 0;
	}

	const char * pszAccessLogFolder = argv[1];
	const char * pszOutputFolder = argv[2];
	char			szPath[1024], szFileName[1024];
	HANDLE		hSearch;
	BOOL			bOk = TRUE;
	WIN32_FIND_DATA wfd;
	CAccessLogStat clsStat;
	
	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszAccessLogFolder );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		printf( "FindFirstFile(%s) error\n", szPath );
		return 0;
	}

	while( bOk )
	{
		if( !strncmp( wfd.cFileName, "access", 6 ) && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			_snprintf( szFileName, sizeof(szFileName), "%s\\%s", pszAccessLogFolder, wfd.cFileName );
			Execute( clsStat, szFileName );
		}

		bOk = FindNextFile( hSearch, &wfd );
	}

	FindClose( hSearch );

	clsStat.PrintMonthStat( pszOutputFolder );

	return 0;
}