#include <stdio.h>
#include <windows.h>

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {filename}\n", argv[0] );
		return 0;
	}

	::ShellExecute( NULL, "print", argv[1], NULL, NULL, SW_SHOW );

	return 0;
}
