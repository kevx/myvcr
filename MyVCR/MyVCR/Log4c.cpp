
#include "stdafx.h"
#include "log4c.h"

bool	Log4c::m_isLoggingEnabled = false;
wchar_t Log4c::m_logFile[] = {NULL};

void Log4c::logMsg(wchar_t * lpFmt, ... )
{
	if( m_logFile == NULL )
		return;
		
	if( m_isLoggingEnabled == false )
		return;

	wchar_t buff[1024];
	va_list    arglist;
	va_start( arglist, lpFmt );
	_vsnwprintf( buff, sizeof buff, lpFmt, arglist );
	va_end( arglist );

	FILE *fp = _wfopen(m_logFile, L"a+");
	if(fp)
	{
		wchar_t date[20], time[20];
		fwprintf(fp, 
			L"%s %s - %s\n", 
			_wstrdate(date), 
			_wstrtime(time), 
			buff
		);
		fclose(fp);
	}
	OutputDebugStringW(buff);
}

bool Log4c::setLogFile(wchar_t *pFile)
{
	ZeroMemory(m_logFile, MAX_PATH * sizeof(wchar_t));
	wcscpy_s(m_logFile, MAX_PATH, pFile);
	return true;
}

void Log4c::enableLogging()
{
	m_isLoggingEnabled = true;
}

bool Log4c::isLoggingEnabled()
{
	return m_isLoggingEnabled;
}