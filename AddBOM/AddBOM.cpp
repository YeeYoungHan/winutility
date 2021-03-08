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

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {input filename} {output filename}\n" );
		return 0;
	}

	const char * pszInputFileName = argv[1];
	const char * pszOutputFileName = argv[2];

	FILE * rfd = fopen( pszInputFileName, "rb" );
	if( rfd == NULL )
	{
		printf( "fopen(%s) error\n", pszInputFileName );
		return 0;
	}

	FILE * wfd = fopen( pszOutputFileName, "wb" );
	if( wfd == NULL )
	{
		printf( "fopen(%s) error\n", pszOutputFileName );
		return 0;
	}
	
	unsigned char pszBOM[] = { 0xEF, 0xBB, 0xBF };
	fwrite( pszBOM, 1, sizeof(pszBOM), wfd );

	char szBuf[8192];
	int n;

	while( 1 )
	{
		n = fread( szBuf, 1, sizeof(szBuf), rfd );
		if( n <= 0 ) break;

		fwrite( szBuf, 1, n, wfd );
	}

	fclose( wfd );
	fclose( rfd );

	return 0;
}
