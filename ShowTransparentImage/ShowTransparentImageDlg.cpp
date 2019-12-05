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
#include "ShowTransparentImage.h"
#include "ShowTransparentImageDlg.h"

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


// CShowTransparentImageDlg dialog




CShowTransparentImageDlg::CShowTransparentImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowTransparentImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowTransparentImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSPARENT_IMAGE, m_clsControl);
	DDX_Control(pDX, IDC_COLOR, m_clsColor);
}

BEGIN_MESSAGE_MAP(CShowTransparentImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CShowTransparentImageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN, &CShowTransparentImageDlg::OnBnClickedOpen)
	ON_CBN_SELCHANGE(IDC_COLOR, &CShowTransparentImageDlg::OnCbnSelchangeColor)
END_MESSAGE_MAP()


// CShowTransparentImageDlg message handlers

BOOL CShowTransparentImageDlg::OnInitDialog()
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

	m_clsColor.AddString( "red" );
	m_clsColor.AddString( "blue" );
	m_clsColor.AddString( "green" );
	m_clsColor.SetCurSel( 0 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShowTransparentImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CShowTransparentImageDlg::OnPaint()
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
HCURSOR CShowTransparentImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CShowTransparentImageDlg::OnBnClickedOk()
{
}

void CShowTransparentImageDlg::OnBnClickedOpen()
{
	CFileDialog clsDlg( TRUE, NULL, NULL, 0, "Image file(*.png)|*png||" );
	if( clsDlg.DoModal() == IDOK )
	{
		if( m_clsControl.SetFile( clsDlg.GetPathName() ) == false )
		{
			MessageBox( "Read image file error." );
		}
	}
}

void CShowTransparentImageDlg::OnCbnSelchangeColor()
{
	int iSel = m_clsColor.GetCurSel();
	if( iSel >= 0 )
	{
		CString strText;

		m_clsColor.GetLBText( iSel, strText );

		if( strText == "red" )
		{
			m_clsControl.SetAlphaColor( RGB(255,0,0) );
		}
		else if( strText == "blue" )
		{
			m_clsControl.SetAlphaColor( RGB(0,0,255) );
		}
		else if( strText == "green" )
		{
			m_clsControl.SetAlphaColor( RGB(0,255,0) );
		}
	}
}
