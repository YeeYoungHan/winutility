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

#include "BackupDirectory.h"

FILE	* gpsttFd = NULL;

/**
 * @brief 파일의 변경 날짜를 가져온다.
 * @param szFileName		파일 full path
 * @param psttFileTime	파일의 변경 날짜를 저장할 변수
 * @returns 성공하면 0 을 리턴하고 실패하면 -1 을 리턴한다.
 */
int GetFileModifyDate( const char * szFileName, FILETIME * psttFileTime )
{
	int				iResult = 0;
	HANDLE			hSearch;
	WIN32_FIND_DATA wfd;

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szFileName, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		iResult = -1;
	}

	FindClose(hSearch);

	memcpy( psttFileTime, &wfd.ftLastWriteTime, sizeof(FILETIME) );

	return iResult;
}

/**
 * @brief 디렉토리를 백업한다.
 * @param szSrcName 원본 폴더 경로
 * @param szDstName 백업 폴더 경로
 */
void CopyDirectory( char * szSrcName, char * szDstName )
{
	char			szPath[1024*2];
	char			szSrcFileName[1024*2], szDstFileName[1024*2];
	HANDLE			hSearch;
	BOOL			fisOk = TRUE, bResult;
	WIN32_FIND_DATA wfd;
	FILETIME		ft;
	BOOL			fisCopy;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", szSrcName );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return;
	}
	
	while( fisOk )
	{
		if( !strcmp( wfd.cFileName, "." ) || !strcmp( wfd.cFileName, ".." ) )
		{
			goto WHILE_END;
		}

		_snprintf( szSrcFileName, sizeof(szSrcFileName), "%s\\%s", szSrcName, wfd.cFileName );
		_snprintf( szDstFileName, sizeof(szDstFileName), "%s\\%s", szDstName, wfd.cFileName );

		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CopyDirectory( szSrcFileName, szDstFileName );
		}
		else
		{
			fisCopy = FALSE;
			if( GetFileModifyDate( szDstFileName, &ft ) == 0 )
			{
				if( memcmp( &ft, &wfd.ftLastWriteTime, sizeof(FILETIME) ) )
				{
					fisCopy = TRUE;

					// 읽기 전용과 같은 경우, 제대로 파일이 복사되지 않는 경우가 
					// 발생할 수 있는 것 같아서 아래와 같이 파일을 삭제한 후, 복사한다.
					SetFileAttributes( szDstFileName, FILE_ATTRIBUTE_NORMAL );
					DeleteFile( szDstFileName );
				}
			}
			else
			{
				fisCopy = TRUE;
			}

			if( fisCopy == TRUE )
			{
				HCreateDir( szDstName );

				bResult = CopyFile( szSrcFileName, szDstFileName, TRUE );

				if( gpsttFd )
				{
					if( bResult )
					{
						fprintf( gpsttFd, "copy %s => %s\r\n", szSrcFileName, szDstFileName );
					}
					else
					{
						fprintf( gpsttFd, "copy %s => %s error(%d)\r\n", szSrcFileName, szDstFileName, GetLastError() );
					}
				}
				else if( bResult == FALSE )
				{
					printf( "copy %s => %s error(%d)\r\n", szSrcFileName, szDstFileName, GetLastError() );
				}
			}
		}

WHILE_END:
		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);
}

/**
 * @brief 백업 폴더에 존재하지만 원본 폴더에 존재하지 않는 파일 및 폴더를 삭제한다.
 * @param szSrcName 백업 폴더
 * @param szDstName 원본 폴더
 * @returns 백업 폴더의 파일 개수를 리턴한다.
 */
int DeleteDirectoryIfNotExist( char * szSrcName, char * szDstName )
{
	char			szPath[1024*2];
	char			szSrcFileName[1024*2], szDstFileName[1024*2];
	HANDLE		hSearch;
	BOOL			fisOk = TRUE;
	WIN32_FIND_DATA wfd;
	FILETIME		ft;
	int				iFileCount = 0;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", szSrcName );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch == INVALID_HANDLE_VALUE )
	{
		return 0;
	}
	
	while( fisOk )
	{
		if( !strcmp( wfd.cFileName, "." ) || !strcmp( wfd.cFileName, ".." ) )
		{
			goto WHILE_END;
		}

		_snprintf( szSrcFileName, sizeof(szSrcFileName), "%s\\%s", szSrcName, wfd.cFileName );
		_snprintf( szDstFileName, sizeof(szDstFileName), "%s\\%s", szDstName, wfd.cFileName );

		if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			iFileCount += DeleteDirectoryIfNotExist( szSrcFileName, szDstFileName );
		}
		else
		{
			if( GetFileModifyDate( szDstFileName, &ft ) != 0 )
			{
				// 목적지 폴더에 파일이 존재하지 않으면 소스 폴더의 파일을 삭제한다.
				// 읽기 전용과 같은 경우, 제대로 파일이 복사되지 않는 경우가 
				// 발생할 수 있는 것 같아서 아래와 같이 파일을 삭제한 후, 복사한다.
				SetFileAttributes( szSrcFileName, FILE_ATTRIBUTE_NORMAL );
				DeleteFile( szSrcFileName );

				fprintf( gpsttFd, "delete %s\r\n", szSrcFileName );
			}
			else
			{
				++iFileCount;
			}
		}

WHILE_END:
		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);

	if( iFileCount == 0 )
	{
		RemoveDirectory( szSrcName );
		fprintf( gpsttFd, "delete_folder %s\r\n", szSrcFileName );
	}

	return iFileCount;
}

/**
 * @brief 파일 & 폴더 백업 프로그램
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {source directory} {destination directory}\n", argv[0] );
		return 0;
	}

	char		szDir[1024], szFileName[1024];
	SYSTEMTIME	st;
	
	_snprintf( szDir, sizeof(szDir), "%s\\%s", HGetProgramDirectory(), "Log" );
	HCreateDir( szDir );

	GetLocalTime( &st );
	_snprintf( szFileName, sizeof(szFileName), "%s\\%04d%02d%02d_%02d%02d%02d_%03d.txt"
		, szDir, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

	gpsttFd = fopen( szFileName, "w" );

	CopyDirectory( argv[1], argv[2] );
	DeleteDirectoryIfNotExist( argv[2], argv[1] );

	if( gpsttFd ) fclose( gpsttFd );

	return 0;
}

