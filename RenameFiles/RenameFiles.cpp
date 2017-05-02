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

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {folder path} {old name} {new name}\n" );
		return 0;
	}

	const char * pszFolderPath = argv[1];
	const char * pszOldName = argv[2];
	const char * pszNewName = argv[3];

	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszFolderPath;

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

		if( strstr( sttFindData.cFileName, pszOldName ) )
		{
			std::string strFileName = sttFindData.cFileName;
			ReplaceString( strFileName, pszOldName, pszNewName );

			std::string strOldFileName = pszFolderPath;
			strOldFileName.append( "\\" );
			strOldFileName.append( sttFindData.cFileName );

			std::string strNewFileName = pszFolderPath;
			strNewFileName.append( "\\" );
			strNewFileName.append( strFileName );

			MoveFile( strOldFileName.c_str(), strNewFileName.c_str() );
		}
	}

	FindClose( hFind );

	return 0;
}