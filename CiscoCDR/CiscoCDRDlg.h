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
#include "afxcmn.h"
#include <map>

typedef std::map< int, int > INT_MAP;

// CCiscoCDRDlg dialog
class CCiscoCDRDlg : public CDialog
{
// Construction
public:
	CCiscoCDRDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CISCOCDR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CListCtrl m_clsCdrList;
	RECT	m_sttCdrListMargin;
	int m_iDateTimeOriginationIndex;
	int m_iDateTimeConnectIndex;
	int m_iDateTimeDisconnectIndex;
	INT_MAP m_clsNoShowMap;
	std::string m_strFileName;
	CString m_strSearch;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedColumn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	void Show( const char * pszFileName );
	bool ShowHeader( STRING_LIST & clsList );
	void AddRow( STRING_LIST & clsList );
	void SaveColumn();
	afx_msg void OnNMDblclkCdrList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawCdrList(NMHDR *pNMHDR, LRESULT *pResult);
};
