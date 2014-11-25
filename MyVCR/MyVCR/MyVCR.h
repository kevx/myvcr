// MyVCR.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Plugin.h"

#define DEFAULT_CONFIG_FILE L"settings.cfg"
// CMyVCRApp:
// See MyVCR.cpp for the implementation of this class
//

class CMyVCRApp : public CWinApp
{
public:
	CMyVCRApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

public:
	CString & getResStr(int nID);
public:
	HINSTANCE hLang;
public:
	bool unableToCapture;
	CString m_szPath;
	CString m_szLog;
	CPlugin m_plugins[10];
	DWORD	m_dwPluginCount;

public:
	CString& getMyPath(void);
	void	initEnvVar(void);
	bool	parseArgs(void);
	void	loadSettings(void);
	bool	m_alwaysOnTop;
	int		m_extLang;
	bool	m_withAudio;
	bool	m_withRascal;
	bool	saveSettings(void);
	CString m_szCfgPath;
	void	registerPlugins(void);
};

extern CMyVCRApp theApp;