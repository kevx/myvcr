// MyVCRSplash.cpp : implementation file
//

#include "stdafx.h"
#include "MyVCR.h"
#include "MyVCRSplash.h"


// CMyVCRSplash dialog

IMPLEMENT_DYNAMIC(CMyVCRSplash, CDialog)

CMyVCRSplash::CMyVCRSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CMyVCRSplash::IDD, pParent)
	, m_dwTime(0)
{

}

CMyVCRSplash::~CMyVCRSplash()
{
}

void CMyVCRSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyVCRSplash, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyVCRSplash message handlers

BOOL CMyVCRSplash::OnInitDialog()
{
	CDialog::OnInitDialog();
    
	::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetTimer( 1, 32, NULL );
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMyVCRSplash::OnTimer(UINT_PTR nIDEvent)
{
	
	m_dwTime ++;
	if( m_dwTime == M_TIME_SHOWN )
	{
		KillTimer( 1 );
		//DestroyWindow();
		SendMessage( WM_CLOSE );
	}
	else
	{
		::SetLayeredWindowAttributes(this->GetSafeHwnd(),0, m_dwTime * 5 + 50 ,LWA_ALPHA);
	}
	CDialog::OnTimer(nIDEvent);
}
