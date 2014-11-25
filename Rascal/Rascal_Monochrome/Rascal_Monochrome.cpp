// Rascal_Monochrome.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "RascalSdk.h"

/*
	this is an easy example which demonstrates 
	how does plugin work. It just transforms the
	original bitmap to monochrome style.
	
	Parameters:
		pIn: 
			bitmap data in uncompressed format.

		dwSize: 
			Height*Width*Bits-per-pixel
			The default image resolution of MyVCR
			is 320*240 and the value of bits-per-pixel is
			24(i.e. 3 Bytes).
*/
void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	DWORD dwRGBSum = 0;
	for(DWORD dwIndex = 0; dwIndex < size; dwIndex+=3)
	{
		dwRGBSum = 
			0.1 * pIn[dwIndex+0] +   //Blue
			0.6 * pIn[dwIndex+1] +   //Green
			0.3 * pIn[dwIndex+2];    //Red

		pIn[dwIndex+0] = dwRGBSum;
		pIn[dwIndex+1] = dwRGBSum;
		pIn[dwIndex+2] = dwRGBSum;
	}
}
