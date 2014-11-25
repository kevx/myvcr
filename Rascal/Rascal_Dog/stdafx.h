// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma comment(lib, "opencv_core220.lib")
#pragma comment(lib, "opencv_highgui220.lib")
#pragma comment(lib, "opencv_imgproc220")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <wchar.h>
#define DETECT_CONTOUR

typedef struct tagDogConfig {
	int initTime;
	int interval;
	int threshold;
	int alarmRate;
	int alarmType;
}DogConfig;

extern DogConfig g_dogConfig;		//Global dog config, loaded from file 'rascal_dog.cfg'
