#pragma once

#define M_TIME_SHOWN 40
// CMyVCRSplash dialog

class CMyVCRSplash : public CDialog
{
	DECLARE_DYNAMIC(CMyVCRSplash)

public:
	CMyVCRSplash(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyVCRSplash();

// Dialog Data
	enum { IDD = IDD_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	DWORD m_dwTime;
};
