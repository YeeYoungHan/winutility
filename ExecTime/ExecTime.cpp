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

#include <stdio.h>
#include <windows.h>
#include <string>

int main( int argc, char * argv[] )
{
	if( argc == 1 )
	{
		printf( "[Usage] %s {program}\n", argv[0] );
		return 0;
	}

	std::string strProgram;
	DWORD iStart, iEnd;

	for( int i = 1; i < argc; ++i )
	{
		if( i != 1 )
		{
			strProgram.append( " " );
		}

		strProgram.append( argv[i] );
	}

	iStart = GetTickCount();

	system( strProgram.c_str() );

	iEnd = GetTickCount();

	int iRes = iEnd - iStart;

	printf( "execution time is %u ms\n", iRes );

	return 0;
}
