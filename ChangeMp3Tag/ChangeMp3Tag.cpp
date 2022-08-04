#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct Mp3Tag
{
	char szTag[3];
	char szTitle[30];
	char szArtist[30];
	char szAlbum[30];
	char szYear[4];
	char szComment[30];
	char szGenre[1];
};

int main( int argc, char * argv[] )
{
	if( argc <= 3 )
	{
		printf( "%s {input mp3} {output mp3} {title} {artist} {album}\n", argv[0] );
		return 0;
	}

	const char * pszInputFile = argv[1];
	const char * pszOutputFile = argv[2];
	const char * pszTitle = NULL;
	const char * pszArtist = NULL;
	const char * pszAlbum = NULL;
	long iInputFileSize = 0;
	char * pszBuf = NULL;

	if( argc >= 4 ) pszTitle = argv[3];
	if( argc >= 5 ) pszArtist = argv[4];
	if( argc >= 6 ) pszAlbum = argv[5];

	FILE * in = fopen( pszInputFile, "rb" );
	if( in == NULL )
	{
		printf( "input mp3(%s) open error\n", pszInputFile );
	}
	else
	{
		struct stat sttStat;

		if( fstat( _fileno( in ), &sttStat ) == -1 )
		{
			printf( "input mp3(%s) fstat error\n", pszInputFile );
		}
		else
		{
			iInputFileSize = sttStat.st_size;

			pszBuf = (char *)malloc( iInputFileSize );
			if( pszBuf == NULL )
			{
				printf( "malloc error\n" );
			}
			else
			{
				if( fread( pszBuf, 1, iInputFileSize, in ) != iInputFileSize )
				{
					printf( "input mp3(%s) read error\n", pszInputFile );
				}
			}
		}

		fclose( in );
	}

	if( pszBuf == NULL ) return 0;

	FILE * out = fopen( pszOutputFile, "wb" );
	if( out == NULL )
	{
		printf( "output mp3(%s) open error\n", pszOutputFile );
	}
	else
	{
		Mp3Tag sttTag;

		if( !strncmp( pszBuf + iInputFileSize - 128, "TAG", 3 ) )
		{
			fwrite( pszBuf, 1, iInputFileSize - 128, out );
			memcpy( &sttTag, pszBuf + iInputFileSize - 128, 128 );
		}
		else
		{
			fwrite( pszBuf, 1, iInputFileSize, out );

			memset( &sttTag, 0, sizeof(sttTag) );
			sprintf( sttTag.szTag, "TAG" );
		}

		if( pszTitle ) _snprintf( sttTag.szTitle, sizeof(sttTag.szTitle), "%s", pszTitle );
		if( pszArtist ) _snprintf( sttTag.szArtist, sizeof(sttTag.szArtist), "%s", pszArtist );
		if( pszAlbum ) _snprintf( sttTag.szAlbum, sizeof(sttTag.szAlbum), "%s", pszAlbum );

		fwrite( &sttTag, sizeof(sttTag), 1, out );
		
		fclose( out );
	}

	free( pszBuf );

	return 0;
}
