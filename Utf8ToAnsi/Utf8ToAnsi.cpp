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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/**
 * @ingroup Utf8ToAnsi
 * @brief UTF8 문자열을 ANSI 문자열로 변환한다.
 * @param pszUtf8		UTF8 문자열 (input)
 * @param strOutput ANSI 문자열 (output)
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
		printf( "%s MultiByteToWideChar error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	strWide = SysAllocStringLen( NULL, iLength );
	if( strWide == NULL )
	{
		printf( "%s SysAllocStringLen error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	MultiByteToWideChar( CP_UTF8, 0, pszUtf8, iUtf8Length, strWide, iLength );

	iLength = WideCharToMultiByte( CP_ACP, 0, strWide, -1, NULL, 0, NULL, NULL );
	if( iLength == 0 )
	{
		SysFreeString( strWide );
		printf( "%s WideCharToMultiByte error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	pszAnsi = new char[iLength];
	if( pszAnsi == NULL )
	{
		SysFreeString( strWide );
		printf( "%s new error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	WideCharToMultiByte( CP_ACP, 0, strWide, -1, pszAnsi, iLength, NULL, NULL );
	strOutput = pszAnsi;
	
	SysFreeString( strWide );
	delete [] pszAnsi;
	
	return true;
}

/**
 * @ingroup Utf8ToAnsi
 * @brief UTF8 파일을 ANSI 파일로 변환한다.
 * @param pszInputFileName	UTF8 파일 이름
 * @param pszOutputFileName ANSI 파일 이름
 * @param iFileSize					UTF8 파일 크기
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool Utf8ToAnsi( const char * pszInputFileName, const char * pszOutputFileName, int iFileSize )
{
	bool bRes = false;

	char * pszIn = (char *)malloc( iFileSize + 1 );
	if( pszIn == NULL )
	{
		return false;
	}

	pszIn[iFileSize] = '\0';

	FILE * fdSrc = fopen( pszInputFileName, "rb" );
	if( fdSrc == NULL )
	{
		printf( "src file(%s) open error\n", pszInputFileName );
	}
	else
	{
		FILE * fdDst = fopen( pszOutputFileName, "wb" );
		if( fdDst == NULL )
		{
			printf( "dest file(%s) open error\n", pszOutputFileName );
		}
		else
		{
			if( fread( pszIn, 1, iFileSize, fdSrc ) != iFileSize )
			{
				printf( "src file(%s) read error\n", pszInputFileName );
			}
			else
			{
				std::string strAnsi;

				if( Utf8ToAnsi( pszIn, strAnsi ) == false )
				{
					printf( "Utf8ToAnsi error\n" );
				}
				else
				{
					if( fwrite( strAnsi.c_str(), 1, strAnsi.length(), fdDst ) != strAnsi.length() )
					{
						printf( "dst file(%s) write error\n", pszOutputFileName );
					}
					else
					{
						bRes = true;
					}
				}
			}

			fclose( fdDst );

			if( bRes == false )
			{
				::DeleteFile( pszOutputFileName );
			}
		}

		fclose( fdSrc );
	}

	free( pszIn );

	return bRes;
}

/**
 * @ingroup Utf8ToAnsi
 * @brief 디렉토리에 포함된 모든 파일을 UTF8 에서 ANSI 로 변환한다.
 * @param pszDirectory	디렉토리
 * @param pszExt				파일 확장자. NULL 을 입력하면 모든 파일을 변환하고 파일 확장자를 입력하면 해당 파일 확장자인 파일만 변환한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool Utf8ToAnsiDirectory( const char * pszDirectory, const char * pszExt = NULL )
{
	char			szPath[2048];
	char			szFileName[2048];
	HANDLE		hSearch;
	BOOL			bOk = TRUE;
	WIN32_FIND_DATA wfd;

	if( pszExt )
	{
		_snprintf( szPath, sizeof(szPath), "%s\\*.%s", pszDirectory, pszExt );
	}
	else
	{
		_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszDirectory );
	}

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
				Utf8ToAnsiDirectory( szFileName );
			}
			else
			{
				if( wfd.nFileSizeHigh != 0 || wfd.nFileSizeLow >= 100000000 )
				{
					printf( "%s is too big\n", szFileName );
					break;
				}

				std::string strFileName = szFileName;
				strFileName.append( "_utf8toansi" );

				if( Utf8ToAnsi( szFileName, strFileName.c_str(), wfd.nFileSizeLow ) == false )
				{
					break;
				}

				::DeleteFile( szFileName );
				::MoveFile( strFileName.c_str(), szFileName );
			}
		}

		bOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);

	return true;
}

int main( int argc, char * argv[] )
{
	if( argc == 1 )
	{
		printf( "[Usage] %s {folder full path} {file ext}\n", argv[0] );
		return 0;
	}

	if( argc == 3 )
	{
		Utf8ToAnsiDirectory( argv[1], argv[2] );
	}
	else
	{
		Utf8ToAnsiDirectory( argv[1] );
	}

	return 0;
}
