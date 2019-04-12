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
 * @brief �̹��� ������ ���� ���̸� �����Ѵ�.
 * @param pszInput	�Է� �̹��� ���� �̸�
 * @param pszOutput ��� �̹��� ���� �̸�
 * @param iWidth		��� �̹��� ���� ��
 * @param iHeight		��� �̹��� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
