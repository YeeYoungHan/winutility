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
#include <sys/stat.h>

// BOM 파일인지 검사한다.
// 참고자료 : https://ko.wikipedia.org/wiki/%EB%B0%94%EC%9D%B4%ED%8A%B8_%EC%88%9C%EC%84%9C_%ED%91%9C%EC%8B%9D
int GetBOMLen( unsigned char * pszBuf, int iReadSize )
{
	int		iBOMLen = 0;

	if( iReadSize >= 2 )
	{
		if( pszBuf[0] == 0xFE && pszBuf[1] == 0xFF )
		{
			// UTF-16 (BE)
			iBOMLen = 2;
		}
		else if( pszBuf[0] == 0xFF && pszBuf[1] == 0xFE )
		{
			// UTF-16 (LE)
			iBOMLen = 2;
		}
	}

	if( iBOMLen == 0 && iReadSize >= 3 )
	{
		if( pszBuf[0] == 0xEF && pszBuf[1] == 0xBB && pszBuf[2] == 0xBF )
		{
			// UTF-8
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0xF7 && pszBuf[1] == 0x64 && pszBuf[2] == 0x4C )
		{
			// UTF-1
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0x0E && pszBuf[1] == 0xFE && pszBuf[2] == 0xFF )
		{
			// SCSU
			iBOMLen = 3;
		}
		else if( pszBuf[0] == 0xFB && pszBuf[1] == 0xEE && pszBuf[2] == 0x28 )
		{
			// BOCU-1
			iBOMLen = 3;
		}
	}

	if( iBOMLen == 0 && iReadSize >= 4 )
	{
		if( pszBuf[0] == 0x00 && pszBuf[1] == 0x00 && pszBuf[2] == 0xFE && pszBuf[3] == 0xFF )
		{
			// UTF-32 (BE)
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0xFF && pszBuf[1] == 0xFE && pszBuf[2] == 0x00 && pszBuf[3] == 0x00 )
		{
			// UTF-32 (LE)
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0x2B && pszBuf[1] == 0x2F && pszBuf[2] == 0x76 )
		{
			// UTF-7
			if( pszBuf[3] == 0x38 )
			{
				iBOMLen = 4;
				if( iReadSize == 5 && pszBuf[4] == 0x2D )
				{
					iBOMLen = 5;
				}
			}
			else if( pszBuf[3] == 0x39 || pszBuf[3] == 0x2B || pszBuf[3] == 0x2F )
			{
				iBOMLen = 4;
			}
		}
		else if( pszBuf[0] == 0xDD && pszBuf[1] == 0x73 && pszBuf[2] == 0x66 && pszBuf[3] == 0x73 )
		{
			// UTF-EBCDIC
			iBOMLen = 4;
		}
		else if( pszBuf[0] == 0x84 && pszBuf[1] == 0x31 && pszBuf[2] == 0x95 && pszBuf[3] == 0x33 )
		{
			// GB-18030
			iBOMLen = 4;
		}
	}

	return iBOMLen;
}

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {input filename} {output filename}\n", argv[0] );
		return 0;
	}

	const char * pszInputFileName = argv[1];
	const char * pszOutputFileName = argv[2];

	FILE * fd = fopen( pszInputFileName, "rb" );
	if( fd == NULL )
	{
		printf( "input file(%s) open error\n", pszInputFileName );
		return 0;
	}

	unsigned char	szBuf[5];
	int		iReadSize;
	int		iBOMLen = 0;

	iReadSize = fread( szBuf, 1, 5, fd );
	iBOMLen = GetBOMLen( szBuf, iReadSize );

	unsigned char * pszBuf = NULL;
	unsigned int iBufSize = 0;

	if( iBOMLen == 0 )
	{
		printf( "input file(%s) is not BOM file\n", pszInputFileName );
	}
	else
	{
		struct stat sttStat;

		if( stat( pszInputFileName, &sttStat ) == -1 )
		{
			printf( "input file(%s) size error\n", pszInputFileName );
		}
		else
		{
			iBufSize = sttStat.st_size - iBOMLen;

			pszBuf = (unsigned char *)malloc( iBufSize );
			if( pszBuf == NULL )
			{
				printf( "malloc error\n" );
			}
			else if( fseek( fd, iBOMLen, SEEK_SET ) != 0 || fread( pszBuf, 1, iBufSize, fd ) != iBufSize )
			{
				printf( "fseek or fread error\n" );
				free( pszBuf );
				pszBuf = NULL;
			}
		}
	}

	fclose( fd );

	if( pszBuf )
	{
		fd = fopen( pszOutputFileName, "wb" );
		if( fd == NULL )
		{
			printf( "output file(%s) open error\n", pszOutputFileName );
			return 0;
		}

		if( fwrite( pszBuf, 1, iBufSize, fd ) != iBufSize )
		{
			printf( "fwrite error\n" );
		}

		fclose( fd );
	}

	if( pszBuf ) free( pszBuf );

	return 0;
}
