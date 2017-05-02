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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

#define	PROGRAM_VERSION	"0.4"

int		fisDebug = 0;

#ifdef WIN32

#define snprintf _snprintf

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define OPTERRCOLON (1)
#define OPTERRNF (2)
#define OPTERRARG (3)

char *optarg;
int optreset = 0;
int optind = 1;
int opterr = 1;
int optopt;

static int optiserr(int argc, char * const *argv, int oint, const char *optstr, int optchr, int err)
{
  if(opterr)
  {
    fprintf(stderr, "Error in argument %d, char %d: ", oint, optchr+1);
    switch(err)
    {
    case OPTERRCOLON:
      fprintf(stderr, ": in flags\n");
      break;
    case OPTERRNF:
      fprintf(stderr, "option not found %c\n", argv[oint][optchr]);
      break;
    case OPTERRARG:
      fprintf(stderr, "no argument for option %c\n", argv[oint][optchr]);
      break;
    default:
      fprintf(stderr, "unknown\n");
      break;
    }
  }
  optopt = argv[oint][optchr];
  return('?');
}

int getopt( int argc, char* const *argv, const char *optstr )
{
  static int optchr = 0;
  static int dash = 0; /* have already seen the - */

  const char *cp;

  if (optreset)
    optreset = optchr = dash = 0;
  if(optind >= argc)
    return(EOF);
  if(!dash && (argv[optind][0] !=  '-'))
    return(EOF);
  if(!dash && (argv[optind][0] ==  '-') && !argv[optind][1])
  {
    /*
     * use to specify stdin. Need to let pgm process this and
     * the following args
     */
    return(EOF);
  }
  if( (argv[optind][0] == '-') && (argv[optind][1] == '-') )
  {
    /* -- indicates end of args */
    optind++;
    return(EOF);
  }

  if( !dash )
  {
    assert((argv[optind][0] == '-') && argv[optind][1]);
    dash = 1;
    optchr = 1;
  }

  // Check if the guy tries to do a -: kind of flag
  assert(dash);
  if( argv[optind][optchr] == ':' )
  {
    dash = 0;
    optind++;
    return( optiserr(argc, argv, optind-1, optstr, optchr, OPTERRCOLON) );
  }

  if( !(cp = strchr( optstr, argv[optind][optchr] ) ) )
  {
    int errind = optind;
    int errchr = optchr;

    if( !argv[optind][optchr+1] )
    {
      dash = 0;
      optind++;
    }
    else
      optchr++;
    return( optiserr( argc, argv, errind, optstr, errchr, OPTERRNF ) );
  }

  if( cp[1] == ':' )
  {
    dash = 0;
    optind++;
    if( optind == argc )
      return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRARG));
    optarg = argv[optind++];
    return(*cp);
  }
  else
  {
    if( !argv[optind][optchr+1] )
    {
      dash = 0;
      optind++;
    }
    else
      optchr++;
    return(*cp);
  }

  assert(0);
  return(0);
}

#endif

/** 사용법을 출력한다. */
void PrintUsage( char * szProgramName = NULL )
{
	printf( "[Usage] %s {-f filename}\n", szProgramName?szProgramName:"tail" );
	printf( "\t\t{-g search word}\n" );
	printf( "\t\t{-h} : show help message\n" );
	printf( "\t\t{-v} : show program version\n" );
	printf( "\t\t{-t} : search file from start position\n" );

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

	if( fisDebug == 1 ) 
	{
		printf( "arg = [%s]\n", szArg );
	}

	return 0;
}

/* 주 함수 */

int main( int argc, char * argv[] )
{
	char	fisPrintUsage = 0;
	char	fisTotal = 0;
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
			fisPrintUsage = 1;
			break;
		case 'f':
			snprintf( szFileName, sizeof(szFileName), "%s", optarg );
			break;
		case 'g':
			snprintf( szWord, sizeof(szWord), "%s", optarg );
			break;
		case 't':
			fisTotal = 1;
			break;
		default:
			fisPrintUsage = 1;
			break;
		}
	}

	if( strlen( szFileName ) == 0 ) fisPrintUsage = 1;

	if( fisPrintUsage == 1 )
	{
		PrintUsage( argv[0] );
	}

	CheckArg( szFileName );
	CheckArg( szWord );

	FILE			* fd;
	char			szBuf[1024];
	int				iFileSize = 0;
	int				i;
	struct _stat		clsStat;

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
	if( fisTotal == 0 && iFileSize > 1024 )
	{
		fseek( fd, iFileSize - 1024, SEEK_SET );
	}

	// 주기적으로 파일의 입력을 검사하여서 입력된 경우에는 출력한다.
	while(1)
	{
		memset( szBuf, 0, sizeof(szBuf) );
		while( fgets( szBuf, sizeof(szBuf), fd ) )
		{
			//printf( "%s", szBuf );
			if( szWord[0] != '\0' && strstr( szBuf, szWord ) == NULL ) continue;

			for( i = 0; i < sizeof(szBuf); i++ )
			{
				if( szBuf[i] == 0 ) break;
				
				if( szBuf[i] > 126 || ( szBuf[i] != '\r' && szBuf[i] != '\n' && szBuf[i] < 32 ) )
				{
					printf( "{undefined character}\n" );
					break;
				}
				else
				{
					printf( "%c", szBuf[i] );
				}
			}

			memset( szBuf, 0, sizeof(szBuf) );
		}

		Sleep( 1000 );
	}

	fclose( fd );

	return 0;
}