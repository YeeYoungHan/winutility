#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {input filename} {output filename}\n" );
		return 0;
	}

	const char * pszInputFileName = argv[1];
	const char * pszOutputFileName = argv[2];

	FILE * rfd = fopen( pszInputFileName, "rb" );
	if( rfd == NULL )
	{
		printf( "fopen(%s) error\n", pszInputFileName );
		return 0;
	}

	FILE * wfd = fopen( pszOutputFileName, "wb" );
	if( wfd == NULL )
	{
		printf( "fopen(%s) error\n", pszOutputFileName );
		return 0;
	}
	
	unsigned char pszBOM[] = { 0xEF, 0xBB, 0xBF };
	fwrite( pszBOM, 1, sizeof(pszBOM), wfd );

	char szBuf[8192];
	int n;

	while( 1 )
	{
		n = fread( szBuf, 1, sizeof(szBuf), rfd );
		if( n <= 0 ) break;

		fwrite( szBuf, 1, n, wfd );
	}

	fclose( wfd );
	fclose( rfd );

	return 0;
}
