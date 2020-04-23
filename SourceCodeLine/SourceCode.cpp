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

#include "StdAfx.h"
#include "SourceCode.h"

#include "SourceCodeThread.hpp"

CSourceFolder::CSourceFolder() : m_iLineCount(0)
{
}

// 기본 소스 코드 확장자
static const char * garrExt[] = { "cpp", "c", "hpp", "h", "m", "mm", "cc", "java", "js", "kt", "cs", "html", "htm", "css", "php", NULL };

CSourceCode::CSourceCode() : m_hWnd(NULL), m_bStop(false), m_bRun(false)
{
}

CSourceCode::~CSourceCode()
{
}

/**
 * @ingroup SourceCodeLine
 * @brief 입력된 폴더에 포함된 소스 코드 라인 개수를 측정하는 쓰레드를 실행한다.
 * @param hWnd			소스 코드 라인 개수 이벤트 수신 윈도우 핸들러
 * @param pszFolder 소스 코드 폴더
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSourceCode::Start( HWND hWnd, const char * pszFolder )
{
	if( m_bRun ) return false;

	std::string strFileName = GetProgramDirectory();
	strFileName.append( "\\ext.txt" );

	ReadExtFile( strFileName.c_str() );

	if( m_clsExtMap.empty() )
	{
		for( int i = 0; garrExt[i]; ++i )
		{
			InsertExt( garrExt[i] );
		}
	}

	m_clsFolderList.clear();

	m_hWnd = hWnd;
	m_strFolder = pszFolder;

	DWORD		dwThreadId;
	HANDLE	hThread;

	hThread = CreateThread( NULL, 0, SourceCodeThread, this, 0, &dwThreadId );
	if( hThread == NULL )
	{
		return false;
	}

	CloseHandle( hThread );

	return true;
}

/**
 * @ingroup SourceCodeLine
 * @brief 소스 코드 라인 개수를 측정하는 쓰레드를 중지시킨다.
 */
void CSourceCode::Stop( )
{
	if( m_bRun )
	{
		m_bStop = true;

		while( m_bRun )
		{
			Sleep(20);
		}

		m_bStop = false;
	}
}

/**
 * @ingroup SourceCodeLine
 * @brief 소스 코드 폴더 및 해당 폴더에 포함된 라인 개수를 저장한다.
 * @param pszName			폴더 이름
 * @param iLineCount	소스 코드 라인 개수
 */
void CSourceCode::Insert( const char * pszName, int iLineCount )
{
	CSourceFolder clsInfo;

	clsInfo.m_strName = pszName;
	clsInfo.m_iLineCount = iLineCount;

	m_clsFolderList.push_back( clsInfo );
}

/**
 * @ingroup SourceCodeLine
 * @brief 소스 코드 확장자를 저장한다.
 * @param pszExt 소스 코드 확장자
 */
void CSourceCode::InsertExt( const char * pszExt )
{
	EXT_MAP::iterator itMap;
	std::string strExt = pszExt;

	ToLower( strExt );

	itMap = m_clsExtMap.find( strExt );
	if( itMap == m_clsExtMap.end() )
	{
		m_clsExtMap.insert( EXT_MAP::value_type( strExt, 1 ) );
	}
}

/**
 * @ingroup SourceCodeLine
 * @brief 파일 이름이 소스 코드인지 검사한다.
 * @param pszFileName 파일 이름
 * @returns 파일 이름이 소스 코드이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSourceCode::IsSourceCodeFile( const char * pszFileName )
{
	std::string strExt;
	EXT_MAP::iterator itMap;

	if( GetFileExt( pszFileName, strExt ) == false ) return false;

	ToLower( strExt );

	itMap = m_clsExtMap.find( strExt );
	if( itMap == m_clsExtMap.end() )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup SourceCodeLine
 * @brief 파일에서 소스 코드 확장자 리스트를 가져온다.
 * @param pszFileName 파일 이름
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSourceCode::ReadExtFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "r" );
	if( fd )
	{
		m_clsExtMap.clear();

		char szBuf[255];

		memset( szBuf, 0, sizeof(szBuf) );
		while( fgets( szBuf, sizeof(szBuf), fd ) )
		{
			for( int i = 0; szBuf[i]; ++i )
			{
				if( szBuf[i] == '\r' || szBuf[i] == '\n' )
				{
					szBuf[i] = '\0';
					break;
				}
			}

			InsertExt( szBuf );

			memset( szBuf, 0, sizeof(szBuf) );
		}

		fclose( fd );
	}

	return true;
}
