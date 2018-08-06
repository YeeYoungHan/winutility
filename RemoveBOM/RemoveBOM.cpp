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
 * @brief BOM 헤더를 저장하고 있는 파일을 읽어서 BOM 헤더를 제거하여서 저장한다.
 * @param pszInputFileName	[in] BOM 헤더를 저장하고 있는 파일 이름
 * @param pszOutputFileName [out] BOM 헤더가 제거된 파일 이름
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool RemoveBOM( const char * pszInputFileName, const char * pszOutputFileName )
{
	FILE * fd = fopen( pszInputFileName, "rb" );
	if( fd == NULL )
	{
		printf( "input file(%s) open error\n", pszInputFileName );
		return false;
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
			return false;
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
