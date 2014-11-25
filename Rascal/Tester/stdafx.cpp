// stdafx.cpp : source file that includes just the standard includes
// Tester.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <xmmintrin.h>
#include <emmintrin.h>

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount)
{
	UINT16 dwRGBSum0 = 0;
	UINT16 dwRGBSum1 = 0;
	UINT16 dwRGBSum2 = 0;
	UINT16 dwRGBSum3 = 0;

	for(DWORD idx = 0; idx < size; idx+=12)
	{
		dwRGBSum0 = 
			1 * pIn[idx+0] +   //Blue
			6 * pIn[idx+1] +   //Green
			3 * pIn[idx+2];    //Red

		dwRGBSum1 = 
			1 * pIn[idx+3] +   //Blue
			6 * pIn[idx+4] +   //Green
			3 * pIn[idx+5];    //Red

		dwRGBSum2 = 
			1 * pIn[idx+6] +   //Blue
			6 * pIn[idx+7] +   //Green
			3 * pIn[idx+8];    //Red

		dwRGBSum3 = 
			1 * pIn[idx+9] +   //Blue
			6 * pIn[idx+10] +   //Green
			3 * pIn[idx+11];    //Red


		__m128 old = _mm_set_ps(dwRGBSum0, dwRGBSum1, dwRGBSum2, dwRGBSum3);
		__m128 ret = _mm_mul_ps(old, vec);

		pIn[idx+0] = pIn[idx+1] = pIn[idx+2] = (BYTE)ret.m128_f32[3];
		pIn[idx+3] = pIn[idx+4] = pIn[idx+5] = (BYTE)ret.m128_f32[2];
		pIn[idx+6] = pIn[idx+7] = pIn[idx+8] = (BYTE)ret.m128_f32[1];
		pIn[idx+9] = pIn[idx+10] = pIn[idx+11] = (BYTE)ret.m128_f32[0];
	}
}