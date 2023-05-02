#pragma once
#include "afxwin.h"


// CColumnDlg ��ȭ �����Դϴ�.

class CColumnDlg : public CDialog
{
	DECLARE_DYNAMIC(CColumnDlg)

public:
	CColumnDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColumnDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_COL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_clsShowList;
	CListBox m_clsNoShowList;

	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
