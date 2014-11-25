// Rascal_Xxx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "RascalSdk.h"
#include "Xr4.h"

static rc4_key _key;
static bool	   _isKeyPrepared = false;
static unsigned char _keyData[16] = 
{
	0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,
	0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,
};

void doProcess(PBYTE pIn, DWORD dwSize) {
	if(_isKeyPrepared == false) {
		prepare_key(_keyData, 16, &_key);
		_isKeyPrepared = true;
	}

	rc4((unsigned char*)pIn, dwSize, &_key);
}

