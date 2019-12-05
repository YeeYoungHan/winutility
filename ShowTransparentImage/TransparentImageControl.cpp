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
#include "ShowTransparentImage.h"
#include "TransparentImageControl.h"


// CTransparentImageControl

IMPLEMENT_DYNAMIC(CTransparentImageControl, CWnd)

CTransparentImageControl::CTransparentImageControl() : m_bLoad(false), m_bAlpha(false)
{
	m_iAlphaColor = RGB(255,0,0);
}

CTransparentImageControl::~CTransparentImageControl()
{
}

BOOL CTransparentImageControl::RegisterWindowClass()
{
 WNDCLASS    sttClass;
 HINSTANCE   hInstance = AfxGetInstanceHandle();

 if( GetClassInfo( hInstance, TRANSPARENT_IMAGE_CONTROL_CLASSNAME, &sttClass ) == FALSE )
 {
  sttClass.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
  sttClass.lpfnWndProc      = ::DefWindowProc;
  sttClass.cbClsExtra       = 0;
  sttClass.cbWndExtra       = 0;
  sttClass.hInstance        = hInstance;
  sttClass.hIcon            = NULL;
  sttClass.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
  sttClass.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
  sttClass.lpszMenuName     = NULL;
  sttClass.lpszClassName    = TRANSPARENT_IMAGE_CONTROL_CLASSNAME;

  if( !AfxRegisterClass( &sttClass ) )
  {
   AfxThrowResourceException();
   return FALSE;
  }
 }

 return TRUE;
}

/**
 * @ingroup ShowTransparentImage
 * @brief 화면에 보여줄 이미지 파일을 설정한다.
 * @param pszFileName 이미지 파일 full path
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTransparentImageControl::SetFile( const char * pszFileName )
{
	m_clsImage.Destroy();

	m_bAlpha = false;

	HRESULT hr = m_clsImage.Load( pszFileName );

	if( SUCCEEDED(hr) )
	{
		if( m_clsImage.GetBPP() == 32 )
		{
			int iWidth = m_clsImage.GetWidth();
			int iHeight = m_clsImage.GetHeight();

			for( int i = 0; i < iWidth; ++i )
			{
				for( int j = 0; j < iHeight; ++j )
				{
					BYTE * p = (BYTE *)m_clsImage.GetPixelAddress( i, j );
					if( p[3] != 255 )
					{
						p[0] = ((p[0] * p[3]) + 127) / 255;
						p[1] = ((p[1] * p[3]) + 127) / 255;
						p[2] = ((p[2] * p[3]) + 127) / 255;

						m_bAlpha = true;
					}
				}
			}
		}

		m_bLoad = true;
		RedrawWindow();
		return true;
	}

	m_bLoad = false;
	RedrawWindow();

	return false;
}

/**
 * @ingroup ShowTransparentImage
 * @brief 투명한 영역에 보여줄 색상을 저장한다.
 * @param iColor 투명한 영역에 보여줄 색상
 */
void CTransparentImageControl::SetAlphaColor( COLORREF iColor )
{
	m_iAlphaColor = iColor;
	RedrawWindow();
}

BEGIN_MESSAGE_MAP(CTransparentImageControl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransparentImageControl 메시지 처리기입니다.



void CTransparentImageControl::OnPaint()
{
	CPaintDC dc(this); 

	if( m_bLoad )
	{
		// 더블버퍼링을 위한 메모리 DC 생성
		RECT sttRect;

		GetClientRect( &sttRect );
		CDC clsDC;
		CBitmap clsBitmap, * pclsBitmap;

		clsDC.CreateCompatibleDC( &dc ); 

		clsBitmap.CreateCompatibleBitmap( &dc, sttRect.right, sttRect.bottom );
		pclsBitmap = clsDC.SelectObject( &clsBitmap );

		// 메모리 DC 에 그리기
		CBrush clsBrush, * pclsBrush;
		clsBrush.CreateSolidBrush( m_bAlpha ? m_iAlphaColor : RGB(240,240,240) );
		pclsBrush = clsDC.SelectObject( &clsBrush );
		clsDC.FillRect( &sttRect, &clsBrush );
		clsDC.SelectObject( pclsBrush );

		if( m_bAlpha )
		{
			m_clsImage.AlphaBlend( clsDC, 0, 0, sttRect.right, sttRect.bottom, 0, 0, sttRect.right, sttRect.bottom );
		}
		else
		{
			m_clsImage.BitBlt( clsDC, 0, 0, sttRect.right, sttRect.bottom, 0, 0, SRCCOPY );
		}

		// 메모리 DC 에 그리기 연산을 모두 수행한 후, 메모리 DC 의 비트맵을 CPaintDC 에 복사한다.
		dc.BitBlt( 0, 0, sttRect.right, sttRect.bottom, &clsDC, 0, 0, SRCCOPY );
		clsDC.SelectObject( pclsBitmap );
	}
}
