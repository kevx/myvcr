#pragma once

#define MAX_ADDINS 5
#include <memory>
#include <iostream>
using namespace std;

typedef void (*IAddin)(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount);

class AddinManager
{
public:
	
	~AddinManager(void);

	void loadAddins();
	void unloadAddins();
	void process(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount);
	static AddinManager* getInstance();

private:
	wchar_t m_szPath[MAX_PATH];
	wchar_t m_szCfg[MAX_PATH];
	
	friend class auto_ptr<AddinManager>;
	static auto_ptr<AddinManager> m_pInstance;
	static bool isLoaded;

	DWORD	m_dwAddins;
	HMODULE	m_addinLibs[MAX_ADDINS];
	IAddin	m_addinEntries[MAX_ADDINS];

	inline static void doNull(PBYTE pbIn, DWORD dwSize)
	{
		//OutputDebugStringW(L"doNull invoked..");
	}

	AddinManager(void);
};

