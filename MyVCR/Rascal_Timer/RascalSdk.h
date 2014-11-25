/*
	RASCAL SDK HEADER FILE
	Created BY KEVX@11/7/2010

	#modified@05/12/2011: Interface upgraded

	RascalSdk.h
*/
#include <WinDef.h>

/*All plugins should implement the following interface*/

/*
	Parameters:
		pIn: pointer to the buffer of a frame.
		size: size of the buffer(i.e width*height*bitCount/8)
		width: width of the buffer.
		height : height of the buffer.
		bitCount : must be 24
	Diz:
		This function may never change the size of the buffer,
		otherwise something unexpected will happen
*/
#define IAddin __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif

IAddin void doProcess(PBYTE pIn, DWORD size, DWORD width, DWORD height, DWORD bitCount);

#ifdef __cplusplus
}
#endif