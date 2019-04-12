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
#include "SetupDlg.h"
#include "Setup.h"

// CSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
	, m_iCropWidth(0)
	, m_iCropHeight(0)
	, m_strCropOutputFolder(_T(""))
	, m_bUseThumbnail(FALSE)
	, m_iThumbnailWidth(0)
	, m_iThumbnailHeight(0)
	, m_strThumbnailPrefix(_T(""))
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CROP_WIDTH, m_iCropWidth);
	DDX_Text(pDX, IDC_CROP_HEIGHT, m_iCropHeight);
	DDX_Text(pDX, IDC_CROP_OUTPUT_FOLDER, m_strCropOutputFolder);
	DDX_Check(pDX, IDC_USE_THUMBNAIL, m_bUseThumbnail);
	DDX_Text(pDX, IDC_THUMBNAIL_WIDTH, m_iThumbnailWidth);
	DDX_Text(pDX, IDC_THUMBNAIL_HEIGHT, m_iThumbnailHeight);
	DDX_Text(pDX, IDC_THUMBNAIL_PREFIX, m_strThumbnailPrefix);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SELECT_OUTPUT_FOLDER, &CSetupDlg::OnBnClickedSelectOutputFolder)
END_MESSAGE_MAP()


// CSetupDlg 메시지 처리기입니다.

BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_iCropWidth = gclsSetup.m_iCropWidth;
	m_iCropHeight = gclsSetup.m_iCropHeight;
	m_strCropOutputFolder = gclsSetup.m_strOutputFolderPath.c_str();

	m_bUseThumbnail = gclsSetup.m_bUseThumbnail ? TRUE : FALSE;
	m_iThumbnailWidth = gclsSetup.m_iThumbnailWidth;
	m_iThumbnailHeight = gclsSetup.m_iThumbnailHeight;
	m_strThumbnailPrefix = gclsSetup.m_strThumbnailPrefix.c_str();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if( m_iCropWidth <= 0 )
	{
		MessageBox( "Invalid crop image width" );
		return;
	}

	if( m_iCropHeight <= 0 )
	{
		MessageBox( "Invalid crop image height" );
		return;
	}

	if( m_strCropOutputFolder.IsEmpty() )
	{
		MessageBox( "Invalid crop output folder" );
		return;
	}

	if( m_bUseThumbnail )
	{
		if( m_iThumbnailWidth <= 0 )
		{
			MessageBox( "Invalid thumbnail image width" );
			return;
		}

		if( m_iThumbnailHeight <= 0 )
		{
			MessageBox( "Invalid thumbnail image height" );
			return;
		}

		if( m_strThumbnailPrefix.IsEmpty() )
		{
			MessageBox( "Invalid thumbnail prefix" );
			return;
		}
	}

	gclsSetup.m_iCropWidth = m_iCropWidth;
	gclsSetup.m_iCropHeight = m_iCropHeight;
	gclsSetup.m_strOutputFolderPath = m_strCropOutputFolder;

	gclsSetup.m_bUseThumbnail = m_bUseThumbnail ? true : false;
	gclsSetup.m_iThumbnailWidth = m_iThumbnailWidth;
	gclsSetup.m_iThumbnailHeight = m_iThumbnailHeight;
	gclsSetup.m_strThumbnailPrefix = m_strThumbnailPrefix;

	gclsSetup.Put();

	OnOK();
}

void CSetupDlg::OnBnClickedSelectOutputFolder()
{
	BROWSEINFO sttInfo;
	char szFolder[1024];

	memset( &sttInfo, 0, sizeof(sttInfo) );
	memset( szFolder, 0, sizeof(szFolder) );

  sttInfo.hwndOwner = GetSafeHwnd();
  sttInfo.lpszTitle = "Select crop image save folder";
  sttInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS;
  LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&sttInfo);
  ::SHGetPathFromIDList( pItemIdList, szFolder );

	m_strCropOutputFolder = szFolder;

	UpdateData(FALSE);
}
