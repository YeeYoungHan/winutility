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
#include "ResizeImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

			if( clsInput.StretchBlt( hDC, 0, 0, iWidth, iHeight, SRCCOPY ) == FALSE )
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
