#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

void PrintString()
{
	char szTemp[255];

	for( int i = 0; i < 1000000000; ++i )
	{
		_snprintf( szTemp, sizeof(szTemp), "%d", i );
	}
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {loop count}\n", argv[0] );
		return 0;
	}

	int iLoop = atoi( argv[1] );

	for( int i = 0; i < iLoop; ++i )
	{
		PrintString( );
	}

	return 0;
}
