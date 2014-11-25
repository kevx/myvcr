// Tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


typedef void (*IAddin)(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount);

static HMODULE hAddin = NULL;
static IAddin pfn = NULL;
static BITMAPFILEHEADER bmpHeader;
static BITMAPINFOHEADER bmpInfo;

static int pxSize = 0;
static int buffSize = 0;

const wchar_t* rascalAddin = L"E:\\cpp\\Rascal\\release\\Rascal_Monochrome.dll";
const char* interfaceName = "doProcess";

wchar_t* $(int len) {
	wchar_t* s = (wchar_t*)malloc(len*sizeof(wchar_t));
	ZeroMemory(s, len*sizeof(wchar_t));
	return s;
}

void $$(wchar_t* s) {
	free(s);
}

BYTE* loadBmp()
{
	FILE* bmp = fopen("R:\\sample.bmp", "rb");
	if(bmp == NULL) {
		return NULL;
	}

    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, bmp);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, bmp);
	pxSize = bmpInfo.biBitCount / 8;
	buffSize = bmpInfo.biHeight * bmpInfo.biWidth * pxSize;

	BYTE* imgBuff = (BYTE*)malloc(buffSize);
	fseek(bmp, bmpHeader.bfOffBits, SEEK_SET);
	fread(imgBuff, buffSize, 1, bmp);
	fclose(bmp);

	return imgBuff;
}

void saveBmp(BYTE* buff, char* file)
{
	if(file == NULL) {
		goto __free;
	}

	FILE *bmpOut = fopen(file, "wb+");
	fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, bmpOut);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, bmpOut);
	//fseek(bmpOut, bmpHeader.bfOffBits, SEEK_SET);
	fwrite(buff, buffSize, 1, bmpOut);
	fclose(bmpOut);

__free:
	free(buff);
}

void loadTest(BYTE* buff)
{	
	DWORD t1 = GetTickCount();
	BYTE* data = (BYTE*)malloc(buffSize);
	for(int i = 0; i < 10; i++) 
	{
		memcpy(data, buff, buffSize);
		doProcess(data, buffSize, bmpInfo.biWidth, bmpInfo.biHeight, pxSize);	

	}
	DWORD t2 = GetTickCount();
	printf("LOOP:10#CallTime=%d\n", t2 - t1);

	free(data);
}

void featureTest()
{
	//wchar_t* s = $(100);
	//swprintf(s, L"dataOffset=%d,size=%d\n", dataOffset, size);
	//OutputDebugString(s);
	//$$(s);
	//pfn(data, size, width, height, 24);
	//saveBmp();
}

int _tmain(int argc, _TCHAR* argv[])
{
	BYTE* imgBuff = loadBmp();
	
	//hAddin = LoadLibraryW(rascalAddin);
	//pfn = (IAddin)GetProcAddress(hAddin, interfaceName);
	loadTest(imgBuff);
	//featureTest();
	doProcess(imgBuff, buffSize, bmpInfo.biWidth, bmpInfo.biHeight, pxSize);
	saveBmp(imgBuff, "R:\\sample2.bmp");

	//FreeLibrary(hAddin);
	system("pause");
	return 0;
}

