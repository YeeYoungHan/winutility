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

#include "afxwin.h"

// CColumnDlg 대화 상자입니다.

class CColumnDlg : public CDialog
{
	DECLARE_DYNAMIC(CColumnDlg)

public:
	CColumnDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColumnDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_clsShowList;
	CListBox m_clsNoShowList;

	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
