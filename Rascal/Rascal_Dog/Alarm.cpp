#include "StdAfx.h"
#include "Alarm.h"


#ifdef ADDIN_PRO
void sendAlarm(int alarmType)
{
}
#else
void sendAlarm(int alarmType)
{
	OutputDebugString(L"!!!DETECTED!!!");
	sndPlaySound(L"alarm.wav", SND_ASYNC | SND_NOSTOP);
}
#endif