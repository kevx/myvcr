#pragma warning(disable:4996)
#pragma warning(disable:4995)

#include "StdAfx.h"
#include "AddinManager.h"
#include <wchar.h>

#include "Log4c.h"

bool AddinManager::isLoaded = false;
auto_ptr<AddinManager> AddinManager::m_pInstance;

AddinManager::AddinManager(void):m_dwAddins(0)
{
	GetModuleFileName(NULL,m_szPath,MAX_PATH);
	for(DWORD dwIndex = MAX_PATH - 1; dwIndex > 0; dwIndex--)
	{
		if(m_szPath[dwIndex] != L'\\')
			m_szPath[dwIndex] = L'\0';
		else
			break;
	}
	wcscpy(m_szCfg, m_szPath);
	wcscat(m_szCfg, L"settings.cfg");
}


AddinManager::~AddinManager(void)
{
}

AddinManager* AddinManager::getInstance()
{
	if(m_pInstance.get() == NULL)
		m_pInstance.reset(new AddinManager);
	return m_pInstance.get();
}

void AddinManager::loadAddins()
{
	if(isLoaded)
		return;
	else
		isLoaded = true;

	wchar_t szLibs[MAX_PATH];
	wchar_t szLibPath[MAX_ADDINS][MAX_PATH];
	wchar_t szTmp[MAX_PATH];
	DWORD	dwActualCount = 0;
	DWORD	dwPtr = 0;

	ZeroMemory(szLibs, MAX_PATH * sizeof(wchar_t));
	ZeroMemory(szTmp, MAX_PATH * sizeof(wchar_t));
	GetPrivateProfileString(L"rascal",L"lib",L"",szLibs, MAX_PATH, m_szCfg);
	for(DWORD dwIndex = 0; dwIndex < MAX_PATH; dwIndex++)
	{
		if(szLibs[dwIndex] == L',')
		{		
			ZeroMemory(szLibPath[dwActualCount], MAX_PATH * sizeof(wchar_t));
			wcscpy(szLibPath[dwActualCount], m_szPath);
			wcscat(szLibPath[dwActualCount], szTmp);
			ZeroMemory(szTmp, MAX_PATH * sizeof(wchar_t));
			dwActualCount++;
			dwPtr = 0;
		}
		else
		{
			szTmp[dwPtr++] = szLibs[dwIndex];
		}
		if(dwActualCount >= MAX_ADDINS)
			break;
	}
	
	m_dwAddins = dwActualCount;
	OutputDebugStringW(L"AddinManager::loadAddins()...Begin");
	for(DWORD dwIndex = 0; dwIndex < m_dwAddins; dwIndex++)
	{
		OutputDebugStringW(szLibPath[dwIndex]);
		m_addinLibs[dwIndex] = ::LoadLibrary(szLibPath[dwIndex]);
		if(m_addinLibs[dwIndex] != NULL)
		{
			m_addinEntries[dwIndex] = 
				(IAddin)GetProcAddress(m_addinLibs[dwIndex], "doProcess");
			if(m_addinEntries[dwIndex] == NULL)
			{
				m_addinEntries[dwIndex] = (IAddin)AddinManager::doNull;
				OutputDebugStringW(L"Failed GetProcAddress()");
			}
		}
		else
		{
			m_addinEntries[dwIndex] = (IAddin)AddinManager::doNull;
			OutputDebugStringW(L"Failed LoadLibrary()");
		}
	}

	OutputDebugStringW(L"AddinManager::loadAddins()...DONE//");
}

void AddinManager::unloadAddins()
{
	if(!isLoaded)
		return;

	isLoaded = false;
	OutputDebugStringW(L"AddinManager::unloadAddins()...DONE//");
	for(DWORD dwIndex = 0; dwIndex < m_dwAddins; dwIndex++)
		::FreeLibrary(m_addinLibs[dwIndex]);
}

void AddinManager::process(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	for(DWORD dwIndex = 0; dwIndex < m_dwAddins; dwIndex++)
		m_addinEntries[dwIndex](pIn, size, width, height, bitCount);
}
