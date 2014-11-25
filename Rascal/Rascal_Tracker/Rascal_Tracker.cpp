// Rascal_Tracker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RascalSdk.h"

#define DRAW_LATENCY 500

static DWORD dwLastTime = 0;
static PBYTE pBuff = NULL;

void findRedestPixel(DWORD* dwX, DWORD* dwY, DWORD dwWidth, DWORD dwHeight, DWORD dwBitCount)
{
	PBYTE pLine = NULL;
    BYTE  red = 0;
	int   greenAndBlue = 255 + 255;
    DWORD dwCurrX = 0;
    DWORD dwCurrY = 0;
    DWORD dwBytePerPixel = dwBitCount / 8; //must be 3

    for(DWORD dwLine = 0; dwLine < dwHeight; dwLine++)
    {
		pLine = pBuff + dwLine * dwWidth * dwBytePerPixel;
        for(DWORD dwOffset = 0; dwOffset < dwWidth * dwBytePerPixel; dwOffset += dwBytePerPixel)
        {
            if(pLine[dwOffset + 2] > red)
            {
				if(pLine[dwOffset + 0] + pLine[dwOffset + 1] < greenAndBlue)
				{
					greenAndBlue = pLine[dwOffset + 0] + pLine[dwOffset + 1];
					red = pLine[dwOffset + 2];
					dwCurrX = dwOffset;
					dwCurrY = dwLine;
				} 
            }
        }
    }

	*dwX = dwCurrX / 3;
	*dwY = dwHeight - dwCurrY - 1;
}

inline DWORD coordToIndex(DWORD dwX, DWORD dwY, DWORD dwWidth)
{
	return dwY * dwWidth * 3 + dwX * 3;
}

inline void setPixel(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight)
{
	if(dwX > dwWidth || dwX <= 0)
		return;
	if(dwY > dwHeight || dwY <= 0)
		return;

	pBuff[coordToIndex(dwX, dwY, dwWidth) + 0] = 0;
	pBuff[coordToIndex(dwX, dwY, dwWidth) + 1] = 0;
	pBuff[coordToIndex(dwX, dwY, dwWidth) + 2] = 0;
}

void drawRect(DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight) 
{
	dwY = dwHeight - dwY - 1;

	for(int x = 0; x < 40; x++)
	{
		setPixel(dwX - 20 + x, dwY - 10, dwWidth, dwHeight);
		setPixel(dwX - 20 + x, dwY + 10, dwWidth, dwHeight);
	}

	for(int y = 0; y <= 18; y++)
	{
		setPixel(dwX - 20, dwY - 9 + y, dwWidth, dwHeight);
		setPixel(dwX + 19, dwY - 9 + y, dwWidth, dwHeight);
	}

	for(int y = 0; y < 20; y++)
	{
		setPixel(dwX, dwY + 10 + y, dwWidth, dwHeight);
		setPixel(dwX, dwY - 10 - y, dwWidth, dwHeight);
	}
}

void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	static DWORD dwX = 0;
	static DWORD dwY = 0;
	pBuff = pIn;

	if(GetTickCount() - dwLastTime > DRAW_LATENCY) {
		findRedestPixel(&dwX, &dwY, width, height, bitCount);
		dwLastTime = GetTickCount();
	}
	drawRect(dwX, dwY, width, height);
}
