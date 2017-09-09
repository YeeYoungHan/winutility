#include <stdio.h>
#include <windows.h>
#include <string>

typedef  unsigned __int64 uint64_t;

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {disk}\n", argv[0] );
		printf( "        %s d:\\\n" );
		return 0;
	}

	std::string strFileName = argv[1];
	strFileName.append( "cleandisk.txt" );

	HANDLE	hFile;
	char		szBuf[8192];
	DWORD		dwWriteWantLen, dwWriteLen;

	memset( szBuf, 0, sizeof(szBuf) );

	hFile = CreateFile( strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		printf( "file(%s) open error(%d)\n", strFileName.c_str(), GetLastError() );
		return 0;
	}

	while( 1 )
	{
		dwWriteWantLen = sizeof(szBuf);

		if( WriteFile( hFile, szBuf, dwWriteWantLen, &dwWriteLen, NULL ) == FALSE )
		{
			break;
		}
	}

	CloseHandle( hFile );

	DeleteFile( strFileName.c_str() );

	return 0;
}
