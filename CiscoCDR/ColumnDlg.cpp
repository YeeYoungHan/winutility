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
