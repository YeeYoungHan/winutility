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
#include "ThumbnailImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		if( argc != 5 )
		{
			_tprintf( _T("[Usage] %s {file full path} {thumbnail image full path} {thumbnail width} {thumbnail height}\n"), argv[0] );
			return 0;
		}

		TCHAR * pszFilePath = argv[1];
		TCHAR * pszThumbnailFilePath = argv[2];
		int iWidth = _ttoi( argv[3] );
		int iHeight = _ttoi( argv[4] );

		std::wstring strFolder, strFileName;

		int iLen = _tcslen( pszFilePath );

		for( int i = iLen - 1; i >= 0; --i )
		{
			if( pszFilePath[i] == _T('\\') )
			{
				strFolder.append( pszFilePath, i );
				strFileName.append( pszFilePath + i + 1 );
				break;
			}
		}

		HRESULT hr;

		CComPtr<IShellFolder> pclsDesktop, pclsFolder;
		IExtractImage * pclsExtractImage;
		LPITEMIDLIST pItemIdList;

		hr = SHGetDesktopFolder( &pclsDesktop );
		if( FAILED(hr) )
		{
			_tprintf( _T("SHGetDesktopFolder error\n") );
			return -1;
		}

		hr = pclsDesktop->ParseDisplayName( NULL, NULL, (LPWSTR)strFolder.c_str(), NULL, &pItemIdList, NULL );
		if( FAILED(hr) )
		{
			_tprintf( _T("ParseDisplayName desktop error\n") );
			return -1;
		}

		hr = pclsDesktop->BindToObject( pItemIdList, NULL, IID_IShellFolder, (void **)&pclsFolder );
		if( FAILED(hr) )
		{
			_tprintf( _T("BindToObject error\n") );
			return -1;
		}

		hr = pclsFolder->ParseDisplayName( NULL, NULL, (LPWSTR)strFileName.c_str(), NULL, &pItemIdList, NULL );
		if( FAILED(hr) )
		{
			_tprintf( _T("ParseDisplayName folder error\n") );
			return -1;
		}

		hr = pclsFolder->GetUIObjectOf( NULL, 1, (LPCITEMIDLIST *)&pItemIdList, IID_IExtractImage, NULL, (void **)&pclsExtractImage );
		if( FAILED(hr) )
		{
			_tprintf( _T("GetUIObjectOf error\n") );
			return -1;
		}

		SIZE sttSize;

		sttSize.cx = iWidth;
		sttSize.cy = iHeight;

		DWORD dwFlag = IEIFLAG_ORIGSIZE | IEIFLAG_QUALITY;
		HBITMAP hBitmap;
		TCHAR szPath[MAX_PATH];

		hr = pclsExtractImage->GetLocation( szPath, MAX_PATH, NULL, &sttSize, 32, &dwFlag );
		if( FAILED(hr) )
		{
			_tprintf( _T("GetLocation error\n") );
			return -1;
		}

		hr = pclsExtractImage->Extract( &hBitmap );
		if( FAILED(hr) )
		{
			_tprintf( _T("Extract error\n") );
			return -1;
		}

		CImage clsImage;

		clsImage.Attach( hBitmap );
		clsImage.Save( pszThumbnailFilePath );
	}

	return nRetCode;
}
