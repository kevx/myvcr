// Setup.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Setup.h"

#define X264_DESC L"x264 H.264 Video Codec"
#define X264_FILE L"x264vfw.dll"

typedef HRESULT (*IDllRegisterServer)(void);

static wchar_t _szPath[MAX_PATH];
static wchar_t _szRascalDll[MAX_PATH];
static wchar_t _szX264Dll[MAX_PATH];
static wchar_t _szX264Dest[MAX_PATH];
static wchar_t _szX264Cfg[MAX_PATH];

void getPath() {
	wchar_t szFullPath[MAX_PATH];
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];

	ZeroMemory(_szPath, sizeof(wchar_t)*MAX_PATH);
	ZeroMemory(_szRascalDll, sizeof(wchar_t)*MAX_PATH);
	ZeroMemory(_szX264Dll, sizeof(wchar_t)*MAX_PATH);
	ZeroMemory(_szX264Dest, sizeof(wchar_t)*MAX_PATH);
	ZeroMemory(_szX264Cfg, sizeof(wchar_t)*MAX_PATH);

	ZeroMemory(szFullPath, sizeof(wchar_t)*MAX_PATH);
	GetModuleFileName(NULL, szFullPath, MAX_PATH);
	_wsplitpath(szFullPath, drive, dir, NULL, NULL);

	StringCchPrintf(_szPath, sizeof(wchar_t)*MAX_PATH, 
		L"%s%s",drive, dir);

	StringCchPrintf(_szRascalDll, sizeof(wchar_t)*MAX_PATH, 
		L"%sRascal.dll", _szPath);

	StringCchPrintf(_szX264Cfg, sizeof(wchar_t)*MAX_PATH, 
		L"%sx264cfg.dat", _szPath);

	StringCchPrintf(_szX264Dll, sizeof(wchar_t)*MAX_PATH, 
		L"%sx264vfw.dll", _szPath);

	StringCchPrintf(_szX264Dest, sizeof(wchar_t)*MAX_PATH, 
		L"%sx264vfw.dll", L"C:\\Windows\\System32\\");
}

bool registerRascal()
{
	HINSTANCE hDll = LoadLibrary(_szRascalDll);
	IDllRegisterServer iDll = (IDllRegisterServer)
		GetProcAddress(hDll, "DllRegisterServer");

	if(iDll != NULL)
	{
		iDll();
	}

	return true;
}

bool registerX264Codec()
{
	HKEY hKey;
	size_t nLen = 0;

	CopyFile(_szX264Dll, _szX264Dest, false);

	StringCchLength(X264_FILE, MAX_PATH, &nLen);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		L"Software\\Microsoft\\Windows NT\\CurrentVersion\\drivers32", 
		0, KEY_ALL_ACCESS, &hKey
	);

	RegSetValueEx(hKey, L"vidc.x264", 0, REG_SZ, (const BYTE*)X264_FILE, (nLen + 1)*sizeof(wchar_t));
	RegCloseKey(hKey);

	StringCchLength(X264_DESC, MAX_PATH, &nLen);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		L"Software\\Microsoft\\Windows NT\\CurrentVersion\\drivers.desc", 
		0, KEY_ALL_ACCESS, &hKey
	);

	RegSetValueEx(hKey, X264_FILE, 0, REG_SZ, 
		(const BYTE*)X264_DESC, 
		(nLen + 1)*sizeof(wchar_t)
	);
	RegCloseKey(hKey);

	return true;
}

bool importX264Cfg()
{
	wchar_t cmdToRun[MAX_PATH];
	ZeroMemory(cmdToRun, sizeof(wchar_t)*MAX_PATH);
	StringCchPrintf(cmdToRun, MAX_PATH*sizeof(wchar_t), L"REG IMPORT %s", _szX264Cfg);
	_wsystem(cmdToRun);
	return true;
}

int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
//int main()
{
	getPath();
	registerX264Codec();
	registerRascal();
	importX264Cfg();

	MessageBox(NULL, L"Successfully~!", L"MyVCR Setup", MB_ICONINFORMATION);
	return 0;
}

