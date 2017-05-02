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

/** get full path of program binary code
 *
 *  @author Yee Young Han 
 *  @return full path of program binary code
 */
char * HGetProgramDirectory( )
{
	static	char	szDir[1024*2];

	if( strlen(szDir) == 0 )
	{
		int		i;
		HMODULE	hThis;

		hThis = GetModuleHandle( NULL );

		GetModuleFileName( hThis, szDir, sizeof(szDir));
		for( i = strlen( szDir) - 1; i >= 0; i-- )
		{
			if( szDir[i] == '\\' ) 
			{
				szDir[i] = '\0';
				break;
			}
		}
	}

	return szDir;
}

/* create directory
 * 
 *  @author Yee Young Han 
 *  @param 	szDirName	directory fullpath
 *	@return	if success, return 0.
 *			otherwise return -1.
 */
static int _HCreateDir( char * szDirName )
{
	int		n;
	int		iResult = 0;

	// If execute CreateDirectory when directory already exist,
	// window handle is increased. below code prevent this resource leak.
	n = GetFileAttributes( szDirName );
	if( n == -1 )
	{
		// create directory
		// QQQ : How set up security of directory.
		n = CreateDirectory( szDirName, NULL );
				
		if( n == 0 && GetLastError() != ERROR_ALREADY_EXISTS )
		{
			iResult = -1;
		}
	}

	return iResult;
}

/** Create directory. if there is not parent directory, this function create parent directory too.
 * 
 *  @author Yee Young Han 
 *  @param 	szFullDirName	directory path
 *	@return	if success, return 0.
 *			otherwise return -1.
 */
int HCreateDir( char * szFullDirName )
{
	char	szDirName[1024*2];
	int		i, j;
	int		iLen;
	int		iResult = 0;
	
	iLen = strlen( szFullDirName );
	memset( szDirName, 0, sizeof(szDirName));

	j = 0;
	for( i = 0; i < iLen; i++ )
	{
		// if input path is not drive name, create directory.
		if( i >= 2 && szFullDirName[i-1] != ':' && szFullDirName[i] == '\\' )
		{
			if( _HCreateDir( szDirName ) != 0 )	
			{
				iResult = -1;
				break;
			}
		}
		szDirName[j++] = szFullDirName[i];
	}
	
	if( _HCreateDir( szDirName ) != 0 ) iResult = -1;

	return iResult;
}
