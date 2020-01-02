
// wdmTestAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "wdmTestApp.h"
#include "wdmTestAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CwdmTestAppDlg dialog



CwdmTestAppDlg::CwdmTestAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WDMTESTAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDevice = NULL;
}

void CwdmTestAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WRITE, m_ctrlWriteEdit);
}

BEGIN_MESSAGE_MAP(CwdmTestAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CwdmTestAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CwdmTestAppDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CwdmTestAppDlg message handlers

BOOL CwdmTestAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CwdmTestAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CwdmTestAppDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CwdmTestAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwdmTestAppDlg::OnBnClickedButton1()
{
	//Using CFile Class~~
	// TODO: Add your control notification handler code here
	if (m_DeviceFile.m_hFile == CFile::hFileNull)
	{

		if (m_DeviceFile.Open(_T("\\\\.\\WDMHello"), CFile::modeReadWrite | CFile::typeBinary))
		{
			AfxMessageBox(_T("Open!!Device"));

		}
		else
			AfxMessageBox(_T("Open!!fail Device"));
	
	}
	else
	{
		AfxMessageBox(_T("Close"));
		m_DeviceFile.Close();
	}
	
}


void CwdmTestAppDlg::OnBnClickedButton2()
{
	if (m_DeviceFile.m_hFile == CFile::hFileNull)
	{
		return;
	}

	if (m_ctrlWriteEdit.GetWindowTextLengthW())
	{
		CString str;
		m_ctrlWriteEdit.GetWindowText(str);
		DWORD dwAlloc = str.GetLength() + 1;
		PWCHAR pBuf = new WCHAR[dwAlloc];
		ZeroMemory(pBuf, sizeof(WCHAR) * dwAlloc);
		wcsncpy_s(pBuf, dwAlloc, str.GetBuffer(), _TRUNCATE);
		//pBuf[dwAlloc-1] = 0;
		
		m_DeviceFile.Write(pBuf, sizeof(WCHAR) * dwAlloc);
		delete[] pBuf;
		AfxMessageBox(_T("write!!!"));
	}

}
