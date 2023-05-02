
// CiscoCDRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CiscoCDR.h"
#include "CiscoCDRDlg.h"
#include "Setup.h"

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


// CCiscoCDRDlg dialog




CCiscoCDRDlg::CCiscoCDRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCiscoCDRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCiscoCDRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CDR_LIST, m_clsCdrList);
}

BEGIN_MESSAGE_MAP(CCiscoCDRDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCiscoCDRDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN, &CCiscoCDRDlg::OnBnClickedOpen)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCiscoCDRDlg message handlers

BOOL CCiscoCDRDlg::OnInitDialog()
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

	gclsSetup.GetFile();

	RECT sttRect, sttCdrRect;

	GetWindowRect( &sttRect );
	m_clsCdrList.GetWindowRect( &sttCdrRect );

	int iFrame = GetSystemMetrics(SM_CYFRAME);
	int iCaption = GetSystemMetrics(SM_CYCAPTION);
	int iTitleHeight = iFrame + iCaption;

	m_sttCdrListMargin.left = sttCdrRect.left - sttRect.left - iFrame;
	m_sttCdrListMargin.top = sttCdrRect.top - sttRect.top - iTitleHeight;
	m_sttCdrListMargin.bottom = sttRect.bottom - sttCdrRect.bottom - iFrame;
	m_sttCdrListMargin.right = sttRect.right - sttCdrRect.right - iFrame;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCiscoCDRDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCiscoCDRDlg::OnPaint()
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
HCURSOR CCiscoCDRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCiscoCDRDlg::OnBnClickedOk()
{
}

void CCiscoCDRDlg::OnBnClickedOpen()
{
	CFileDialog clsDlg( TRUE );
	if( clsDlg.DoModal() == IDOK )
	{
		Show( clsDlg.GetPathName() );
	}
}

void CCiscoCDRDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if( m_clsCdrList.GetSafeHwnd() )
	{
		m_clsCdrList.SetWindowPos( NULL, m_sttCdrListMargin.left, m_sttCdrListMargin.top
			, cx - m_sttCdrListMargin.left - m_sttCdrListMargin.right, cy - m_sttCdrListMargin.top - m_sttCdrListMargin.bottom, SWP_NOZORDER );
	}
}

void CCiscoCDRDlg::OnDestroy()
{
	CDialog::OnDestroy();

	CHeaderCtrl * pclsHeaderCtrl = m_clsCdrList.GetHeaderCtrl();
	int i, iCount = pclsHeaderCtrl->GetItemCount();

	for( i = 0; i < iCount; ++i )
	{
		gclsSetup.PutInt( "cdr", i, m_clsCdrList.GetColumnWidth( i ) );
	}

	gclsSetup.PutFile();
}

void CCiscoCDRDlg::Show( const char * pszFileName )
{
	int iHeaderCount = m_clsCdrList.GetHeaderCtrl()->GetItemCount();

	for( int i = 0; i < iHeaderCount; ++i )
	{
		m_clsCdrList.DeleteColumn(0);
	}

	m_clsCdrList.DeleteAllItems();

	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL )
	{
		MessageBox( "file open error" );
		return;
	}

	char szBuf[8192];
	int iLine = 0;
	STRING_LIST clsList;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		++iLine;

		SplitString( szBuf, clsList, ',' );
		if( clsList.size() < 20 )
		{
			break;
		}

		if( iLine == 1 )
		{
			if( ShowHeader( clsList ) == false ) break;
		}
		else
		{
			AddRow( clsList );
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );
}

bool CCiscoCDRDlg::ShowHeader( STRING_LIST & clsList )
{
	STRING_LIST::iterator itList;
	const char * pszText;
	int iCol = 0;

	m_iDateTimeOriginationIndex = -1;
	m_iDateTimeConnectIndex = -1;
	m_iDateTimeDisconnectIndex = -1;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		pszText = itList->c_str();

		if( !strcmp( pszText, "dateTimeOrigination" ) )
		{
			m_iDateTimeOriginationIndex = iCol;
		}
		else if( !strcmp( pszText, "dateTimeConnect" ) )
		{
			m_iDateTimeConnectIndex = iCol;
		}
		else if( !strcmp( pszText, "dateTimeDisconnect" ) )
		{
			m_iDateTimeDisconnectIndex = iCol;
		}

		m_clsCdrList.InsertColumn( iCol, pszText, LVCFMT_LEFT, gclsSetup.GetInt( "cdr", iCol, 50 ) );
		++iCol;
	}

	return true;
}

void CCiscoCDRDlg::AddRow( STRING_LIST & clsList )
{
	STRING_LIST::iterator itList;
	int iCol = 0;

	int iRow = m_clsCdrList.GetItemCount();

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( iCol == m_iDateTimeOriginationIndex || iCol == m_iDateTimeConnectIndex || iCol == m_iDateTimeDisconnectIndex )
		{
			time_t iTime = atoi( itList->c_str() );
			struct tm	sttTm;
			char szTime[31];

			localtime_s( &sttTm, &iTime );

			_snprintf( szTime, sizeof(szTime), "%04d/%02d/%02d %02d:%02d:%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
				, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );

			if( iCol == 0 )
			{
				m_clsCdrList.InsertItem( iRow, szTime );
			}
			else
			{
				m_clsCdrList.SetItemText( iRow, iCol, szTime );
			}
		}
		else
		{
			if( iCol == 0 )
			{
				m_clsCdrList.InsertItem( iRow, itList->c_str() );
			}
			else
			{
				m_clsCdrList.SetItemText( iRow, iCol, itList->c_str() );
			}
		}

		++iCol;
	}
}
