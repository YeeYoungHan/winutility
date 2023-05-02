
// CiscoCDRDlg.h : header file
//

#pragma once
#include "afxcmn.h"


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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void Show( const char * pszFileName );
	bool ShowHeader( STRING_LIST & clsList );
	void AddRow( STRING_LIST & clsList );

	CListCtrl m_clsCdrList;
	RECT	m_sttCdrListMargin;
};
