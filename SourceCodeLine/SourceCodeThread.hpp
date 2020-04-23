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


int GetFileLineCount( const char * pszFileName )
{
	int iLineCount = 0;
	FILE * fd = fopen( pszFileName, "r" );
	if( fd )
	{
		char szBuf[8192];

		while( fgets( szBuf, sizeof(szBuf), fd ) )
		{
			++iLineCount;
		}

		fclose( fd );
	}

	return iLineCount;
}

int GetFolderLineCount( CSourceCode * pclsArg, const char * pszFolder )
{
	int				iLineCount = 0;
	char			szPath[2048];
	HANDLE		hSearch;
	WIN32_FIND_DATA wfd;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pszFolder );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL	bFind = TRUE;
	
		while( bFind && pclsArg->m_bStop == false )
		{
			if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
			{
				_snprintf( szPath, sizeof(szPath), "%s\\%s", pszFolder, wfd.cFileName );

				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					iLineCount += GetFolderLineCount( pclsArg, szPath );
				}
				else if( pclsArg->IsSourceCodeFile( wfd.cFileName ) )
				{
					iLineCount += GetFileLineCount( szPath );
				}
			}

			bFind = FindNextFile( hSearch, &wfd );
		}

		FindClose( hSearch );
	}

	return iLineCount;
}

DWORD WINAPI SourceCodeThread( LPVOID lpParameter )
{
	CSourceCode * pclsArg = (CSourceCode *)lpParameter;

	pclsArg->m_bRun = true;

	int       iLineCount = 0, iFolderCount = 0, iFileCount = 0, iFolderIndex = 0, iPercent;
	char			szPath[2048];
	HANDLE		hSearch;
	WIN32_FIND_DATA wfd;

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pclsArg->m_strFolder.c_str() );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL	bFind = TRUE;

		while( bFind && pclsArg->m_bStop == false )
		{
			if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
			{
				_snprintf( szPath, sizeof(szPath), "%s\\%s", pclsArg->m_strFolder.c_str(), wfd.cFileName );

				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					++iFolderCount;
				}
				else if( pclsArg->IsSourceCodeFile( wfd.cFileName ) )
				{
					++iFileCount;
				}
			}

			bFind = FindNextFile( hSearch, &wfd );
		}

		FindClose( hSearch );
	}

	_snprintf( szPath, sizeof(szPath), "%s\\*.*", pclsArg->m_strFolder.c_str() );

	memset( &wfd, 0, sizeof(wfd) );
	hSearch = FindFirstFile( szPath, &wfd );
	if( hSearch != INVALID_HANDLE_VALUE )
	{
		BOOL	bFind = TRUE;

		while( bFind && pclsArg->m_bStop == false )
		{
			if( strcmp( wfd.cFileName, "." ) && strcmp( wfd.cFileName, ".." ) )
			{
				_snprintf( szPath, sizeof(szPath), "%s\\%s", pclsArg->m_strFolder.c_str(), wfd.cFileName );

				if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					int iFolderLineCount = GetFolderLineCount( pclsArg, szPath );

					if( iFolderLineCount > 0 )
					{
						pclsArg->Insert( wfd.cFileName, iFolderLineCount );
					}

					++iFolderIndex;
					iPercent = iFolderIndex * 100 / iFolderCount;
					::SendMessage( pclsArg->m_hWnd, MESSAGE_SOURCE_EVENT, WPARAM_PERCENT, iPercent );
				}
				else if( pclsArg->IsSourceCodeFile( wfd.cFileName ) )
				{
					iLineCount += GetFileLineCount( szPath );
				}
			}

			bFind = FindNextFile( hSearch, &wfd );
		}

		FindClose( hSearch );
	}

	if( iLineCount > 0 )
	{
		pclsArg->Insert( ".", iLineCount );
	}

	::SendMessage( pclsArg->m_hWnd, MESSAGE_SOURCE_EVENT, WPARAM_END, 0 );

	pclsArg->m_bRun = false;

	return 0;
}
