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

/**
 * @ingroup CropImage
 * @brief 이미지를 보여주고 잘라질 이미지를 사각형 박스로 보여주는 윈도우 컨트롤러
 */
class CCropImageControl : public CWnd
{
	DECLARE_DYNAMIC(CCropImageControl)

public:
	CCropImageControl();
	virtual ~CCropImageControl();

	static BOOL RegisterWindowClass();
	bool SetFile( const char * pszFileName );
	bool SaveFile( const char * pszFileName );
	bool Update( bool bForce = false );
	bool IsLoad( );

protected:
	DECLARE_MESSAGE_MAP()

	CImage m_clsImage;

	int m_iImageWidth;
	int m_iImageHeight;

	int m_iPaintWidth;
	int m_iPaintHeight;

	RECT m_sttBoxRect;
	int	m_iBoxWidth;
	int m_iBoxHeight;

	bool m_bDrag;
	bool m_bLoad;
	bool m_bWidth;	// 폭 기준인가?
	
	CPoint m_clsMousePos;

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


