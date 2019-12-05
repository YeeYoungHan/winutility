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
#include "DosCropImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

/**
 * @ingroup ResizeImage
 * @brief 이미지 파일의 지정된 위치에서 지정된 폭과 높이로 자른다.
 * @param pszInput	입력 이미지 파일 이름
 * @param pszOutput 출력 이미지 파일 이름
 * @param iX				잘라낼 입력 이미지의 X 좌표
 * @parma iY				잘라낼 입력 이미지의 Y 좌표
 * @param iWidth		출력 이미지 파일 폭
 * @param iHeight		출력 이미지 파일 높이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CropImage( const char * pszInput, const char * pszOutput, int iX, int iY, int iWidth, int iHeight )
{
	CImage clsInput, clsOutput;
	HRESULT hr;
	bool bRes = false;

	hr = clsInput.Load( pszInput );
	if( FAILED(hr) )
	{
		printf( "Load(%s) error\n", pszInput );
		return false;
	}

	if( clsOutput.Create( iWidth, iHeight, clsInput.GetBPP() ) == FALSE )
	{
		printf( "output create error\n" );
	}
	else
	{
		HDC hDC = clsOutput.GetDC();
		if( hDC == NULL )
		{
			printf( "GetDC error\n" );
		}
		else
		{
			SetStretchBltMode( hDC, HALFTONE );

			if( clsInput.BitBlt( hDC, 0, 0, iWidth, iHeight, iX, iY, SRCCOPY ) == FALSE )
			{
				printf( "StretchBlt error\n" );
			}
			else
			{
				hr = clsOutput.Save( pszOutput );
				
				if( FAILED(hr) )
				{
					printf( "Save(%s) error\n", pszOutput );
				}
				else
				{
					bRes = true;
				}
			}

			clsOutput.ReleaseDC();
		}
	}

	return bRes;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		if( argc != 7 )
		{
			printf( "[Usage] %s {input image filename} {output image filename} {x} {y} {width} {height}\n", argv[0] );
		}
		else
		{
			const char * pszInput = argv[1];
			const char * pszOutput = argv[2];
			int iX = atoi( argv[3] );
			int iY = atoi( argv[4] );
			int iWidth = atoi( argv[5] );
			int iHeight = atoi( argv[6] );

			CropImage( pszInput, pszOutput, iX, iY, iWidth, iHeight );
		}
	}

	return nRetCode;
}
