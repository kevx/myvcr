#include "StdAfx.h"
#include "Plugin.h"


CPlugin::CPlugin(wchar_t* strPlugin)
{
	_szFileName = strPlugin;
}

CPlugin::CPlugin() :
_isDisabled(false),
_szFileName(L"")
{
}

CPlugin::~CPlugin(void)
{
}

CString& CPlugin::getFileName() {
	return _szFileName;
}

bool CPlugin::isDisabled() {
	return _isDisabled;
}

void CPlugin::setFileName(wchar_t* strPlugin)
{
	_szFileName = strPlugin;
}

void CPlugin::setDisabled(bool b) {
	_isDisabled = b;
}
