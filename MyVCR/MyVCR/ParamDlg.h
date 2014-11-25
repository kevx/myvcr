#pragma once
#include "afxwin.h"
#include "Cortez.h"
#include "afxcmn.h"

#define M_VALID_SCALE 0.75
// CParamDlg dialog

class CParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamDlg)

public:
	CParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParamDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CArray<CString> m_szDevLst;
public:
	CArray<CString> m_szDevLstAu;
public:
	DWORD m_dwDevNum;
public:
	DWORD m_dwDevNumAu;
public:
	Resolution* m_ResolutionHigh;
	Resolution* m_ResolutionLow;
	Resolution* m_CurrResolution;
	void addResolution(Resolution* low, Resolution* high, Resolution* curr);
	void setResolution(Resolution* curr);
	Resolution* getResolution();

public:
	bool getParams(void);
public:
	afx_msg void OnBnClickedOk();
	bool bNoErr;
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_CbDevLst;
public:
	DWORD m_CurDev;
public:
	afx_msg void OnCbnSelchangeComboDevlst();
public:
	afx_msg void OnBnClickedCheckOntop();
public:
	CComboBox m_CbDevLstAu;
public:
	afx_msg void OnCbnSelchangeComboDevlstAu();
public:
	DWORD m_CurDevAu;
public:
	afx_msg void OnBnClickedCheckAudio();
public:
	afx_msg void OnBnClickedCheckLang();
	void setDeviceList(CArray<CString>& deviceList);
	void setDeviceListAu(CArray<CString>& deviceList);
private:
	CComboBox m_CbResolution;
public:
	afx_msg void OnCbnSelchangeComboResolution();
private:
	CListCtrl m_ListCtrlPlugin;
};
