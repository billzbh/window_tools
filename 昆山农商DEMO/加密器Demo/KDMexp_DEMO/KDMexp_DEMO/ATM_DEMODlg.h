// ATM_DEMODlg.h : header file
//

#if !defined(AFX_ATM_DEMODLG_H__B92755C0_80FB_4721_837D_A209EAB4540D__INCLUDED_)
#define AFX_ATM_DEMODLG_H__B92755C0_80FB_4721_837D_A209EAB4540D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CATM_DEMODlg dialog

class CATM_DEMODlg : public CDialog
{
// Construction
public:
	static void ShowTmkInfo(void);
	static void ClearRand(void);
	static void Log(CString info);
	void GetCommNum(char *com_buff);
	CATM_DEMODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CATM_DEMODlg)
	enum { IDD = IDD_ATM_DEMO_DIALOG };
	CEdit	m_tmklen;
	CEdit	m_TermCode;
	CEdit	m_check;
	CEdit	m_Rand;
	CComboBox	m_Comm;
	CButton	m_bntExcute;
	CEdit	m_editLog;
	CString	m_rand;
	CString	m_Check;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATM_DEMODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CATM_DEMODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeComm();
	afx_msg void OnExcute();
	afx_msg void OnClearLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATM_DEMODLG_H__B92755C0_80FB_4721_837D_A209EAB4540D__INCLUDED_)
