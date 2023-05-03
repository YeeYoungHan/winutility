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
#include "CiscoCDRDlg.h"
#include "ColumnDlg.h"
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
	ON_BN_CLICKED(IDC_COLUMN, &CCiscoCDRDlg::OnBnClickedColumn)
	ON_NOTIFY(NM_DBLCLK, IDC_CDR_LIST, &CCiscoCDRDlg::OnNMDblclkCdrList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CDR_LIST, &CCiscoCDRDlg::OnNMCustomdrawCdrList)
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

	m_clsCdrList.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT ); 

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
		m_strFileName = clsDlg.GetPathName();
	}
}

void CCiscoCDRDlg::OnBnClickedColumn()
{
	CColumnDlg clsDlg;

	if( clsDlg.DoModal() == IDOK && m_strFileName.empty() == false )
	{
		SaveColumn();
		Show( m_strFileName.c_str() );
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
	SaveColumn();

	CDialog::OnDestroy();
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
	int iLine = 0, iLen;
	STRING_LIST clsList;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		++iLine;

		iLen = strlen(szBuf);
		if( iLen >= 2 && szBuf[iLen-2] == '\r' )
		{
			szBuf[iLen-2] = '\0';
		}
		else if( iLen >= 1 && szBuf[iLen-1] == '\n' )
		{
			szBuf[iLen-1] = '\0';
		}

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
	int iCol = 0, iIndex = 0;

	m_iDateTimeOriginationIndex = -1;
	m_iDateTimeConnectIndex = -1;
	m_iDateTimeDisconnectIndex = -1;

	gclsColList = clsList;

	m_clsCdrList.InsertColumn( 0, "line", LVCFMT_LEFT, gclsSetup.GetInt( "cdr", iCol, 50 ) );
	++iCol;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		pszText = itList->c_str();

		if( !strcmp( pszText, "dateTimeOrigination" ) )
		{
			m_iDateTimeOriginationIndex = iIndex;
		}
		else if( !strcmp( pszText, "dateTimeConnect" ) )
		{
			m_iDateTimeConnectIndex = iIndex;
		}
		else if( !strcmp( pszText, "dateTimeDisconnect" ) )
		{
			m_iDateTimeDisconnectIndex = iIndex;
		}

		if( gclsSetup.GetInt( itList->c_str(), 0 ) == 1 )
		{
			m_clsNoShowMap.insert( INT_MAP::value_type( iIndex, iIndex ) );
		}
		else
		{
			m_clsCdrList.InsertColumn( iCol, pszText, LVCFMT_LEFT, gclsSetup.GetInt( "cdr", iCol, 50 ) );
			++iCol;
		}

		++iIndex;
	}

	return true;
}

void CCiscoCDRDlg::AddRow( STRING_LIST & clsList )
{
	INT_MAP::iterator itMap;
	STRING_LIST::iterator itList;
	int iCol = 0, iIndex = 0;
	char szLine[11];

	int iRow = m_clsCdrList.GetItemCount();

	_snprintf( szLine, sizeof(szLine), "%d", iRow + 2 );
	m_clsCdrList.InsertItem( iRow, szLine );
	++iCol;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		itMap = m_clsNoShowMap.find( iIndex );
		if( itMap == m_clsNoShowMap.end() )
		{
			if( iIndex == m_iDateTimeOriginationIndex || iIndex == m_iDateTimeConnectIndex || iIndex == m_iDateTimeDisconnectIndex )
			{
				const char * pszText = itList->c_str();

				if( !strcmp( pszText, "" ) || !strcmp( pszText, "0" ) )
				{
					m_clsCdrList.SetItemText( iRow, iCol, "" );
				}
				else
				{
					time_t iTime = atoi( itList->c_str() );
					struct tm	sttTm;
					char szTime[31];

					localtime_s( &sttTm, &iTime );

					_snprintf( szTime, sizeof(szTime), "%04d/%02d/%02d %02d:%02d:%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
						, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );

					m_clsCdrList.SetItemText( iRow, iCol, szTime );
				}
			}
			else
			{
				m_clsCdrList.SetItemText( iRow, iCol, itList->c_str() );
			}

			++iCol;
		}

		++iIndex;
	}
}

void CCiscoCDRDlg::SaveColumn()
{
	CHeaderCtrl * pclsHeaderCtrl = m_clsCdrList.GetHeaderCtrl();
	int i, iCount = pclsHeaderCtrl->GetItemCount();

	for( i = 0; i < iCount; ++i )
	{
		gclsSetup.PutInt( "cdr", i, m_clsCdrList.GetColumnWidth( i ) );
	}

	gclsSetup.PutFile();
}

void CCiscoCDRDlg::OnNMDblclkCdrList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if( pNMItemActivate->iItem != -1 )
	{
		m_strSearch = m_clsCdrList.GetItemText( pNMItemActivate->iItem, pNMItemActivate->iSubItem );
		m_clsCdrList.Invalidate();
	}

	*pResult = 0;
}

void CCiscoCDRDlg::OnNMCustomdrawCdrList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if( pNMCD->dwDrawStage == CDDS_PREPAINT )
	{
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW; 
		return;
	}

	if( pNMCD->dwDrawStage == CDDS_ITEMPREPAINT )
	{
		*pResult = (LRESULT)CDRF_NOTIFYSUBITEMDRAW;
		return;
	}

	if( pNMCD->dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT) && m_strSearch.IsEmpty() == false )
	{
		NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR); 
		CString strText = m_clsCdrList.GetItemText( pNMCD->dwItemSpec, pDraw->iSubItem );

		if( m_strSearch == strText )
		{
			pDraw->clrText = RGB( 255, 0, 0 );
			pDraw->clrTextBk = RGB( 255, 255, 128 );
		}
		else
		{
			pDraw->clrText = RGB( 0, 0, 0 );
			pDraw->clrTextBk = RGB( 255, 255, 255 );
		}
		
		*pResult = (LRESULT)CDRF_NEWFONT;

		return;
	}

	*pResult = 0;
}
