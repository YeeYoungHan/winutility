#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int giLoopCount = 1;

void PrintString()
{
	char szTemp[255];

	for( int i = 0; i < 1000000000; ++i )
	{
		_snprintf( szTemp, sizeof(szTemp), "%d", i );
	}
}

DWORD WINAPI LoopThread( LPVOID lpParameter )
{
	for( int i = 0; i < giLoopCount; ++i )
	{
		PrintString();
	}

	return 0;
}

bool StartThread( const char * pszName, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter )
{
	DWORD		dwThreadId;
	HANDLE	hThread;

	hThread = CreateThread( NULL, 0, lpStartAddress, lpParameter, 0, &dwThreadId );
	if( hThread == NULL )
	{
		return false;
	}

	CloseHandle( hThread );
	
	return true;
}

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {thread count} {loop count}\n", argv[0] );
		return 0;
	}

	int iThreadCount = atoi( argv[1] );
	giLoopCount = atoi( argv[2] );

	for( int i = 0; i < iThreadCount; ++i )
	{
		StartThread( "LoopThread", LoopThread, NULL );
	}

	char c;

	while( 1 )
	{
		scanf( "%c", &c );

		if( c == 'x' || c == 'q' ) break;
	}

	return 0;
}
