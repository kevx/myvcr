// Created by KEVX@10/24/2010#12:30AM

/* 
   Okay, u may recognise that some of the follwing code
   are copied from MSDN. That's right~

 */


#include "stdafx.h"

#include <Streams.h>
#include "Rascal.h"

#include "Log4c.h"

AddinManager* Rascal::m_pAddinMgr = NULL;

static WCHAR g_wszName[] = L"RascalFilter";

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

const AMOVIESETUP_PIN sudpPins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &sudPinTypes          // Pin information
    }
};

const AMOVIESETUP_FILTER sudRascalFilter =
{
    &CLSID_RASCAL,         // Filter CLSID
    L"KEVX Rascal Image Effects (RASCAL)",       // String name
    MERIT_NORMAL,       // Filter merit
    2,                      // Number of pins
    sudpPins                // Pin information
};

CFactoryTemplate g_Templates[] = 
{
  { 
    g_wszName,
    &CLSID_RASCAL,
	(LPFNNewCOMObject)Rascal::CreateInstance,
    NULL,
    &sudRascalFilter
  }
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);  

STDAPI DllRegisterServer()
{
    return AMovieDllRegisterServer2( TRUE );
}
STDAPI DllUnregisterServer()
{
    return AMovieDllRegisterServer2( FALSE );
}

wchar_t* guid2String(GUID guid, wchar_t* buff, int len) {
	ZeroMemory(buff, sizeof(wchar_t)*len);
	wsprintf(buff, L"%lx", guid.Data1);
	return buff;
}

Rascal::Rascal() : 
	CTransformFilter(NAME("RascalFilter"), 0, CLSID_RASCAL)
{
	m_inMediaType = NULL;
}

Rascal::~Rascal()
{
	OutputDebugStringW(L"Rascal::~Rascal()...DONE//");
}

bool Rascal::subtypeNotSupported(GUID guid)
{
	if( guid != MEDIASUBTYPE_RGB24)
		return true;
	/*!!!ToFix, should check the subtype..
	but now i dont have enough time for this shit
	,just pass...works
	*/
	return false;
}

bool Rascal::bitCountSupported(WORD bitCount)
{
	if(bitCount == 24/* || bitCount == 32*/) {
		//OutputDebugStringW(L"Rascal::bitCountSupported()...true");
		return true;
	}
	else
		return false;
}

HRESULT Rascal::CheckInputType(const CMediaType *mtIn)
{
	if ((mtIn->majortype != MEDIATYPE_Video) ||
        (subtypeNotSupported(mtIn->subtype)) ||
        (mtIn->formattype != FORMAT_VideoInfo) || 
        (mtIn->cbFormat < sizeof(VIDEOINFOHEADER)))
    {
        return VFW_E_TYPE_NOT_ACCEPTED;
    }

    VIDEOINFOHEADER *pVih = 
        reinterpret_cast<VIDEOINFOHEADER*>(mtIn->pbFormat);
    if (!bitCountSupported(pVih->bmiHeader.biBitCount) ||
        (pVih->bmiHeader.biCompression != BI_RGB))
    {
		return VFW_E_TYPE_NOT_ACCEPTED;
    }

    // Check the palette table.
    if (pVih->bmiHeader.biClrUsed > PALETTE_ENTRIES(pVih))
    {
        return VFW_E_TYPE_NOT_ACCEPTED;
    }
    DWORD cbPalette = pVih->bmiHeader.biClrUsed * sizeof(RGBQUAD);
    if (mtIn->cbFormat < sizeof(VIDEOINFOHEADER) + cbPalette)
    {
        return VFW_E_TYPE_NOT_ACCEPTED;
    }
    // Everything is good.

	this->m_width = pVih->bmiHeader.biWidth;
	this->m_height = pVih->bmiHeader.biHeight;
	this->m_bitCount = pVih->bmiHeader.biBitCount;
	this->m_sampleSize = m_width*m_height*m_bitCount / 8;
    return S_OK;
}

HRESULT Rascal::GetMediaType(int iPosition, CMediaType *pMediaType)
{
    ASSERT(m_pInput->IsConnected());
    if (iPosition < 0)
    {
        return E_INVALIDARG;
    }
    if (iPosition == 0)
    {
		VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)(m_pInput->CurrentMediaType().pbFormat);
		*pMediaType = m_pInput->CurrentMediaType();
        return S_OK;
    }
    // else
	OutputDebugStringW(L"Rascal::GetMediaType()...FAIL");
    return VFW_S_NO_MORE_ITEMS;
}

HRESULT Rascal::CheckTransform(
	const CMediaType *mtIn, const CMediaType *mtOut)
{
    // Check the major type.
    if (mtOut->majortype != MEDIATYPE_Video ||
		mtOut->subtype != mtIn->subtype)
    {
		wchar_t buff[100];
		OutputDebugStringW(L"Rascal::CheckTransform()...mtOut->subtype");
		OutputDebugStringW(guid2String(mtOut->subtype, buff, 100));

        return VFW_E_TYPE_NOT_ACCEPTED;
    }
	OutputDebugStringW(L"Rascal::CheckTransform()...S_OK");
	return S_OK;

}

HRESULT Rascal::DecideBufferSize(
    IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp)
{
    AM_MEDIA_TYPE mt;
    HRESULT hr = m_pOutput->ConnectionMediaType(&mt);
    if (FAILED(hr))
    {
        return hr;
    }

    ASSERT(mt.formattype == FORMAT_VideoInfo);
    BITMAPINFOHEADER *pbmi = HEADER(mt.pbFormat);
    pProp->cbBuffer = DIBSIZE(*pbmi) * 2; 
    if (pProp->cbAlign == 0)
    {
        pProp->cbAlign = 1;
    }
    if (pProp->cBuffers == 0)
    {
        pProp->cBuffers = 1;
    }
    // Release the format block.
    FreeMediaType(mt);

    // Set allocator properties.
    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProp, &Actual);
    if (FAILED(hr)) 
    {
        return hr;
    }
    // Even when it succeeds, check the actual result.
    if (pProp->cbBuffer > Actual.cbBuffer) 
    {
        return E_FAIL;
    }
    return S_OK;
}

HRESULT Rascal::Transform(IMediaSample *pSource, IMediaSample *pDest)
{
    // Get pointers to the underlying buffers.
    BYTE *pBufferIn, *pBufferOut;
    HRESULT hr = pSource->GetPointer(&pBufferIn);
    if (FAILED(hr))
    {
        return hr;
    }
    hr = pDest->GetPointer(&pBufferOut);
    if (FAILED(hr))
    {
        return hr;
    }
    // Process the data.

	DWORD cbDest = doProceed(pBufferIn, pBufferOut);
    KASSERT((long)cbDest <= pDest->GetSize());

    pDest->SetActualDataLength(cbDest);
    pDest->SetSyncPoint(TRUE);
    return S_OK;
}

HRESULT Rascal::StopStreaming()
{
	m_pAddinMgr->unloadAddins();
	m_pAddinMgr = NULL;
	OutputDebugStringW(L"Rascal::StopStreaming()...DONE//");
	return S_OK;
}

HRESULT Rascal::StartStreaming()
{
	Rascal::m_pAddinMgr = AddinManager::getInstance();
	Rascal::m_pAddinMgr->loadAddins();
	
	OutputDebugStringW(L"Rascal::StartStreaming()...DONE//");
	return S_OK;
}

CUnknown * WINAPI Rascal::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr)
{
    Rascal *pFilter = new Rascal();
    if (pFilter== NULL) 
    {
        *pHr = E_OUTOFMEMORY;
    }
    return pFilter;
}

DWORD Rascal::doProceed(BYTE* pBufferIn, BYTE* pBufferOut)
{
	CopyMemory(pBufferOut, pBufferIn, this->m_sampleSize);
	m_pAddinMgr->process(pBufferOut, m_sampleSize, m_width, m_height, m_bitCount);
	return m_sampleSize;
}
