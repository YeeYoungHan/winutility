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

CTransparentImageControl::CTransparentImageControl() : m_bLoad(false)
{

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

bool CTransparentImageControl::SetFile( const char * pszFileName )
{
	m_clsImage.Destroy();

	HRESULT hr = m_clsImage.Load( pszFileName );

	if( SUCCEEDED(hr) )
	{
		m_bLoad = true;
		RedrawWindow();
		return true;
	}

	m_bLoad = false;
	RedrawWindow();

	return false;
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

		clsBrush.CreateSolidBrush( RGB(255,0,0) );
		pclsBrush = clsDC.SelectObject( &clsBrush );
		clsDC.FillRect( &sttRect, &clsBrush );
		clsDC.SelectObject( pclsBrush );

		//m_clsImage.BitBlt( clsDC, 0, 0, sttRect.right, sttRect.bottom, 0, 0, SRCCOPY );

		// CImage 의 m_iTransparentColor 가 -1 이어서 정상적으로 동작하지 않는다.
		//m_clsImage.TransparentBlt( clsDC, 0, 0, sttRect.right, sttRect.bottom, 0, 0, sttRect.right, sttRect.bottom );

		m_clsImage.AlphaBlend( clsDC, 0, 0, sttRect.right, sttRect.bottom, 0, 0, sttRect.right, sttRect.bottom );

		// 메모리 DC 에 그리기 연산을 모두 수행한 후, 메모리 DC 의 비트맵을 CPaintDC 에 복사한다.
		dc.BitBlt( 0, 0, sttRect.right, sttRect.bottom, &clsDC, 0, 0, SRCCOPY );
		clsDC.SelectObject( pclsBitmap );
	}
}
