// MyVCRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyVCR.h"
#include "MyVCRDlg.h"
#include "BuildCfg.h"

// CMyVCRDlg dialog

CMyVCRDlg::CMyVCRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyVCRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyVCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BEGIN, m_BtnBegin);
	DDX_Control(pDX, IDC_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_PAUSE, m_BtnPause);
	DDX_Control(pDX, IDC_PREVIEW, m_BtnPreview);
	DDX_Control(pDX, IDC_OPTION, m_BtnOption);
	DDX_Control(pDX, IDC_STATIC_L1, m_Line1);
	DDX_Control(pDX, IDC_STATIC_L2, m_Line2);
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_WEBLINK_SITE, m_LnkMySite);
}

BEGIN_MESSAGE_MAP(CMyVCRDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BEGIN, &CMyVCRDlg::OnBnBeginClicked)
	ON_BN_CLICKED(IDC_STOP, &CMyVCRDlg::OnBnStopClicked)
	ON_BN_CLICKED(IDC_PAUSE, &CMyVCRDlg::OnBnPauseClicked)
	ON_BN_CLICKED(IDC_PREVIEW, &CMyVCRDlg::OnBnPreviewClicked)
	ON_BN_CLICKED(IDC_OPTION, &CMyVCRDlg::OnBnOptionClicked)

	ON_COMMAND(IDM_EXIT, &CMyVCRDlg::OnMenuExit)
	ON_COMMAND(IDM_BEGIN_CAPTURE, &CMyVCRDlg::OnMenuStartCapture)
	ON_COMMAND(IDM_PAUSE_CAPTURE, &CMyVCRDlg::OnMenuPauseCapture)
	ON_COMMAND(IDM_STOP_CAPTURE, &CMyVCRDlg::OnMenuStopCapture)
	ON_COMMAND(IDM_OPTION, &CMyVCRDlg::OnMenuOption)
	ON_COMMAND(IDM_HOWDOI, &CMyVCRDlg::OnMenuHowDoI)
	ON_COMMAND(IDM_ABOUTME, &CMyVCRDlg::OnMenuAboutMe)
	
	ON_STN_CLICKED(IDC_STATUS, &CMyVCRDlg::OnStnClickedStatus)
	ON_NOTIFY(NM_CLICK, IDC_WEBLINK_SITE, &CMyVCRDlg::OnNMClickSyslinkSite)
END_MESSAGE_MAP()


// CMyVCRDlg message handlers

BOOL CMyVCRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CArray<CString> deviceListTemp;

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Load button icons
	////////////////////
	HICON hIcon = AfxGetApp()->LoadIconW(IDI_BTN_BEGIN);
	m_BtnBegin.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIconW(IDI_BTN_STOP);
	m_BtnStop.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIconW(IDI_BTN_PAUSE);
	m_BtnPause.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIconW(IDI_BTN_PREVIEW);
	m_BtnPreview.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIconW(IDI_BTN_OPTION);
	m_BtnOption.SetIcon(hIcon);
	/////////////////////
	
	//no default video device ?
	/////////////////////
	CString strErrMsg(theApp.getResStr(IDS_ERR_MSG_TITLE));
	bool willShowErrMsg = false;
	int ret = m_cortez.initDevice();
	if( ret & NO_VIDEO_DEVICE )
	{
		strErrMsg += theApp.getResStr(IDS_STR_BIGERR);
		theApp.unableToCapture = true;
		willShowErrMsg = true;
	}
	else
	{
		m_cortez.getDeviceList(deviceListTemp);
		m_ParamDlg.setDeviceList(deviceListTemp);
	}
	/////////////////////

	//no default audio device ?
	/////////////////////
	if( ret & NO_AUDIO_DEVICE )
	{
		strErrMsg += theApp.getResStr(IDS_STR_NOAU);
		willShowErrMsg = true;
	}
	else
	{
		m_cortez.getDeviceListAu(deviceListTemp);
		m_ParamDlg.setDeviceListAu(deviceListTemp);
	}

	if(willShowErrMsg)
	{
		strErrMsg += theApp.getResStr(IDS_ERR_HELP);
		MessageBox(strErrMsg.GetBuffer(), L"ERROR", MB_ICONQUESTION);
	}
	/////////////////////	

	m_ParamDlg.addResolution(
		m_cortez.getLowResolution(), 
		m_cortez.getHighResolution(),
		m_cortez.getCurrentResolution()
	);
	CMyVCRDlg::resizeWindow();

	//Localize the UI
	////////////////////
	setStatusString(IDS_STR_READY);
	GetDlgItem(IDC_WEBLINK_SITE)->SetWindowTextW( theApp.getResStr(IDS_WEBLINK_SITE));
	//then we load corresponding menus
	CMenu cm;
	cm.m_hMenu = ::LoadMenu( theApp.hLang, MAKEINTRESOURCE(IDR_MENU_TOP) );
	SetMenu( &cm );
	////////////////////

	return TRUE;
}

void CMyVCRDlg::OnPaint()
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
HCURSOR CMyVCRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMyVCRDlg::getOutputFile(CString& strDefault)
{
	DWORD dwFlag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	m_cortez.buildDefaultFileName(strDefault);
	CFileDialog dlg( 0, L"avi", strDefault, dwFlag,     
		L"AVI Video File(*.avi)|*.avi|ALL File(*.*)|*.*||", NULL);
	
	dlg.m_ofn.lpstrTitle = theApp.getResStr(IDS_WHERE_TO_SAVE);
	if(dlg.DoModal() == IDCANCEL)
		return false;
	
	if(dlg.GetPathName() == L"")
		return false;
	else
	{
		if( _waccess( dlg.GetPathName().GetBuffer(), 00) )
			DeleteFile( dlg.GetPathName().GetBuffer() );
	}

	strDefault.SetString(dlg.GetPathName());
	return true;
}

void CMyVCRDlg::OnBnBeginClicked()
{
	CString strBuff = L"";
	if( theApp.unableToCapture )
		return;

	m_BtnBegin.EnableWindow(false);
	if(m_cortez.isInCaptureProgress())
	{
		if(m_cortez.isCapturePaused())
		{
			m_cortez.resumeCapture();
			setStatusString(IDS_STR_CAP);
		}
	} 
	else 
	{
		m_cortez.setAudioDisabled(!theApp.m_withAudio);	
		if(getOutputFile(strBuff))
		{
			m_cortez.setOutputFile(strBuff);
			m_cortez.bind2deviceAu(m_ParamDlg.m_CurDevAu);
			m_cortez.bind2device(m_ParamDlg.m_CurDev);
			m_cortez.bind2ui( this );	
			if(m_cortez.beginCapture())
			{
				setStatusString(IDS_STR_CAP);
			}
		}
	}

	m_BtnBegin.EnableWindow(true);
	this->SetFocus();
}

void CMyVCRDlg::OnBnStopClicked()
{
	m_BtnStop.EnableWindow(false);
	m_cortez.stopCapture();
	setStatusString(IDS_STR_READY);

	this->SetFocus();
	m_BtnStop.EnableWindow(true);
}

void CMyVCRDlg::OnBnPauseClicked()
{
	if( m_cortez.isInCaptureProgress() )
	{
		m_cortez.pauseCapture();
		setStatusString(IDS_STR_PAUSE);
	}
}

void CMyVCRDlg::OnBnOptionClicked()
{
	m_ParamDlg.getParams();
	this->setWindowOnTop();

	if( m_cortez.isInCaptureProgress() && m_cortez.getCurrentResolution() != m_ParamDlg.getResolution())
	{
		MessageBox(theApp.getResStr(IDS_STR_RNC), 0, MB_ICONWARNING);
		m_ParamDlg.setResolution(m_cortez.getCurrentResolution());
	} else {
		m_cortez.setVideoResolution(m_ParamDlg.getResolution());
		this->resizeWindow();
	}
}

void CMyVCRDlg::setWindowOnTop()
{
	//Always on top ?
	if(theApp.m_alwaysOnTop)
		SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	else
		SetWindowPos(&this->wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	////////////////////
}

void CMyVCRDlg::OnMenuExit()
{
	CMyVCRDlg::OnBnStopClicked();
	::FreeLibrary( theApp.hLang );
	OnOK();
}

void CMyVCRDlg::OnClose()
{
	CMyVCRDlg::OnMenuExit();
	//CDialog::OnClose();
}

void CMyVCRDlg::OnMenuStartCapture()
{
	CMyVCRDlg::OnBnBeginClicked();
}

void CMyVCRDlg::OnMenuPauseCapture()
{
	CMyVCRDlg::OnBnPauseClicked();
}

void CMyVCRDlg::OnMenuStopCapture()
{
	CMyVCRDlg::OnBnStopClicked();
}

bool CMyVCRDlg::resizeWindow(void)
{
	CString t;
	RECT r;
	DWORD dwVideoWidth = m_cortez.getCurrentResolution()->getWidth();//0
	DWORD dwVideoHeight = m_cortez.getCurrentResolution()->getHeight();//1
	
	//The following hard-coded magic numbers come from experiment
	this->GetWindowRect(&r);
	this->MoveWindow(r.left, r.top, dwVideoWidth + 25, dwVideoHeight + 180);

	CWnd* ctlArray[] = {
		&m_BtnBegin, 
		&m_BtnPause,
		&m_BtnStop,
		&m_BtnPreview,
		&m_BtnOption,
	};
	for(DWORD dwI = 0; dwI < 5; dwI++) {
		ctlArray[dwI]->GetWindowRect(&r);
		ScreenToClient(&r);
		r.bottom = dwVideoHeight + 50;
		ctlArray[dwI]->MoveWindow(r.left, r.bottom, 39, 36);
	}
	
	m_Line1.SetWindowPos(NULL, 0, 0, dwVideoWidth, 1, SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
	m_Line2.GetWindowRect(&r);
	ScreenToClient(&r);
	m_Line2.MoveWindow(r.left, dwVideoHeight + 30, dwVideoWidth, 2);

	m_Status.GetWindowRect(&r);
	ScreenToClient(&r);
	r.bottom = dwVideoHeight + 90;
	m_Status.MoveWindow(r.left, r.bottom, 120, 15);	

	m_LnkMySite.GetWindowRect(&r);
	ScreenToClient(&r);
	r.bottom = dwVideoHeight + 90;
	m_LnkMySite.MoveWindow(dwVideoWidth * 0.8, r.bottom, 100,15);

	wchar_t* bmpToShow = dwVideoWidth > 320 ? 
		MAKEINTRESOURCE(IDB_BITMAP_STANDBY_HIGH):
		MAKEINTRESOURCE(IDB_BITMAP_STANDBY);
	
	HBITMAP hOldbmp = ((CStatic*)GetDlgItem(IDC_STANDBY))->
		SetBitmap(::LoadBitmap(AfxGetInstanceHandle(), bmpToShow));
	if(hOldbmp != NULL)
		DeleteObject(hOldbmp);

	return true;
}

void CMyVCRDlg::OnMenuOption()
{
	CMyVCRDlg::OnBnOptionClicked();
}

void CMyVCRDlg::setStatusString(int nMsg)
{
	CString &szText = theApp.getResStr( nMsg );

	GetDlgItem(IDC_STATUS)->SetWindowTextW(szText);
}

void CMyVCRDlg::OnMenuHowDoI()
{
	CString szPth;
	szPth = theApp.getMyPath() + L"help.doc";
	ShellExecute( this->m_hWnd, L"Open", szPth, L"", L"", SW_SHOWNORMAL );
}

void CMyVCRDlg::OnMenuAboutMe()
{
	CString &szInfo = theApp.getResStr( IDS_STR_ABOUT );
	MessageBox( szInfo.GetBuffer(),L"About me...", MB_ICONINFORMATION );
}

void CMyVCRDlg::OnStnClickedStatus()
{

}

void CMyVCRDlg::OnNMClickSyslinkSite(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	ShellExecute(NULL,L"open",L"http://spinlock.blog.51cto.com/",NULL,NULL,SW_SHOWNORMAL);
}


void CMyVCRDlg::OnBnPreviewClicked()
{
	if( theApp.unableToCapture )
		return;

	m_BtnPreview.EnableWindow(false);
	if(m_cortez.isInCaptureProgress())
	{
		if(m_cortez.isCapturePaused())
		{
			m_cortez.resumeCapture();
			setStatusString(IDS_PREVIEWING);
		}
	} else {
		m_cortez.setAudioDisabled(true);
		m_cortez.bind2device(m_ParamDlg.m_CurDev);
		m_cortez.bind2ui( this );	
		m_cortez.beginPreview();
		setStatusString(IDS_PREVIEWING);
	}

	m_BtnPreview.EnableWindow(true);
}
