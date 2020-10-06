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

#include "Tail.h"

bool gbDebug = false;

/** 사용법을 출력한다. */
void PrintUsage( char * szProgramName = NULL )
{
	printf( "[Usage] %s {-f filename}\n", szProgramName?szProgramName:"tail" );
	printf( "\t\t{-g search word}\n" );
	printf( "\t\t{-h} : show help message\n" );
	printf( "\t\t{-v} : show program version\n" );
	printf( "\t\t{-t} : search file from start position\n" );
	printf( "\t\t{-u} : Converted to utf8 string and output\n" );

	exit(1);
}

/** 입력 문자열 중에서 ' 와 ' 로 구분되는 문자열이면 ' 를 제거하여 준다.
 *	입력 문자열 중에서 ' 와 ' 로 구분되는 문자열이면 ' 를 제거하여 준다.
 *
 *	@return	0 을 리턴한다.
 */
int CheckArg( char * szArg )
{
	int		i, iLen;

	iLen = (int)strlen( szArg );

	if( iLen >= 2 )
	{
		if( ( szArg[0] == '"' && szArg[iLen-1] == '"' ) || 
			( szArg[0] == '\'' && szArg[iLen-1] == '\'' ) )
		{
			for( i = 1; i < (iLen - 1); i++ )
			{
				szArg[i-1] = szArg[i];
			}

			szArg[iLen-2] = '\0';
		}
	}

	if( gbDebug ) 
	{
		printf( "arg = [%s]\n", szArg );
	}

	return 0;
}

/* 주 함수 */

int main( int argc, char * argv[] )
{
	bool	bUseUtf8 = false;
	bool	bUseTotal = false;
	char	szWord[1024], szFileName[1024];
	int		iOpt;

	memset( szFileName, 0, sizeof(szFileName) );
	memset( szWord, 0, sizeof(szWord) );

	// 사용자 입력 인자를 분석한다.
	while( (iOpt = getopt( argc, argv, "vhf:g:t" )) != -1 ) 
	{
		switch (iOpt) 
		{
		case 'v':
			printf( "%s [ver - %s]\n", argv[0], PROGRAM_VERSION );
			return 1;
			break;
		case 'h':
			PrintUsage( argv[0] );
			break;
		case 'f':
			snprintf( szFileName, sizeof(szFileName), "%s", optarg );
			break;
		case 'g':
			snprintf( szWord, sizeof(szWord), "%s", optarg );
			break;
		case 't':
			bUseTotal = true;
			break;
		case 'u':
			bUseUtf8 = true;
			break;
		default:
			PrintUsage( argv[0] );
			break;
		}
	}

	if( strlen( szFileName ) == 0 ) PrintUsage( argv[0] );

	CheckArg( szFileName );
	CheckArg( szWord );

	FILE			* fd;
	char			szBuf[1024];
	int				iFileSize = 0;
	struct _stat		clsStat;
	std::string strBuf;

	if( _stat( szFileName, &clsStat ) != 0 )
	{
		printf( "file [%s] stat error(%d)\n", argv[1], errno );
		return -1;
	}
	else
	{
		iFileSize = clsStat.st_size;
	}

	if( (fd = fopen( szFileName, "r" )) == NULL )
	{
		printf( "file [%s] open error(%d)\n", argv[1], errno );
		return -1;
	}

	// 파일 크기가 1 K 보다 큰 경우에는 파일의 끝에서 1 K 부터 보여준다.
	if( bUseTotal == false && iFileSize > 1024 )
	{
		fseek( fd, iFileSize - 1024, SEEK_SET );
	}

	// 주기적으로 파일의 입력을 검사하여서 입력된 경우에는 출력한다.
	while(1)
	{
		memset( szBuf, 0, sizeof(szBuf) );
		while( fgets( szBuf, sizeof(szBuf), fd ) )
		{
			if( szWord[0] != '\0' && strstr( szBuf, szWord ) == NULL ) continue;

			if( bUseUtf8 )
			{
				Utf8ToAnsi( szBuf, strBuf );
				printf( "%s", strBuf.c_str() );
			}
			else
			{
				printf( "%s", szBuf );
			}

			memset( szBuf, 0, sizeof(szBuf) );
		}

		Sleep( 1000 );
	}

	fclose( fd );

	return 0;
}