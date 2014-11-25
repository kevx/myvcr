/* 
*  Rascal Project
 * Addin : Dog
 * Created BY KEVX@05/13/2011
 * 
 */

#include "stdafx.h"
#include "RascalSdk.h"
#include "HistDetect.h"
#include "ContourDetect.h"

#include "Alarm.h"

#define DEPTH 8						//Must be 8


static DWORD _lastDetectionTime = 0;	//last enter time
static BOOL  _isStarted = FALSE;	//doProcess() haven't been called since the lib was loaded
static BOOL  _alarmDisabled = TRUE;	//alarm disabled before the webcam becomes stable
static DWORD _startTime = 0;		//the first time when doProcess() is called

void _oString(long a, double b, wchar_t* format) {
	wchar_t m[255];
	ZeroMemory(m, sizeof(wchar_t)*255);
	swprintf(m, format, a, b);
	OutputDebugString(m);
}

void saveCurrentFrame(IplImage* img, DWORD enterTime)
{
	char fImgPath[MAX_PATH];
	ZeroMemory(fImgPath, MAX_PATH*sizeof(char));

	sprintf(fImgPath, "R:\\%d.png", enterTime);
	cvSaveImage(fImgPath, img);
}


extern "C" IAddin void doTest()
{

}

bool overAlarmRate(double rate)
{
	if(_alarmDisabled)
		return FALSE;

	double alarmRate = (double)g_dogConfig.alarmRate / 100.0;
	double currentRate = rate;

	if(currentRate < alarmRate)
		return TRUE;
	else
		return FALSE;
}


IAddin void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	DWORD enterTime = GetTickCount();

	if(_isStarted == FALSE) {
		_startTime = enterTime;
		_isStarted = TRUE;
		return;
	}

	/*Generally a camera need a few seconds before it becomes stable*/
	if(enterTime - _startTime >= g_dogConfig.initTime)
		_alarmDisabled = FALSE;

	if(enterTime - _lastDetectionTime > g_dogConfig.interval)
	{
		int bytePerPixel = bitCount / DEPTH;
		IplImage* img = cvCreateImageHeader(cvSize(width, height), DEPTH, bytePerPixel);
		img->origin = 1;
		cvSetData(img, pIn, width * bytePerPixel);

		double matchRate = 0.0;
#ifdef DETECT_CONTOUR
		matchRate = compareContours(img);
		_oString(0, matchRate, L"CONT-%d-%.4f");
#else 
		//matchRate = compareHist(img);
		//_oString(0, matchRate, L"HIST-%d-%.4f");
#endif
		if(overAlarmRate(matchRate))
		{
			sendAlarm(g_dogConfig.alarmType);
			saveCurrentFrame(img, enterTime);
		}
		cvReleaseImageHeader(&img);
		_lastDetectionTime = enterTime;
	}
}
