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
