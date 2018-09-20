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
#include <stdlib.h>

typedef unsigned __int64 uint64_t;

int main( int argc, char * argv[] )
{
	if( argc < 4 )
	{
		printf( "[Usage] %s {src file} {dest file} {start pos} {end pos}\n", argv[0] );
		return 0;
	}

	const char * pszSrcFile = argv[1];
	const char * pszDstFile = argv[2];
	uint64_t iStartPos = _strtoui64( argv[3], NULL, 10 );
	uint64_t iEndPos = 0;

	if( argc >= 5 )
	{
		iEndPos = _strtoui64( argv[4], NULL, 10 );
	}

	FILE * fdSrc = fopen( pszSrcFile, "rb" );
	if( fdSrc == NULL )
	{
		printf( "src file(%s) open error\n", pszSrcFile );
	}
	else
	{
		FILE * fdDst = fopen( pszDstFile, "wb" );
		if( fdDst == NULL )
		{
			printf( "dest file(%s) open error\n", pszDstFile );
		}
		else
		{
			if( _fseeki64( fdSrc, iStartPos, SEEK_SET ) != 0 )
			{
				printf( "seek error\n" );
			}
			else
			{
				char szBuf[8192];
				int iRead, iWant;
				uint64_t iPos = iStartPos;

				while( 1 )
				{
					if( iEndPos > 0 )
					{
						if( iPos + sizeof(szBuf) < iEndPos )
						{
							iWant = sizeof(szBuf);
						}
						else
						{
							iWant = (int)(iEndPos - iPos);
						}
					}
					else
					{
						iWant = sizeof(szBuf);
					}

					iRead = fread( szBuf, 1, iWant, fdSrc );
					if( iRead <= 0 ) break;

					fwrite( szBuf, 1, iRead, fdDst );
					iPos += iRead;
				}
			}

			fclose( fdDst );
		}

		fclose( fdSrc );
	}

	return 0;
}
