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
#include "CropImage.h"
#include "CropImageControl.h"

#define CROP_IMAGE_CONTROL_CLASSNAME _T("CropImageControl")

// CCropImageControl

IMPLEMENT_DYNAMIC(CCropImageControl, CWnd)

CCropImageControl::CCropImageControl()
{
	m_clsImage.Load( "c:\\temp\\town\\1.jpg" );
	m_iCropWidth = 847;
	m_iCropHeight = 551;
}

CCropImageControl::~CCropImageControl()
{
}

BOOL CCropImageControl::RegisterWindowClass()
{
	WNDCLASS    sttClass;
	HINSTANCE   hInstance = AfxGetInstanceHandle();

	if( GetClassInfo( hInstance, CROP_IMAGE_CONTROL_CLASSNAME, &sttClass ) == FALSE )
	{
		sttClass.style            = CS_DBLCLKS;
		sttClass.lpfnWndProc      = ::DefWindowProc;
		sttClass.cbClsExtra       = 0;
		sttClass.cbWndExtra       = 0;
		sttClass.hInstance        = hInstance;
		sttClass.hIcon            = NULL;
		sttClass.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		sttClass.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		sttClass.lpszMenuName     = NULL;
		sttClass.lpszClassName    = CROP_IMAGE_CONTROL_CLASSNAME;

		if( !AfxRegisterClass( &sttClass ) )
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCropImageControl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CCropImageControl 메시지 처리기입니다.

void CCropImageControl::OnPaint()
{
	CPaintDC dc(this); 

	RECT sttRect;

	GetClientRect( &sttRect );

	m_iPaintWidth = sttRect.right;
	m_iPaintHeight = sttRect.bottom;

	int iImageWidth = m_clsImage.GetWidth();
	int iImageHeight = m_clsImage.GetHeight();
	
	if( iImageWidth >= iImageHeight )
	{
		m_iPaintHeight = (int)((double)iImageHeight / iImageWidth * m_iPaintWidth);
	}
	else
	{
		m_iPaintWidth = (int)((double)iImageWidth / iImageHeight * m_iPaintHeight);
	}

	m_clsImage.StretchBlt( dc, 0, 0, m_iPaintWidth, m_iPaintHeight, 0, 0, iImageWidth, iImageHeight, SRCCOPY );

	memset( &m_sttCropRect, 0, sizeof(m_sttCropRect) );
	
	if( m_iCropWidth >= m_iCropHeight )
	{
		m_sttCropRect.bottom = (int)((double)m_iPaintWidth * m_iCropHeight / m_iCropWidth);
		if( m_sttCropRect.bottom > m_iPaintHeight )
		{
			m_sttCropRect.right = (int)((double)m_iPaintHeight * m_iCropWidth / m_iCropHeight);
			m_sttCropRect.bottom = m_iPaintHeight;
		}
		else
		{
			m_sttCropRect.right = m_iPaintWidth;
		}
	}
	else
	{
		m_sttCropRect.right = (int)((double)m_iPaintHeight * m_iCropWidth / m_iCropHeight);
		if( m_sttCropRect.right > m_iPaintWidth )
		{
			m_sttCropRect.right = m_iPaintWidth;
			m_sttCropRect.bottom = (int)((double)m_iPaintWidth * m_iCropHeight / m_iCropWidth);
		}
		else
		{
			m_sttCropRect.bottom = m_iPaintHeight;
		}
	}

	CBrush clsBrush, * pclsBrush;

	clsBrush.CreateStockObject( NULL_BRUSH );

	pclsBrush = dc.SelectObject( &clsBrush );
	dc.Rectangle( &m_sttCropRect );

	dc.SelectObject( pclsBrush );
}
