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
#include "SourceCodeLine.h"
#include "SourceCodeLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSourceCodeLineDlg dialog




CSourceCodeLineDlg::CSourceCodeLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSourceCodeLineDlg::IDD, pParent)
	, m_strFolder(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSourceCodeLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOLDER, m_strFolder);
	DDX_Control(pDX, IDC_FOLDER_LIST, m_clsFolderList);
}

BEGIN_MESSAGE_MAP(CSourceCodeLineDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(MESSAGE_SOURCE_EVENT, &CSourceCodeLineDlg::OnSourceEvent)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSourceCodeLineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSourceCodeLineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SELECT_FOLDER, &CSourceCodeLineDlg::OnBnClickedSelectFolder)
END_MESSAGE_MAP()


// CSourceCodeLineDlg message handlers

BOOL CSourceCodeLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_clsFolderList.InsertColumn( 0, "Name", LVCFMT_LEFT, 300 );
	m_clsFolderList.InsertColumn( 1, "Line", LVCFMT_RIGHT, 130 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSourceCodeLineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSourceCodeLineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSourceCodeLineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CSourceCodeLineDlg::OnSourceEvent( WPARAM wParam, LPARAM lParam )
{
	if( wParam == WPARAM_END )
	{
		SOURCE_FOLDER::iterator itSF;
		int iRow = 0;
		int iLineCount = 0;
		char szTemp[21];
		std::string strLineCount;

		m_clsFolderList.DeleteAllItems();

		for( itSF = m_clsSourceCode.m_clsFolderList.begin(); itSF != m_clsSourceCode.m_clsFolderList.end(); ++itSF )
		{
			m_clsFolderList.InsertItem( iRow, itSF->m_strName.c_str() );

			_snprintf( szTemp, sizeof(szTemp), "%d", itSF->m_iLineCount );
			strLineCount = szTemp;
			CommaSepString( strLineCount );
			m_clsFolderList.SetItemText( iRow, 1, strLineCount.c_str() );

			iLineCount += itSF->m_iLineCount;
			++iRow;
		}

		m_clsFolderList.InsertItem( iRow, "#" );

		_snprintf( szTemp, sizeof(szTemp), "%d", iLineCount );
		strLineCount = szTemp;
		CommaSepString( strLineCount );
		m_clsFolderList.SetItemText( iRow, 1, strLineCount.c_str() );
	}

	return 0;
}

void CSourceCodeLineDlg::OnBnClickedOk()
{
	//OnOK();
}

void CSourceCodeLineDlg::OnBnClickedCancel()
{
	m_clsSourceCode.Stop();
	OnCancel();
}

void CSourceCodeLineDlg::OnBnClickedSelectFolder()
{
	BROWSEINFO sttInfo;
	char szFolder[1024]; // 선택된 폴더 full path 를 저장할 변수
	
	memset( &sttInfo, 0, sizeof(sttInfo) );
	memset( szFolder, 0, sizeof(szFolder) );
	
	sttInfo.hwndOwner = GetSafeHwnd();
	sttInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&sttInfo);
	::SHGetPathFromIDList( pItemIdList, szFolder );
	::CoTaskMemFree( pItemIdList );

	if( szFolder[0] )
	{
		m_strFolder = szFolder;
		UpdateData( FALSE );

		m_clsSourceCode.Start( GetSafeHwnd(), szFolder );
	}
}
