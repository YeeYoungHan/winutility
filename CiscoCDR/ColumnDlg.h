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
