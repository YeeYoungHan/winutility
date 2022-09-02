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

#include <winsock2.h>
#include <stdio.h>

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {server ip} {server port}\n", argv[0] );
		return 0;
	}

	char * pszIp = argv[1];
	int iPort = atoi( argv[2] );

	WORD 		wVersionRequested = MAKEWORD( 2, 2 );
	WSADATA wsaData;
	
	if( WSAStartup( wVersionRequested, &wsaData ) != 0 ) 
	{
		printf( "WSAStartup error\n" );
		return 0;
	}

	struct	sockaddr_in	addr;
	SOCKET	fd;
	int			n;
	char		szBuf[8192];

	if( ( fd = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET )
	{
		printf( "socket error\n" );
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);
	addr.sin_addr.s_addr = inet_addr( pszIp );

	if( connect( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		printf( "connect(%s:%d) error\n", pszIp, iPort );
		closesocket( fd );
		return 0;
	}

	while( 1 )
	{
		memset( szBuf, 0, sizeof(szBuf) );
		n = recv( fd, szBuf, sizeof(szBuf)-1, 0 );
		if( n <= 0 ) break;

		printf( "recv[%s]\n", szBuf );
	}

	return 0;
}
