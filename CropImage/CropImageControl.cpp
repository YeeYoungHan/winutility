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

CCropImageControl::CCropImageControl() : m_bDrag(false), m_bLoad(false)
{
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

bool CCropImageControl::SetFile( const char * pszFileName )
{
	m_clsImage.Load( pszFileName );

	RECT sttRect;

	GetClientRect( &sttRect );

	m_iPaintWidth = sttRect.right;
	m_iPaintHeight = sttRect.bottom;

	m_iImageWidth = m_clsImage.GetWidth();
	m_iImageHeight = m_clsImage.GetHeight();
	
	if( m_iImageWidth >= m_iImageHeight )
	{
		m_iPaintHeight = (int)((double)m_iImageHeight / m_iImageWidth * m_iPaintWidth);
	}
	else
	{
		m_iPaintWidth = (int)((double)m_iImageWidth / m_iImageHeight * m_iPaintHeight);
	}

	memset( &m_sttBoxRect, 0, sizeof(m_sttBoxRect) );

	if( m_iCropWidth >= m_iCropHeight )
	{
		m_iBoxHeight = (int)((double)m_iPaintWidth * m_iCropHeight / m_iCropWidth);
		if( m_sttBoxRect.bottom > m_iPaintHeight )
		{
			m_iBoxWidth = (int)((double)m_iPaintHeight * m_iCropWidth / m_iCropHeight);
			m_iBoxHeight = m_iPaintHeight;
		}
		else
		{
			m_iBoxWidth = m_iPaintWidth;
		}
	}
	else
	{
		m_iBoxWidth = (int)((double)m_iPaintHeight * m_iCropWidth / m_iCropHeight);
		if( m_sttBoxRect.right > m_iPaintWidth )
		{
			m_iBoxWidth = m_iPaintWidth;
			m_iBoxHeight = (int)((double)m_iPaintWidth * m_iCropHeight / m_iCropWidth);
		}
		else
		{
			m_iBoxHeight = m_iPaintHeight;
		}
	}

	m_sttBoxRect.right = m_iBoxWidth;
	m_sttBoxRect.bottom = m_iBoxHeight;

	m_bLoad = true;

	RedrawWindow();

	return true;
}

BEGIN_MESSAGE_MAP(CCropImageControl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CCropImageControl 메시지 처리기입니다.

void CCropImageControl::OnPaint()
{
	CPaintDC dc(this); 

	if( m_bLoad )
	{
		m_clsImage.StretchBlt( dc, 0, 0, m_iPaintWidth, m_iPaintHeight, 0, 0, m_iImageWidth, m_iImageHeight, SRCCOPY );
		
		CBrush clsBrush, * pclsBrush;

		clsBrush.CreateStockObject( NULL_BRUSH );

		pclsBrush = dc.SelectObject( &clsBrush );
		dc.Rectangle( &m_sttBoxRect );

		dc.SelectObject( pclsBrush );
	}
}

void CCropImageControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bDrag = true;
	m_clsMousePos = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CCropImageControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDrag = false;
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CCropImageControl::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bDrag )
	{
		CPoint clsPos = point - m_clsMousePos;
		RECT sttRect = m_sttBoxRect;
		m_clsMousePos = point;

		if( m_iBoxWidth < m_iPaintWidth )
		{
			sttRect.left += clsPos.x;
			if( sttRect.left <= 0 )
			{
				sttRect.left = 0;
			}
			else if( sttRect.left + m_iBoxWidth >= m_iPaintWidth )
			{
				sttRect.left = m_iPaintWidth - m_iBoxWidth;
			}

			sttRect.right = sttRect.left + m_iPaintWidth;
		}

		if( m_iBoxHeight < m_iPaintHeight )
		{
			sttRect.top += clsPos.y;
			if( sttRect.top <= 0 )
			{
				sttRect.top = 0;
			}
			else if( sttRect.top + m_iBoxHeight >= m_iPaintHeight )
			{
				sttRect.top = m_iPaintHeight - m_iBoxHeight;
			}

			sttRect.bottom = sttRect.top + m_iBoxHeight;
		}

		//TRACE( "point(%d,%d) rect(%d,%d,%d,%d)\n", clsPos.x, clsPos.y, m_sttBoxRect.left, m_sttBoxRect.top, m_sttBoxRect.right, m_sttBoxRect.bottom );

		if( sttRect.top != m_sttBoxRect.top || sttRect.left != m_sttBoxRect.left )
		{
			m_sttBoxRect = sttRect;
			RedrawWindow();
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}
