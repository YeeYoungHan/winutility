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
#include <string.h>

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {first file path} {second file path}\n", argv[0] );
		return 0;
	}

	const char * pszFirstFilePath = argv[1];
	const char * pszSecondFilePath = argv[2];

	char szBuf1[8192], szBuf2[8192];
	int n1, n2;

	FILE * fd1 = fopen( pszFirstFilePath, "rb" );
	if( fd1 == NULL )
	{
		printf( "first file(%s) open error\n" );
	}
	else
	{
		FILE * fd2 = fopen( pszSecondFilePath, "rb" );
		if( fd2 == NULL )
		{
			printf( "second file(%s) open error\n" );
		}
		else
		{
			bool bError = false;

			while( 1 )
			{
				n1 = fread( szBuf1, 1, sizeof(szBuf1), fd1 );
				n2 = fread( szBuf2, 1, sizeof(szBuf2), fd2 );

				if( n1 != n2 )
				{
					bError = true;
					break;
				}

				if( n1 <= 0 ) break;

				if( memcmp( szBuf1, szBuf2, n1 ) )
				{
					bError = true;
					break;
				}
			}

			fclose( fd2 );

			if( bError == false )
			{
				printf( "Same file\n" );
			}
			else
			{
				printf( "files %s and %s differ\n", pszFirstFilePath, pszSecondFilePath );
			}
		}

		fclose( fd1 );
	}

	return 0;
}
