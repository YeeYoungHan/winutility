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
#include "CropImage.h"
#include "CropImageDlg.h"
#include "Setup.h"
#include "SetupDlg.h"

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


// CCropImageDlg dialog




CCropImageDlg::CCropImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCropImageDlg::IDD, pParent)
	, m_strPage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iId = 0;
}

void CCropImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CROP_IMAGE_CONTROL, m_clsCropImage);
	DDX_Text(pDX, IDC_PAGE, m_strPage);
	DDX_Control(pDX, IDC_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_NEXT, m_btnNext);
}

BEGIN_MESSAGE_MAP(CCropImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCropImageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCropImageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SETUP, &CCropImageDlg::OnBnClickedSetup)
	ON_BN_CLICKED(IDC_OPEN_FILE, &CCropImageDlg::OnBnClickedOpenFile)
	ON_BN_CLICKED(IDC_OPEN_DIR, &CCropImageDlg::OnBnClickedOpenDir)
	ON_BN_CLICKED(IDC_RESET_NO, &CCropImageDlg::OnBnClickedResetNo)
	ON_BN_CLICKED(IDC_PREV, &CCropImageDlg::OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, &CCropImageDlg::OnBnClickedNext)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CCropImageDlg::ShowPage()
{
	char szText[40];

	if( m_clsFileList.empty() )
	{
		_snprintf( szText, sizeof(szText), "1/1" );
	}
	else
	{
		_snprintf( szText, sizeof(szText), "%d/%d", m_iFileIndex + 1, (int)m_clsFileList.size() );
	}

	m_strPage = szText;

	UpdateData( FALSE );
}

void CCropImageDlg::ShowFileList()
{
	m_iFileIndex = 0;

	int iCount = m_clsFileList.size();
	if( iCount == 0 )
	{
		MessageBox( "There is no image file in folder." );
	}
	else
	{
		if( m_clsCropImage.SetFile( m_clsFileList[0].c_str() ) == false )
		{
			MessageBox( "Read image file error." );
		}

		if( iCount == 1 )
		{
			m_clsFileList.clear();
			m_btnPrev.EnableWindow( FALSE );
			m_btnNext.EnableWindow( FALSE );
		}
		else
		{
			m_btnPrev.EnableWindow( FALSE );
			m_btnNext.EnableWindow( TRUE );
		}
	}

	ShowPage();
}

// CCropImageDlg message handlers

BOOL CCropImageDlg::OnInitDialog()
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

	DragAcceptFiles( TRUE );

	gclsSetup.Get();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCropImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCropImageDlg::OnPaint()
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
HCURSOR CCropImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCropImageDlg::OnBnClickedOk()
{
	if( gclsSetup.m_strOutputFolderPath.empty() )
	{
		MessageBox( "Output folder is not selected. Click Setup button and set output folder." );
		return;
	}

	if( m_clsCropImage.IsLoad() == false )
	{
		MessageBox( "Image file is not opened. Click Open file button." );
		return;
	}

	char szId[11];

	if( m_iId == 0 )
	{
		m_iId = gclsSetup.m_iCropFileStartNo;
	}
	else
	{
		++m_iId;
	}
	
	_snprintf( szId, sizeof(szId), "%d", m_iId );

	std::string strFilePath = gclsSetup.m_strOutputFolderPath;
	strFilePath.append( "\\" );
	strFilePath.append( szId );
	strFilePath.append( ".png" );

	if( m_clsCropImage.SaveFile( strFilePath.c_str() ) == false )
	{
		MessageBox( "Save error!!!" );
		return;
	}

	if( gclsSetup.m_bUseThumbnail )
	{
		std::string strThumbnailFilePath = gclsSetup.m_strOutputFolderPath;
		strThumbnailFilePath.append( "\\" );
		strThumbnailFilePath.append( gclsSetup.m_strThumbnailPrefix );
		strThumbnailFilePath.append( szId );
		strThumbnailFilePath.append( ".png" );

		ResizeImage( strFilePath.c_str(), strThumbnailFilePath.c_str(), gclsSetup.m_iThumbnailWidth, gclsSetup.m_iThumbnailHeight );
	}

	if( m_clsFileList.empty() == false )
	{
		OnBnClickedNext();
	}
}

void CCropImageDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CCropImageDlg::OnBnClickedSetup()
{
	CSetupDlg clsDlg;

	if( clsDlg.DoModal() == IDOK )
	{
		m_clsCropImage.Update();
	}
}

void CCropImageDlg::OnBnClickedOpenFile()
{
	CFileDialog clsDlg( TRUE, NULL, NULL, 0, "Image file(*.jpg, *.png)|*jpg;*.jpeg;*png||" );
	if( clsDlg.DoModal() == IDOK )
	{
		if( m_clsCropImage.SetFile( clsDlg.GetPathName() ) == false )
		{
			MessageBox( "Read image file error." );
		}
		else
		{
			m_clsFileList.clear();
			m_btnPrev.EnableWindow( FALSE );
			m_btnNext.EnableWindow( FALSE );
			ShowPage();
		}
	}
}

void CCropImageDlg::OnBnClickedOpenDir()
{
	std::string strFolder;

	if( SelectFolder( strFolder ) )
	{
		GetFileList( strFolder.c_str(), m_clsFileList );
		ShowFileList();
	}
}

void CCropImageDlg::OnBnClickedResetNo()
{
	m_iId = 0;
}

void CCropImageDlg::OnBnClickedPrev()
{
	if( m_iFileIndex > 0 )
	{
		--m_iFileIndex;
		if( m_clsCropImage.SetFile( m_clsFileList[m_iFileIndex].c_str() ) == false )
		{
			MessageBox( "Read image file error." );
		}

		ShowPage();

		if( m_iFileIndex == 0 )
		{
			m_btnPrev.EnableWindow( FALSE );
		}

		m_btnNext.EnableWindow( TRUE );
	}
}

void CCropImageDlg::OnBnClickedNext()
{
	if( ( m_iFileIndex + 1 ) < (int)m_clsFileList.size() )
	{
		++m_iFileIndex;
		if( m_clsCropImage.SetFile( m_clsFileList[m_iFileIndex].c_str() ) == false )
		{
			MessageBox( "Read image file error." );
		}

		ShowPage();

		if( ( m_iFileIndex + 1 ) == (int)m_clsFileList.size() )
		{
			m_btnNext.EnableWindow( FALSE );
		}

		m_btnPrev.EnableWindow( TRUE );
	}
}

void CCropImageDlg::OnDropFiles(HDROP hDropInfo)
{
	m_clsFileList.clear();

	DWORD dwCount = DragQueryFile( hDropInfo, 0xFFFFFFFF, NULL, 0 );
	char szFileName[2048];

	for( DWORD i = 0; i < dwCount; ++i )
	{
		DragQueryFile( hDropInfo, i, szFileName, sizeof(szFileName) );
		if( IsImageFile( szFileName ) )
		{
			m_clsFileList.push_back( szFileName );
		}
	}

	ShowFileList();

	CDialog::OnDropFiles(hDropInfo);
}
