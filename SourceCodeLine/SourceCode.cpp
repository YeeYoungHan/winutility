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

// �⺻ �ҽ� �ڵ� Ȯ����
static const char * garrExt[] = { "cpp", "c", "hpp", "h", "m", "mm", "cc", "java", "js", "kt", "cs", "html", "htm", "css", "php", NULL };

CSourceCode::CSourceCode() : m_hWnd(NULL), m_bStop(false), m_bRun(false)
{
}

CSourceCode::~CSourceCode()
{
}

/**
 * @ingroup SourceCodeLine
 * @brief �Էµ� ������ ���Ե� �ҽ� �ڵ� ���� ������ �����ϴ� �����带 �����Ѵ�.
 * @param hWnd			�ҽ� �ڵ� ���� ���� �̺�Ʈ ���� ������ �ڵ鷯
 * @param pszFolder �ҽ� �ڵ� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief �ҽ� �ڵ� ���� ������ �����ϴ� �����带 ������Ų��.
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
 * @brief �ҽ� �ڵ� ���� �� �ش� ������ ���Ե� ���� ������ �����Ѵ�.
 * @param pszName			���� �̸�
 * @param iLineCount	�ҽ� �ڵ� ���� ����
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
 * @brief �ҽ� �ڵ� Ȯ���ڸ� �����Ѵ�.
 * @param pszExt �ҽ� �ڵ� Ȯ����
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
 * @brief ���� �̸��� �ҽ� �ڵ����� �˻��Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ���� �̸��� �ҽ� �ڵ��̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief ���Ͽ��� �ҽ� �ڵ� Ȯ���� ����Ʈ�� �����´�.
 * @param pszFileName ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
