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
#include "PcapViewer.h"
#include "PcapViewerDlg.h"
#include "PacketHeader.h"

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


// CPcapViewerDlg dialog




CPcapViewerDlg::CPcapViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPcapViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

#define MAX_COL	7

void CPcapViewerDlg::ChangePacket( int iFirst, int iSecond )
{
	CString arrFirstText[MAX_COL];
	CString arrSecondText[MAX_COL];

	for( int i = 0; i < MAX_COL; ++i )
	{
		arrFirstText[i] = m_lstPacket.GetItemText( iFirst, i );
		arrSecondText[i] = m_lstPacket.GetItemText( iSecond, i );
	}

	for( int i = 0; i < MAX_COL; ++i )
	{
		m_lstPacket.SetItemText( iFirst, i, arrSecondText[i] );
		m_lstPacket.SetItemText( iSecond, i, arrFirstText[i] );
	}
}

void CPcapViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACKET_LIST, m_lstPacket);
}

BEGIN_MESSAGE_MAP(CPcapViewerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPcapViewerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN, &CPcapViewerDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_UP, &CPcapViewerDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CPcapViewerDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_SAVE, &CPcapViewerDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CPcapViewerDlg message handlers

BOOL CPcapViewerDlg::OnInitDialog()
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

	m_lstPacket.InsertColumn( 0, "No", LVCFMT_LEFT, 50 );
	m_lstPacket.InsertColumn( 1, "Time", LVCFMT_LEFT, 100 );
	m_lstPacket.InsertColumn( 2, "Source", LVCFMT_LEFT, 100 );
	m_lstPacket.InsertColumn( 3, "Destination", LVCFMT_LEFT, 100 );
	m_lstPacket.InsertColumn( 4, "Proto", LVCFMT_LEFT, 50 );
	m_lstPacket.InsertColumn( 5, "Len", LVCFMT_LEFT, 40 );
	m_lstPacket.InsertColumn( 6, "Info", LVCFMT_LEFT, 200 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPcapViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPcapViewerDlg::OnPaint()
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
HCURSOR CPcapViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPcapViewerDlg::OnBnClickedOk()
{
}

void CPcapViewerDlg::OnBnClickedOpen()
{
	CFileDialog clsDlg( TRUE, "pcap", NULL, 0, "pcap file(*.pcap)|*.pcap||" );
	if( clsDlg.DoModal() == IDOK )
	{
		if( m_clsPacketList.Open( clsDlg.GetPathName() ) == false )
		{
			MessageBox( "pcap file open error", "ERROR", MB_OK | MB_ICONERROR );
			return;
		}

		m_clsPacketList.ShowList( m_lstPacket );
	}
}

void CPcapViewerDlg::OnBnClickedSave()
{
	CFileDialog clsDlg( FALSE, NULL, NULL, 0, "pcap file(*.pcap)|*.pcap||" );
	if( clsDlg.DoModal() == IDOK )
	{
		const char * pszFileName = clsDlg.GetPathName();

		if( IsExistFile( pszFileName ) )
		{
			MessageBox( "pcap file already exists", "ERROR", MB_OK | MB_ICONERROR );
			return;
		}

		if( m_clsPacketList.Save( pszFileName ) == false )
		{
			MessageBox( "pcap file save error", "ERROR", MB_OK | MB_ICONERROR );
			return;
		}
	}
}

void CPcapViewerDlg::OnBnClickedUp()
{
	POSITION pos = m_lstPacket.GetFirstSelectedItemPosition();
	if( pos == NULL )
	{
		MessageBox( "no selection", "INFO", MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		int iIndex = m_lstPacket.GetNextSelectedItem(pos);

		if( m_clsPacketList.Up(iIndex) )
		{
			ChangePacket( iIndex - 1, iIndex );
		}
	}
}

void CPcapViewerDlg::OnBnClickedDown()
{
	POSITION pos = m_lstPacket.GetFirstSelectedItemPosition();
	if( pos == NULL )
	{
		MessageBox( "no selection", "INFO", MB_OK | MB_ICONINFORMATION );
	}
	else
	{
		int iIndex = m_lstPacket.GetNextSelectedItem(pos);

		if( m_clsPacketList.Down(iIndex) )
		{
			ChangePacket( iIndex, iIndex + 1 );
		}
	}
}

