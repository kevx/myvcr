//Log4c
//3/22/2010
//Kevx
#pragma once

class Log4c {
private:
	static  bool	m_isLoggingEnabled;
	static  wchar_t m_logFile[MAX_PATH];
	
	Log4c()
	{
	}

public:
	static bool isLoggingEnabled();
	static void enableLogging();
	static bool setLogFile(wchar_t* pFile);
	static void logMsg(wchar_t* lpFmt, ... );
};
