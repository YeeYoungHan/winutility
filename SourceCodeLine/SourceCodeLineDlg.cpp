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
	, m_strPercent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset( m_szFolder, 0, sizeof(m_szFolder) );
}

void CSourceCodeLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FOLDER, m_strFolder);
	DDX_Control(pDX, IDC_FOLDER_LIST, m_clsFolderList);
	DDX_Control(pDX, IDC_PROGRESS, m_clsProgress);
	DDX_Text(pDX, IDC_PERCENT, m_strPercent);
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
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FOLDER_LIST, &CSourceCodeLineDlg::OnLvnItemchangedFolderList)
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

	m_clsFolderList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );

	m_clsFolderList.InsertColumn( 0, "Name", LVCFMT_LEFT, 300 );
	m_clsFolderList.InsertColumn( 1, "Line", LVCFMT_RIGHT, 130 );

	m_clsProgress.SetRange( 0, 100 );

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
		// 소스 코드 라인 분석 완료 이벤트
		SOURCE_FOLDER::iterator itSF;
		int iRow = 0;
		int iLineCount = 0;
		std::string strLineCount;

		m_clsFolderList.DeleteAllItems();

		for( itSF = m_clsSourceCode.m_clsFolderList.begin(); itSF != m_clsSourceCode.m_clsFolderList.end(); ++itSF )
		{
			GetLineCount( itSF->m_iLineCount, strLineCount );

			m_clsFolderList.InsertItem( iRow, itSF->m_strName.c_str() );
			m_clsFolderList.SetItemText( iRow, 1, strLineCount.c_str() );
			m_clsFolderList.SetCheck( iRow, TRUE );

			iLineCount += itSF->m_iLineCount;
			++iRow;
		}

		GetLineCount( iLineCount, strLineCount );

		m_clsFolderList.InsertItem( iRow, "# TOTAL #" );
		m_clsFolderList.SetItemText( iRow, 1, strLineCount.c_str() );

		m_clsProgress.SetPos( 100 );
		m_strPercent = "100%";
		UpdateData( FALSE );
	}
	else if( wParam == WPARAM_PERCENT )
	{
		// 소스 코드 라인 분석 페센트 이벤트
		int iPercent = lParam;

		m_clsProgress.SetPos( iPercent );

		m_strPercent.Format( "%d%%", iPercent );
		UpdateData( FALSE );
	}

	return 0;
}

void CSourceCodeLineDlg::OnBnClickedOk()
{
}

void CSourceCodeLineDlg::OnBnClickedCancel()
{
	m_clsSourceCode.Stop();
	OnCancel();
}

int CALLBACK BrowseCallBack( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	if( uMsg == BFFM_INITIALIZED )
	{
		SendMessage( hwnd, BFFM_SETSELECTION, TRUE, lpData );
	}

	return 0;
}

void CSourceCodeLineDlg::OnBnClickedSelectFolder()
{
	BROWSEINFO sttInfo;
	char szFolder[1024]; // 선택된 폴더 full path 를 저장할 변수

	memset( &sttInfo, 0, sizeof(sttInfo) );
	memset( szFolder, 0, sizeof(szFolder) );
	
	sttInfo.hwndOwner = GetSafeHwnd();
	sttInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_VALIDATE;

	if( strlen( m_szFolder ) > 0 )
	{
		sttInfo.lpfn = BrowseCallBack;
		sttInfo.lParam = (LPARAM)m_szFolder;
	}

	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&sttInfo);
	::SHGetPathFromIDList( pItemIdList, szFolder );
	::CoTaskMemFree( pItemIdList );

	if( szFolder[0] )
	{
		m_strFolder = szFolder;
		_snprintf( m_szFolder, sizeof(m_szFolder), "%s", szFolder );
		m_clsProgress.SetPos( 0 );
		m_strPercent = "0%";
		UpdateData( FALSE );

		m_clsSourceCode.Start( GetSafeHwnd(), szFolder );
	}
	else
	{
		MessageBox( "Select folder!!!", "Error", MB_OK | MB_ICONERROR );
	}
}

void CSourceCodeLineDlg::OnLvnItemchangedFolderList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	// 체크된 폴더의 라인 개수만 TOTAL 개수에 저장한다.
	SOURCE_FOLDER::iterator itSF;
	int iRow = 0;
	int iLineCount = 0;
	std::string strLineCount;

	for( itSF = m_clsSourceCode.m_clsFolderList.begin(); itSF != m_clsSourceCode.m_clsFolderList.end(); ++itSF )
	{
		if( m_clsFolderList.GetCheck( iRow ) )
		{
			iLineCount += itSF->m_iLineCount;
		}

		++iRow;
	}

	GetLineCount( iLineCount, strLineCount );
	m_clsFolderList.SetItemText( iRow, 1, strLineCount.c_str() );
	
	*pResult = 0;
}
