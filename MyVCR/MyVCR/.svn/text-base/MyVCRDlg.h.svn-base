// MyVCRDlg.h : header file
//

#pragma once
#include "Cortez.h"
#include "paramdlg.h"
#include "afxwin.h"
#include "MyVCRSplash.H"
#include "afxcmn.h"

#ifdef _DEBUG
   #define new MYDEBUG_NEW
#endif

// CMyVCRDlg dialog
class CMyVCRDlg : public CDialog
{
// Construction
public:
	CMyVCRDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYVCR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Cortez m_cortez;
public:
	afx_msg void OnBnBeginClicked();
public:
	afx_msg void OnBnStopClicked();
public:
	afx_msg void OnBnPauseClicked();
public:
	afx_msg void OnBnOptionClicked();
public:
	afx_msg void OnBnPreviewClicked();
public:
	CParamDlg m_ParamDlg;
public:
	CButton m_BtnBegin;
public:
	CButton m_BtnStop;
public:
	CButton m_BtnPause;
public:
	CButton m_BtnPreview;
public:
	CButton m_BtnOption;
public:
	CStatic m_Line1;
public:
	CStatic m_Line2;
public:
	CStatic m_Status;
public:
	afx_msg void OnMenuExit();
public:
	afx_msg void OnClose();
public:
	afx_msg void OnMenuStartCapture();
public:
	afx_msg void OnMenuPauseCapture();
public:
	afx_msg void OnMenuStopCapture();
public:
	afx_msg void OnMenuOption();
public:
	afx_msg void OnMenuHowDoI();
public:
	afx_msg void OnMenuAboutMe();
public:
	bool resizeWindow(void);
public:
	void setStatusString(int nMsg);
public:
	bool getOutputFile(CString& strBuff);
public:
	void setWindowOnTop();
public:
	afx_msg void OnStnClickedStatus();
public:
	afx_msg void OnNMClickSyslinkSite(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CLinkCtrl m_LnkMySite;
};
