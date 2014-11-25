//
// CAutoFont.cpp
//

#include "stdafx.h"
#include "CAutoFont.h"

////////////////////////////////////////////////////////////////////////////////
CAutoFont::CAutoFont()
{
	mDC      = NULL;
	mNewFont = NULL;
	mOldFont = NULL;
	DefaultFont();
}

CAutoFont::CAutoFont(LOGFONT inFont)
{
	mDC      = NULL;
	mNewFont = NULL;
	mOldFont = NULL;
	CreateFont(inFont);
}

CAutoFont::~CAutoFont()
{
	RestoreToDC();
}

void CAutoFont::DefaultFont(void)
{
	mLogFont.lfHeight      = -12;
	mLogFont.lfWidth       = 0;
	// Specifies the angle in tenths of degrees
	// If rotation font required, lfEscapement and lfOrientation must be the same
	mLogFont.lfEscapement  = 0;
	mLogFont.lfOrientation = 0;
	mLogFont.lfWeight      = FW_SEMIBOLD;//FW_NORMAL;
	mLogFont.lfItalic      = 0;
	mLogFont.lfUnderline   = 0;
	mLogFont.lfStrikeOut   = 0;
	mLogFont.lfCharSet        = ANSI_CHARSET;
	mLogFont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	mLogFont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	mLogFont.lfQuality        = PROOF_QUALITY;
	mLogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	wcscpy(mLogFont.lfFaceName, L"Courier New");
}

void CAutoFont::CreateFont(LOGFONT inFont)
{
	RestoreToDC();

	mLogFont = inFont;
	mNewFont = CreateFontIndirect(&mLogFont);
}

void CAutoFont::CreateFont()
{
	CreateFont(mLogFont);
}

void CAutoFont::CreateFont(const WCHAR * inFaceName)
{
	RestoreToDC();

	wcscpy(mLogFont.lfFaceName, inFaceName);
	mNewFont = CreateFontIndirect(&mLogFont);
}

void CAutoFont::CreateStockObject(int inIndex)
{
	RestoreToDC();

	// It is not necessary to delete stock objects by calling DeleteObject,
	// but it is not harmful.
	if (inIndex >= OEM_FIXED_FONT && inIndex <= DEFAULT_GUI_FONT)
	{
		mNewFont = (HFONT) GetStockObject(inIndex);
	}
}

void CAutoFont::SelectToDC(HDC inTargetDC)
{
	if (inTargetDC && mNewFont)
	{
		mDC      = inTargetDC;
		mOldFont = (HFONT) SelectObject(mDC, mNewFont);
	}	
}

void CAutoFont::RestoreToDC(void)
{
	if (mOldFont && mDC)
	{
		SelectObject(mDC, mOldFont);
		mOldFont = NULL;
	}
	if (mNewFont)
	{
		DeleteObject(mNewFont);
		mNewFont = NULL;
	}
}