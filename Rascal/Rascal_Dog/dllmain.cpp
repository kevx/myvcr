// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

const wchar_t* SECTION = L"settings";

const wchar_t* cfgFileName = L"rascal_dog.cfg";
wchar_t cfgFile[MAX_PATH];

DogConfig g_dogConfig;

void loadConfig(HMODULE hModule)
{
	wchar_t szFullPath[MAX_PATH];
	wchar_t drive[MAX_PATH];
	wchar_t dir[MAX_PATH];

	ZeroMemory(szFullPath, sizeof(wchar_t) * MAX_PATH);
	GetModuleFileName(hModule, szFullPath, MAX_PATH);
	_wsplitpath(szFullPath, drive, dir, NULL, NULL);
	
	wcscat(cfgFile, drive);
	wcscat(cfgFile, dir);
	wcscat(cfgFile, cfgFileName);
	
	g_dogConfig.initTime  = GetPrivateProfileInt(SECTION, L"initTime", 5000, cfgFile);
	g_dogConfig.interval  = GetPrivateProfileInt(SECTION, L"interval", 200, cfgFile);
	g_dogConfig.threshold = GetPrivateProfileInt(SECTION, L"threshold", 50, cfgFile);
	g_dogConfig.alarmRate = GetPrivateProfileInt(SECTION, L"alarmRate", 70, cfgFile);
	g_dogConfig.alarmType = GetPrivateProfileInt(SECTION, L"alarmType", 0, cfgFile);
	
	wchar_t msg[100];ZeroMemory(msg, sizeof(wchar_t)*100);
	swprintf(msg, L"dog_loaded_with:%d,%d,%d,%d", 
		g_dogConfig.initTime, 
		g_dogConfig.interval,
		g_dogConfig.threshold,
		g_dogConfig.alarmRate);
	OutputDebugString(msg);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		loadConfig(hModule);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

