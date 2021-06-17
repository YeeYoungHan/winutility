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

#include <windows.h>
#include <string>

void ReplaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );
	size_t iAfterLen = strlen( pszAfter );

	while( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		iPos = strCallId.find( pszBefore, iPos + iAfterLen );
	}
}

bool GetFileExt( const char * pszFilePath, std::string & strExt )
{
	strExt.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

	if( pszFilePath[iLen-1] == '\\' ) return false;

	for( int i = iLen - 2; i >= 0; --i )
	{
		if( pszFilePath[i] == '\\' )
		{
			break;
		}

		if( pszFilePath[i] == '.' )
		{
			strExt.append( pszFilePath + i + 1 );
			return true;
		}
	}

	return false;
}

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {folder path} {old name} {new name}\n", argv[0] );
		printf( "[Usage] %s {folder path} * {n}\n", argv[0] );
		return 0;
	}

	const char * pszFolderPath = argv[1];
	const char * pszOldName = argv[2];
	const char * pszNewName = argv[3];
	bool bChangeNum = false;

	if( !strcmp( pszOldName, "*" ) & !strcmp( pszNewName, "{n}" ) )
	{
		bChangeNum = true;
	}

	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszFolderPath;
	std::string strFileName, strOldFileName, strNewFileName, strExt;
	char				szFileName[255];
	int					iIndex = 0;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		printf( "FindFirstFile(%s) error(%d)", pszFolderPath, GetLastError() );
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;
		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;

		if( bChangeNum )
		{
			if( GetFileExt( sttFindData.cFileName, strExt ) )
			{
				++iIndex;
				_snprintf( szFileName, sizeof(szFileName), "%d.%s", iIndex, strExt.c_str() );

				strOldFileName = pszFolderPath;
				strOldFileName.append( "\\" );
				strOldFileName.append( sttFindData.cFileName );

				strNewFileName = pszFolderPath;
				strNewFileName.append( "\\" );
				strNewFileName.append( szFileName );

				MoveFile( strOldFileName.c_str(), strNewFileName.c_str() );
			}
		}
		else if( strstr( sttFindData.cFileName, pszOldName ) )
		{
			strFileName = sttFindData.cFileName;
			ReplaceString( strFileName, pszOldName, pszNewName );

			strOldFileName = pszFolderPath;
			strOldFileName.append( "\\" );
			strOldFileName.append( sttFindData.cFileName );

			strNewFileName = pszFolderPath;
			strNewFileName.append( "\\" );
			strNewFileName.append( strFileName );

			MoveFile( strOldFileName.c_str(), strNewFileName.c_str() );
		}
	}

	FindClose( hFind );

	return 0;
}