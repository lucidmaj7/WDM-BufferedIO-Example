
// wdmTestAppDlg.h : header file
//

#pragma once

#include <devioctl.h>
#define IOCTL_MY_CTL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4000, METHOD_BUFFERED , FILE_ANY_ACCESS)

// CwdmTestAppDlg dialog
class CwdmTestAppDlg : public CDialogEx
{
// Construction
public:
	CwdmTestAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WDMTESTAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CFile m_DeviceFile;
	HANDLE m_hDevice;
	CEdit m_ctrlWriteEdit;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
