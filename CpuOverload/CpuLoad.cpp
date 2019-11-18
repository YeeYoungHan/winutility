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
	PrintString( );

	return 0;
}
