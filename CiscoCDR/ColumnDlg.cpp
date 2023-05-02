// ColumnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CiscoCDR.h"
#include "ColumnDlg.h"
#include "Setup.h"

// CColumnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColumnDlg, CDialog)

CColumnDlg::CColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColumnDlg::IDD, pParent)
{

}

CColumnDlg::~CColumnDlg()
{
}

void CColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALL_LIST, m_clsShowList);
	DDX_Control(pDX, IDC_NO_SHOW_LIST, m_clsNoShowList);
}


BEGIN_MESSAGE_MAP(CColumnDlg, CDialog)
	ON_BN_CLICKED(IDC_INSERT, &CColumnDlg::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_DELETE, &CColumnDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CColumnDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CColumnDlg 메시지 처리기입니다.

void CColumnDlg::OnBnClickedInsert()
{
	int iIndex = m_clsShowList.GetCurSel();

	if( iIndex >= 0 )
	{
		CString strText;

		m_clsShowList.GetText( iIndex, strText );
		m_clsShowList.DeleteString( iIndex );
		m_clsNoShowList.AddString( strText );
	}
}

void CColumnDlg::OnBnClickedDelete()
{
	int iIndex = m_clsNoShowList.GetCurSel();

	if( iIndex >= 0 )
	{
		CString strText;

		m_clsNoShowList.GetText( iIndex, strText );
		m_clsNoShowList.DeleteString( iIndex );
		m_clsShowList.AddString( strText );
	}
}

void CColumnDlg::OnBnClickedOk()
{
	STRING_LIST::iterator itSL;

	for( itSL = gclsColList.begin(); itSL != gclsColList.end(); ++itSL )
	{
		gclsSetup.PutInt( itSL->c_str(), 0 );
	}

	int iCount = m_clsNoShowList.GetCount();

	for( int i = 0; i < iCount; ++i )
	{
		CString strText;

		m_clsNoShowList.GetText( i, strText );
		gclsSetup.PutInt( strText, 1 );
	}

	gclsSetup.PutFile();
	
	OnOK();
}

BOOL CColumnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	STRING_LIST::iterator itSL;

	for( itSL = gclsColList.begin(); itSL != gclsColList.end(); ++itSL )
	{
		if( gclsSetup.GetInt( itSL->c_str(), 0 ) == 1 )
		{
			m_clsNoShowList.AddString( itSL->c_str() );
		}
		else
		{
			m_clsShowList.AddString( itSL->c_str() );
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
