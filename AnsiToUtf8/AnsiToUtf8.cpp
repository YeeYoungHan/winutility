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
 * @ingroup AnsiToUtf8
 * @brief ANSI ���ڿ��� UTF-8 ���ڿ��� ��ȯ�Ѵ�. EUC-KR ���ڿ��� UTF-8 ���ڿ��� ��ȯ�Ѵ�.
 * @param pszAnsi		ANSI ���ڿ�
 * @param strOutput UTF-8 ���ڿ��� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool AnsiToUtf8( const char * pszAnsi, std::string & strOutput )
{
	BSTR    strWide = NULL;
	char*   pszUtf8 = NULL;
	int     iLength;
	int			iAnsiLength = (int)strlen(pszAnsi) + 1;
	
	iLength = MultiByteToWideChar( CP_ACP, 0, pszAnsi, iAnsiLength, NULL, NULL );
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

	MultiByteToWideChar( CP_ACP, 0, pszAnsi, iAnsiLength, strWide, iLength );

	iLength = WideCharToMultiByte( CP_UTF8, 0, strWide, -1, NULL, 0, NULL, NULL );
	if( iLength == 0 )
	{
		SysFreeString( strWide );
		printf( "%s WideCharToMultiByte error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	pszUtf8 = new char[iLength];
	if( pszUtf8 == NULL )
	{
		SysFreeString( strWide );
		printf( "%s new error(%d)", __FUNCTION__, GetLastError() );
		return false;
	}

	WideCharToMultiByte( CP_UTF8, 0, strWide, -1, pszUtf8, iLength, NULL, NULL );
	strOutput = pszUtf8;
	
	SysFreeString( strWide );
	delete [] pszUtf8;
	
	return true;
}

/**
 * @ingroup AnsiToUtf8
 * @brief ANSI ������ UTF8 ���Ϸ� ��ȯ�Ѵ�.
 * @param pszInputFileName	ANSI ���� �̸�
 * @param pszOutputFileName UTF8 ���� �̸�
 * @param iFileSize					ANSI ���� ũ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool AnsiToUtf8( const char * pszInputFileName, const char * pszOutputFileName, int iFileSize )
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
				std::string strUtf8;

				if( AnsiToUtf8( pszIn, strUtf8 ) == false )
				{
					printf( "AnsiToUtf8 error\n" );
				}
				else
				{
					if( fwrite( strUtf8.c_str(), 1, strUtf8.length(), fdDst ) != strUtf8.length() )
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
 * @ingroup AnsiToUtf8
 * @brief ���丮�� ���Ե� ��� ������ ANSI ���� UTF8 �� ��ȯ�Ѵ�.
 * @param pszDirectory	���丮
 * @param pszExt				���� Ȯ����. NULL �� �Է��ϸ� ��� ������ ��ȯ�ϰ� ���� Ȯ���ڸ� �Է��ϸ� �ش� ���� Ȯ������ ���ϸ� ��ȯ�Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool AnsiToUtf8Directory( const char * pszDirectory, const char * pszExt = NULL )
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
				AnsiToUtf8Directory( szFileName, pszExt );
			}
			else
			{
				if( wfd.nFileSizeHigh != 0 || wfd.nFileSizeLow >= 100000000 )
				{
					printf( "%s is too big\n", szFileName );
					break;
				}

				std::string strFileName = szFileName;
				strFileName.append( "_ansitoutf8" );

				if( AnsiToUtf8( szFileName, strFileName.c_str(), wfd.nFileSizeLow ) == false )
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
		AnsiToUtf8Directory( argv[1], argv[2] );
	}
	else
	{
		AnsiToUtf8Directory( argv[1] );
	}

	return 0;
}
