// Rascal_Timer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Rascal_Timer.h"
#include "RascalSdk.h"


Timer::Timer(): 
mLastSecond(0),
mDIBWidthInBytes(0) 
{
	ZeroMemory(mTitleDIBBits, sizeof(BYTE)*5000);
	mFont.CreateFont();
}

//BITMAPFILEHEADER bfh = {0x4D42,0,0,0,62};

HBITMAP Timer::actualCreateTitleDIB(HDC inDC)
{
	struct {
		BITMAPINFOHEADER bmiHeader;
		DWORD rgbEntries[2];
	} bmi =
	{
		{
			sizeof(BITMAPINFOHEADER),
			0,
			0,
			1,	
			1,	
			BI_RGB,
			0,
			0,
			0
		},
		{
			0x00000000,
			0xFFFFFFFF
		}
	};

	mFont.SelectToDC(inDC);
	GetTextExtentPoint32W(inDC, mTitle, lstrlen(mTitle), &mTitleSize);

	// Set proper DIB size here! Important!
	bmi.bmiHeader.biHeight = mTitleSize.cy;
	bmi.bmiHeader.biWidth  = mTitleSize.cx;
	HBITMAP hbm = CreateDIBitmap(inDC, &bmi.bmiHeader, 0, NULL, NULL, 0);
	BOOL	pass = (hbm != NULL);
	// Draw title after selecting DIB into the DC
	if (pass)
	{
		HGDIOBJ hobj = SelectObject(inDC, hbm);
		pass = ExtTextOut(
			inDC, 
			0, 
			0, 
			ETO_OPAQUE | ETO_CLIPPED, 
			NULL, 
			mTitle, 
			lstrlen(mTitle), 
			NULL
		);
		SelectObject(inDC, hobj);
	}
	// Get the title-drew DIB bits
	if (pass)
	{
		//releaseTitleDIB();

		mDIBWidthInBytes = ((mTitleSize.cx + 31) >> 3) & ~3;

		//mTitleDIBBits    = new BYTE[mDIBWidthInBytes * mTitleSize.cy];
		memset(mTitleDIBBits, 0, 100*50);//mDIBWidthInBytes * mTitleSize.cy);
		LONG lLines = GetDIBits(
			inDC, 
			hbm, 
			0, 
			mTitleSize.cy, 
			(PVOID)mTitleDIBBits, 
			(BITMAPINFO *)&bmi, 
			DIB_RGB_COLORS
		);
		pass = (lLines != 0);
	}

	if (!pass && hbm)
	{
		DeleteObject(hbm);
		hbm = NULL;
	}

	return hbm;
}

void Timer::releaseTitleDIB(void)
{/*
	if (mTitleDIBBits)
	{
		delete[] mTitleDIBBits;
		mTitleDIBBits = 0;
	}*/
}

BOOL Timer::createTitleDIBBits(void)
{
	HDC hdc = CreateCompatibleDC(NULL);
	if (hdc)
	{
		HBITMAP hbm = actualCreateTitleDIB(hdc);
		DeleteDC(hdc);
		if (hbm)
		{
			DeleteObject(hbm);
			return TRUE;
		}
	}
	return FALSE;
}

BYTE* Timer::createTimerDIB() {
	if(GetTickCount() - mLastSecond < 1000) {
		return mTitleDIBBits;
	}

	SYSTEMTIME systemTime;
	SYSTEMTIME localTime;
	GetSystemTime(&systemTime);
	SystemTimeToTzSpecificLocalTime(NULL, &systemTime, &localTime);
	ZeroMemory(mTitle, sizeof(wchar_t) * MAX_LENGTH);
	StringCbPrintf(mTitle, 100, L"%4d-%02d-%02d (%02d:%02d:%02d)", 
		localTime.wYear, 
		localTime.wMonth, 
		localTime.wDay, 
		localTime.wHour, 
		localTime.wMinute, 
		localTime.wSecond
	);
	// Update the title DIB bits
	createTitleDIBBits();
	mLastSecond = GetTickCount();

	return mTitleDIBBits;
}

DWORD Timer::getDibWidth() {
	return mTitleSize.cx;
}

DWORD Timer::getDibHeight() {
	return mTitleSize.cy;
}

DWORD Timer::getDibWidthInBytes() {
	return (DWORD)mDIBWidthInBytes;
}

inline void doPixelOverlay(PBYTE pIn, DWORD dwX, DWORD dwY, DWORD dwWidth) {
	//dwY * dwWidth + dwX * 3
	dwY = dwY - 10;
	dwX = dwX + 10;
	pIn[dwY * dwWidth * 3 + dwX * 3 + 0] = 255;
	pIn[dwY * dwWidth * 3 + dwX * 3 + 1] = 255;
	pIn[dwY * dwWidth * 3 + dwX * 3 + 2] = 255;
}

IAddin void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	static Timer timer;

	BYTE* pDib = timer.createTimerDIB();
	DWORD dwDibWidth = timer.getDibWidth();
	DWORD dwDibHeight = timer.getDibHeight();
	DWORD dwDibWidthInBytes = timer.getDibWidthInBytes();
	//wchar_t dmsg[100];ZeroMemory(dmsg, 100*sizeof(wchar_t));swprintf(dmsg, L"%d,%d,%d", dwDibWidth, dwDibHeight, dwDibWidthInBytes);OutputDebugString(dmsg);
	for(DWORD dwY = 0; dwY < dwDibHeight; dwY++) {
		PBYTE pbTitle  = pDib + dwDibWidthInBytes * (dwDibHeight - dwY - 1);

		for(DWORD dwX = 0; dwX < dwDibWidth; dwX++) {
			if ( !((0x80 >> (dwX & 7)) & pbTitle[dwX >> 3]) ) 
				doPixelOverlay(pIn, dwX, height - dwY - 1, width);
		}
	}
}
