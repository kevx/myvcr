//
// CAutoFont.h
//

#ifndef __H_CAutoFont__
#define __H_CAutoFont__

class CAutoFont
{
private:
	HDC        mDC;
	LOGFONT    mLogFont;
	HFONT      mNewFont;
	HFONT      mOldFont;

private:
	void DefaultFont(void);

public:
	CAutoFont();
	CAutoFont(LOGFONT inFont);
	~CAutoFont();

	void CreateFont();
	void CreateFont(LOGFONT inFont);
	void CreateFont(const WCHAR * inFaceName);
	void CreateStockObject(int inIndex);

	void SelectToDC(HDC inTargetDC);
	void RestoreToDC(void);
};

#endif // __H_CAutoFont__