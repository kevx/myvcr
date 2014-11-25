// ParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyVCR.h"
#include "ParamDlg.h"
#include "MyVCRDlg.h"

// CParamDlg dialog

IMPLEMENT_DYNAMIC(CParamDlg, CDialog)

CParamDlg::CParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamDlg::IDD, pParent)
	, bNoErr(false)
	, m_dwDevNum(0)
	, m_CurDev(0)
	, m_dwDevNumAu(0)
	, m_CurDevAu(0)
{
}

CParamDlg::~CParamDlg()
{
}

void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVLST, m_CbDevLst);
	DDX_Control(pDX, IDC_COMBO_DEVLST_AU, m_CbDevLstAu);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_CbResolution);
	DDX_Control(pDX, IDC_PLUGIN, m_ListCtrlPlugin);
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CParamDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVLST, &CParamDlg::OnCbnSelchangeComboDevlst)
	ON_BN_CLICKED(IDC_CHECK_ONTOP, &CParamDlg::OnBnClickedCheckOntop)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVLST_AU, &CParamDlg::OnCbnSelchangeComboDevlstAu)
	ON_BN_CLICKED(IDC_CHECK_AUDIO, &CParamDlg::OnBnClickedCheckAudio)
	ON_BN_CLICKED(IDC_CHECK_LANG, &CParamDlg::OnBnClickedCheckLang)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &CParamDlg::OnCbnSelchangeComboResolution)
END_MESSAGE_MAP()


// CParamDlg message handlers

bool CParamDlg::getParams(void)
{
	this->DoModal();
	return true;
}

void CParamDlg::OnBnClickedOk()
{	
	for(DWORD dwIndex = 0; dwIndex < theApp.m_dwPluginCount; dwIndex++) {
		bool b = m_ListCtrlPlugin.GetCheck(dwIndex);
		theApp.m_plugins[dwIndex].setDisabled(
			!b
		);
	}
	theApp.saveSettings();
	m_ListCtrlPlugin.DeleteAllItems();
	OnOK();
}

BOOL CParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CbResolution.AddString(m_ResolutionLow->toString());
	m_CbResolution.AddString(m_ResolutionHigh->toString());
	m_CbResolution.SetCurSel(m_CurrResolution == m_ResolutionLow ? 0 : 1);

	for(DWORD dwIndex = 0; dwIndex < m_dwDevNum; dwIndex++)
		m_CbDevLst.AddString( m_szDevLst[dwIndex] );
	m_CbDevLst.SetCurSel(0);
	
	for(DWORD dwIndex = 0; dwIndex < m_dwDevNumAu; dwIndex++)
		m_CbDevLstAu.AddString( m_szDevLstAu[dwIndex] );
	m_CbDevLstAu.SetCurSel(0);

	//Load configuration from INI
	///////////////////
	((CButton*)GetDlgItem( IDC_CHECK_LANG ))->SetCheck(theApp.m_extLang > 0 ? true:false);
	((CButton*)GetDlgItem( IDC_CHECK_ONTOP ))->SetCheck(theApp.m_alwaysOnTop);	
	((CButton*)GetDlgItem( IDC_CHECK_AUDIO ))->SetCheck(theApp.m_withAudio);
	///////////////////
	if( m_dwDevNumAu > 0)
		((CButton*)GetDlgItem(IDC_CHECK_AUDIO))->SetCheck( theApp.m_withAudio );
	else
		((CButton*)GetDlgItem(IDC_CHECK_AUDIO))->EnableWindow( false );

	//Localize UI
	///////////////////
	GetDlgItem( IDC_SVD )->SetWindowTextW( theApp.getResStr( IDSC_SVD ) );
	GetDlgItem( IDC_SAD )->SetWindowTextW( theApp.getResStr( IDSC_SAD ) );
	GetDlgItem( IDC_SDR )->SetWindowTextW( theApp.getResStr( IDSC_SDR ) );
	GetDlgItem( IDC_CHECK_ONTOP )->SetWindowTextW( theApp.getResStr( IDSC_AOT ) );
	GetDlgItem( IDC_CHECK_AUDIO )->SetWindowTextW( theApp.getResStr( IDSC_CV ) );
	GetDlgItem( IDC_CHECK_LANG  )->SetWindowTextW( theApp.getResStr( IDSC_LANG ) );
	///////////////////

	//Load plugins status
	///////////////////
	if(!theApp.m_withRascal)
		m_ListCtrlPlugin.ShowWindow(SW_HIDE);
	
	m_ListCtrlPlugin.SetExtendedStyle(
		m_ListCtrlPlugin.GetExtendedStyle() | 
		LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ListCtrlPlugin.InsertColumn(0, L"Available Plugins", LVCFMT_LEFT, 180);
	for(DWORD dwIndex = 0; dwIndex < theApp.m_dwPluginCount; dwIndex++)
	{
		LVITEM item = {0};
		item.iItem = dwIndex; 
		item.mask = LVIF_TEXT;
		item.cchTextMax = theApp.m_plugins[dwIndex].getFileName().GetLength();
		item.pszText = theApp.m_plugins[dwIndex].getFileName().GetBuffer();
		m_ListCtrlPlugin.InsertItem(&item);
		m_ListCtrlPlugin.SetCheck(dwIndex, !theApp.m_plugins[dwIndex].isDisabled());
	}
	///////////////////
	return TRUE;
}

void CParamDlg::OnCbnSelchangeComboDevlst()
{
	m_CurDev = m_CbDevLst.GetCurSel();
}

void CParamDlg::OnBnClickedCheckOntop()
{
	theApp.m_alwaysOnTop = !theApp.m_alwaysOnTop;
}

void CParamDlg::OnCbnSelchangeComboDevlstAu()
{
	m_CurDevAu = m_CbDevLstAu.GetCurSel();
}

void CParamDlg::OnBnClickedCheckAudio()
{
	theApp.m_withAudio = !theApp.m_withAudio;
}

void CParamDlg::addResolution(Resolution* low, Resolution* high, Resolution* curr)
{
	m_ResolutionLow = low;
	m_ResolutionHigh = high;
	m_CurrResolution = curr;
}

Resolution* CParamDlg::getResolution()
{
	return m_CurrResolution;
}

void CParamDlg::setResolution(Resolution* curr)
{
	m_CurrResolution = curr;
}

void CParamDlg::OnBnClickedCheckLang()
{
	if(((CButton*)GetDlgItem( IDC_CHECK_LANG ))->GetCheck() != BST_CHECKED)
	{
		theApp.m_extLang = 0;
	}
	else
	{
		theApp.m_extLang = 1;
	}
}

void CParamDlg::setDeviceList(CArray<CString>& deviceList)
{
	m_szDevLst.Copy(deviceList);
	m_dwDevNum = deviceList.GetSize();
}

void CParamDlg::setDeviceListAu(CArray<CString>& deviceList)
{
	m_szDevLstAu.Copy(deviceList);
	m_dwDevNumAu = deviceList.GetSize();
}


void CParamDlg::OnCbnSelchangeComboResolution()
{
	switch(m_CbResolution.GetCurSel())
	{
	case 0:
		m_CurrResolution = m_ResolutionLow;
		break;
	case 1:
		m_CurrResolution = m_ResolutionHigh;
		break;
	}
}
