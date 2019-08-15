#include "Version.h"

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
public:
	CString m_strVersion;
	CString m_strBuild;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strVersion("PcapViewer, Version " PCAP_VIWER_VERSION)
, m_strBuild("Build " __DATE__ " " __TIME__ )
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDX_Text(pDX, IDC_BUILD_DATE, m_strBuild);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
