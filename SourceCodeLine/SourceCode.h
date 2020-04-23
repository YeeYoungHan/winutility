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

#pragma once

#include <string>
#include <list>
#include <map>

class CSourceFolder
{
public:
	CSourceFolder();

	std::string m_strName;
	int					m_iLineCount;
};

typedef std::list< CSourceFolder > SOURCE_FOLDER;
typedef std::map< std::string, char > EXT_MAP;

class CSourceCode
{
public:
	CSourceCode();
	~CSourceCode();

	bool Start( HWND hWnd, const char * pszFolder );
	void Stop( );

	void Insert( const char * pszName, int iLineCount );

	void InsertExt( const char * pszExt );
	bool IsSourceCodeFile( const char * pszFileName );

	HWND m_hWnd;
	std::string m_strFolder;

	SOURCE_FOLDER m_clsFolderList;
	EXT_MAP	m_clsExtMap;

	bool m_bStop;
	bool m_bRun;
};
