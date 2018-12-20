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
#include <tlhelp32.h>
#include <psapi.h>

#pragma comment( lib, "psapi" )

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {filename}\n", argv[0] );
		return 0;
	}

	const char * pszExeName = argv[1];

	HANDLE hSnap;
  PROCESSENTRY32 sttPE;

	hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hSnap == INVALID_HANDLE_VALUE )
	{
		printf( "CreateToolhelp32Snapshot error\n" );
		return 0;
	}

	sttPE.dwSize = sizeof( PROCESSENTRY32 );

	if( !Process32First( hSnap, &sttPE ) )
	{
		printf( "Process32First error\n" );
	}
	else
	{
		do
		{
			if( !_stricmp( sttPE.szExeFile, pszExeName ) )
			{
				HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, sttPE.th32ProcessID );
				if( hProcess )
				{
					TerminateProcess( hProcess, 0 );
					CloseHandle( hProcess );
				}
			}
		}
		while( Process32Next( hSnap, &sttPE ) );
	}

	CloseHandle( hSnap );

	return 0;
}
