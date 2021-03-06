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

#include "stdafx.h"

const char * GetProgramDirectory( )
{
	static char	szDir[1024];

	if( strlen(szDir) == 0 )
	{
		HMODULE	hThis;

		hThis = GetModuleHandle( NULL );

		GetModuleFileName( hThis, szDir, sizeof(szDir));
		for( int i = (int)strlen( szDir) - 1; i >= 0; i-- )
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

/**
 * @ingroup ResizeImage
 * @brief 이미지 파일의 폭과 높이를 수정한다.
 * @param pszInput	입력 이미지 파일 이름
 * @param pszOutput 출력 이미지 파일 이름
 * @param iWidth		출력 이미지 파일 폭
 * @param iHeight		출력 이미지 파일 높이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool ResizeImage( const char * pszInput, const char * pszOutput, int iWidth, int iHeight )
{
	CImage clsInput, clsOutput;
	HRESULT hr;
	bool bRes = false;

	hr = clsInput.Load( pszInput );
	if( FAILED(hr) )
	{
		return false;
	}

	if( clsOutput.Create( iWidth, iHeight, clsInput.GetBPP() ) == FALSE )
	{
		return false;
	}

	HDC hDC = clsOutput.GetDC();
	if( hDC )
	{
		SetStretchBltMode( hDC, HALFTONE );

		if( clsInput.StretchBlt( hDC, 0, 0, iWidth, iHeight, SRCCOPY ) )
		{
			hr = clsOutput.Save( pszOutput );
			
			if( SUCCEEDED(hr) )
			{
				bRes = true;
			}
		}

		clsOutput.ReleaseDC();
	}

	return bRes;
}

/**
 * @ingroup CropImage
 * @brief 사용자가 폴더를 선택할 수 있는 대화상자를 보여준다.
 * @param strFolder [out] 사용자가 선택한 폴더
 * @returns 사용자가 폴더를 선택하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool SelectFolder( std::string & strFolder )
{
	BROWSEINFO sttInfo;
	char szFolder[1024];

	memset( &sttInfo, 0, sizeof(sttInfo) );
	memset( szFolder, 0, sizeof(szFolder) );

  sttInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&sttInfo);
	if( pItemIdList )
	{
		::SHGetPathFromIDList( pItemIdList, szFolder );
		::CoTaskMemFree( pItemIdList );
		strFolder = szFolder;

		return true;
	}

	return false;
}

/**
 * @ingroup CropImage
 * @brief 이미지 파일인지 검사한다.
 * @param pszFileName 파일 이름
 * @returns 이미지 파일이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsImageFile( const char * pszFileName )
{
	int iLen = strlen( pszFileName );
	const char * pszExt = NULL;

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( pszFileName[i] == '.' )
		{
			pszExt = pszFileName + i;
			break;
		}
	}

	if( pszExt )
	{
		if( !_stricmp( pszExt, ".jpg" ) || !_stricmp( pszExt, ".jpeg" ) || !_stricmp( pszExt, ".png" ) )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup CropImage
 * @brief 폴더에 포함된 이미지 파일 리스트를 가져온다.
 * @param pszDirName	폴더 full path
 * @param clsFileList [out] 이미지 파일 리스트
 * @returns 폴더에 포함된 이미지 파일들을 리스트에 저장한다.
 */
bool GetFileList( const char * pszDirName, FILE_LIST & clsFileList )
{
	clsFileList.clear();

	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;
		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;

		if( IsImageFile( sttFindData.cFileName ) )
		{
			std::string strFileName = pszDirName;
			strFileName.append( "\\" );
			strFileName.append( sttFindData.cFileName );

			clsFileList.push_back( strFileName );
		}
	}

	FindClose( hFind );

	return true;
}

