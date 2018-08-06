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

#include "RemoveBOM.h"

/**
 * @ingroup RemoveBOM
 * @brief ������ �����ϴ� ��� ������ BOM ����� �����Ѵ�.
 * @param pszDirectory ���� ��� ( full path )
 */
void DirectoryRemoveBOM( const char * pszDirectory )
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
				DirectoryRemoveBOM( szFileName );
			}
			else
			{
				RemoveBOM( szFileName, szFileName );
			}
		}

		fisOk = FindNextFile( hSearch, &wfd );
	}

	FindClose(hSearch);
}

/**
 * @ingroup RemoveBOM
 * @brief BOM ������ �����Ѵ�.
 * @param pszFileName BOM ���� �̸� ( full path )
 * @param iBOMType		1 �̸� UTF-8 Ÿ���� BOM ������ �����Ѵ�.
 */
void MakeBOMFile( const char * pszFileName, int iBOMType )
{
	FILE * fd = fopen( pszFileName, "wb" );
	if( fd == NULL )
	{
		printf( "fopen(%s) error\n", pszFileName );
		return;
	}
	
	if( iBOMType == 1 )
	{
		unsigned char pszBOM[] = { 0xEF, 0xBB, 0xBF };
		fwrite( pszBOM, 1, sizeof(pszBOM), fd );
	}

	fwrite( pszFileName, 1, strlen(pszFileName), fd );

	fclose( fd );
}

/**
 * @ingroup RemoveBOM
 * @brief ������ BOM ���ϵ��� �����Ѵ�.
 * @param pszDirectory BOM ���ϵ��� ������ ���� ���
 */
void MakeBOMFiles( const char * pszDirectory )
{
	char			szFileName[2048];

	_snprintf( szFileName, sizeof(szFileName), "%s\\1.txt", pszDirectory );
	MakeBOMFile( szFileName, 1 );

	_snprintf( szFileName, sizeof(szFileName), "%s\\2.txt", pszDirectory );
	MakeBOMFile( szFileName, 1 );
}

/**
 * @ingroup RemoveBOM
 * @brief main �Լ�
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {input filename} {output filename}\n", argv[0] );
		printf( "[Usage] %s -d {input folder}\n", argv[0] );

#ifdef _DEBUG
		printf( "[Usage] %s -t {input folder}\n", argv[0] );
#endif
		return 0;
	}

	const char * pszInputFileName = argv[1];
	const char * pszOutputFileName = argv[2];

	if( !strcmp( pszInputFileName, "-d" ) )
	{
		DirectoryRemoveBOM( pszOutputFileName );
	}
#ifdef _DEBUG
	else if( !strcmp( pszInputFileName, "-t" ) )
	{
		// �׽�Ʈ�� BOM ������ �����Ѵ�.
		MakeBOMFiles( pszOutputFileName );
	}
#endif
	else
	{
		RemoveBOM( pszInputFileName, pszOutputFileName );
	}

	return 0;
}