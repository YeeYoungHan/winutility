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

#include "StringMap.h"

#define FAILED_PASSWORD "Failed password for "
#define INVALID_USER "invalid user "

bool Parse( const char * pszBuf, std::string & strUserId, std::string & strIp )
{
	strUserId.clear();
	strIp.clear();

	const char * pszPos = strstr( pszBuf, FAILED_PASSWORD );
	if( pszPos == NULL ) return false;

	pszPos += strlen( FAILED_PASSWORD );
	if( !strncmp( pszPos, INVALID_USER, strlen(INVALID_USER) ) )
	{
		pszPos += strlen(INVALID_USER);
	}

	for( int i = 0; pszPos[i]; ++i )
	{
		if( pszPos[i] == ' ' )
		{
			strUserId.append( pszPos, i );
			pszPos += i + 1;
			break;
		}
	}

	pszPos = strstr( pszPos, "from " );
	if( pszPos == NULL ) return false;

	pszPos += 5;
	for( int i = 0; pszPos[i]; ++i )
	{
		if( pszPos[i] == ' ' )
		{
			strIp.append( pszPos, i );
			break;
		}
	}

	return true;
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {file name}\n", argv[0] );
		return 0;
	}

	const char * pszFileName = argv[1];

	CStringMap clsUserIdMap, clsIpMap;
	std::string strUserId, strIp;
	char szBuf[1024];

	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL )
	{
		printf( "fopen(%s) error\n", pszFileName );
		return 0;
	}

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		if( Parse( szBuf, strUserId, strIp ) )
		{
			clsUserIdMap.Insert( strUserId.c_str() );
			clsIpMap.Insert( strIp.c_str() );
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );

	printf( "Failed User List\n" );
	printf( "==================================\n" );
	clsUserIdMap.Print();


	printf( "Failed IP List\n" );
	printf( "==================================\n" );
	clsIpMap.Print();

	return 0;
}
