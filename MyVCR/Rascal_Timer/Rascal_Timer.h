
#ifdef RASCAL_TIMER_EXPORTS
#define RASCAL_TIMER_API __declspec(dllexport)
#else
#define RASCAL_TIMER_API __declspec(dllimport)
#endif

#define MAX_LENGTH 100

#include "CAutoFont.h"

class Timer {
private:
	HBITMAP actualCreateTitleDIB(HDC inDC);
	BOOL createTitleDIBBits(void);
	void releaseTitleDIB(void);
	

	wchar_t				mTitle[MAX_LENGTH];
	SIZE				mTitleSize;
	BYTE				mTitleDIBBits[100*50];
	long                mDIBWidthInBytes;
	long				mLastSecond;
	CAutoFont			mFont;
public :
	BYTE* createTimerDIB();
	DWORD getDibWidth();
	DWORD getDibHeight();
	DWORD getDibWidthInBytes();
	Timer();
};