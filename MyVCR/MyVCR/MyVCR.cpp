// MyVCR.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyVCR.h"
#include "MyVCRDlg.h"

#include "log4c.h"
#include "BuildCfg.h"

// CMyVCRApp

BEGIN_MESSAGE_MAP(CMyVCRApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMyVCRApp construction

CMyVCRApp::CMyVCRApp()
: unableToCapture(false)
, m_szPath(_T(""))
, m_alwaysOnTop(false)
, m_extLang(0)
, m_withAudio(false)
, m_withRascal(false)
, m_dwPluginCount(0)
, m_szCfgPath(_T(""))
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMyVCRApp object

CMyVCRApp theApp;


// CMyVCRApp initialization

BOOL CMyVCRApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("MyVCR"));
	parseArgs();
	initEnvVar();
	loadSettings();
	registerPlugins();

	CMyVCRSplash cd;
	CMyVCRDlg dlg;
	
	//m_pMainWnd = &cd;
	INT_PTR nRe = cd.DoModal();

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	FreeLibrary(hLang);
	return FALSE;
}

CString& CMyVCRApp::getResStr(int nID)
{
	static CString szMsg;
	szMsg.Empty();
	szMsg.LoadStringW( theApp.hLang , nID );
	return szMsg;
}

CString& CMyVCRApp::getMyPath(void)
{
	return m_szPath;
}

void CMyVCRApp::initEnvVar(void)
{
	//Get My Path
	wchar_t szFullPath[MAX_PATH];
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];

	ZeroMemory(szFullPath, sizeof(wchar_t)*MAX_PATH);
	GetModuleFileName(NULL, szFullPath, MAX_PATH);
	_wsplitpath(szFullPath, drive, dir, NULL, NULL);
	m_szPath.Format(L"%s%s", drive, dir);
	m_szLog.Format(L"%s%s", m_szPath, DEFAULT_LOG_FILE);
	m_szCfgPath.Format(L"%s%s", getMyPath(), DEFAULT_CONFIG_FILE);

	Log4c::setLogFile(m_szLog.GetBuffer());
}


bool CMyVCRApp::parseArgs(void)
{
    LPWSTR *szArglist;
	int nArgs = 0;
	//int i = 0;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if( NULL == szArglist )
	{
		return false;
	}
	else
	{
		for(int i = 0; i < nArgs; i++) 
		{
			if(_wcsicmp(L"/log",szArglist[i]) == 0)
				Log4c::enableLogging();

			if(_wcsicmp(L"/rascal",szArglist[i]) == 0) {
				Cortez::setRascalEnabled(true);
				m_withRascal = true;
			}
		}
	}
	LocalFree(szArglist);
	return true;
}


void CMyVCRApp::loadSettings(void)
{
	wchar_t str[10];
	CString szResDll;

	ZeroMemory(str, sizeof(wchar_t)*10);

	GetPrivateProfileString(L"settings",L"alwaysOnTop",L"",str,MAX_PATH,m_szCfgPath);
	m_alwaysOnTop = (_wtoi(str) == 1 ? true:false);
	
	GetPrivateProfileString(L"settings",L"withAudio",L"",str,MAX_PATH,m_szCfgPath);
	m_withAudio = (_wtoi(str) == 1 ? true:false);

	if(GetPrivateProfileInt(L"rascal", L"alwaysEnable", 0, m_szCfgPath) > 0)
	{
		Cortez::setRascalEnabled(true);
		m_withRascal = true;
	}

	GetPrivateProfileString(L"settings",L"extLang",L"",str,MAX_PATH,m_szCfgPath);
	if(_wtoi(str) > 0)
	{
		GetPrivateProfileString(L"settings",L"lang",L"",str,MAX_PATH,m_szCfgPath);
		m_extLang = _wtoi(str);
	}

	if(m_extLang)
	{
		szResDll.Empty();
		szResDll.Format(LANG_LIB_FORMAT, theApp.getMyPath(), m_extLang);
		theApp.hLang = (HINSTANCE)::LoadLibrary(szResDll);
		//resource dll loaded without any exception
		if( theApp.hLang != NULL )
			return;
	} 

	//otherwise we just use the internal resources
	theApp.hLang = AfxGetApp()->m_hInstance;

}


bool CMyVCRApp::saveSettings(void)
{
	CString szTrue = L"1";
	CString szFalse = L"0";
	CString szDisabledPlugin = L"";
	CString szEnabledPlugin = L"";

	WritePrivateProfileString(L"settings", L"alwaysOnTop", 
		m_alwaysOnTop == true ? szTrue:szFalse, m_szCfgPath);
	
	WritePrivateProfileString(L"settings", L"withAudio", 
		m_withAudio == true ? szTrue:szFalse, m_szCfgPath);

	WritePrivateProfileString(L"settings", L"extLang", 
		m_extLang > 0 ? szTrue:szFalse, m_szCfgPath);

	for(DWORD dwIndex = 0; dwIndex < m_dwPluginCount; dwIndex++) {
		if(m_plugins[dwIndex].isDisabled()) {
			szDisabledPlugin += m_plugins[dwIndex].getFileName();
			szDisabledPlugin += L",";
		} else {
			szEnabledPlugin += m_plugins[dwIndex].getFileName();
			szEnabledPlugin += L",";
		}
	}

	WritePrivateProfileString(L"rascal", L"lib", szEnabledPlugin, m_szCfgPath);
	WritePrivateProfileString(L"rascal", L"disabled", szDisabledPlugin, m_szCfgPath);
	return false;
}


void CMyVCRApp::registerPlugins(void)
{
    wchar_t szFind[MAX_PATH];
	wchar_t szFile[MAX_PATH];
	wchar_t szDisabled[MAX_PATH];

	CString szLibs;
    WIN32_FIND_DATA FindFileData;

	ZeroMemory(szFind,MAX_PATH*sizeof(wchar_t));
    ZeroMemory(szFile,MAX_PATH*sizeof(wchar_t));
	ZeroMemory(szDisabled,MAX_PATH*sizeof(wchar_t));

	wcscpy(szFind, m_szPath);
    wcscat(szFind, PLUGIN_LIB_FORMAT);

	// Check if the plugin is disabled.
	GetPrivateProfileString(L"Rascal", L"disabled", L"", 
		szDisabled, MAX_PATH, m_szCfgPath);

    HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)    return;
    
    while(TRUE)
    {
        if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			wcscpy(szFile,m_szPath);
            wcscat(szFile,L"\\");
			wcscat(szFile,FindFileData.cFileName);
			m_plugins[m_dwPluginCount].setFileName(FindFileData.cFileName);

			if(wcsstr(szDisabled, FindFileData.cFileName) == NULL) {
				szLibs += FindFileData.cFileName;
				szLibs += L",";
				m_plugins[m_dwPluginCount].setDisabled(false);
			} else {
				m_plugins[m_dwPluginCount].setDisabled(true);
			}
			m_dwPluginCount++;
       }
        if(!FindNextFile(hFind,&FindFileData))    
			break;
    }
    FindClose(hFind);

	WritePrivateProfileString(L"rascal", L"lib", 
		szLibs, m_szCfgPath);
}
