#pragma once

#include "vfw.h"
#include <dshow.h>
#include <uuids.h>
#include <strmif.h>
#include "afxtempl.h"
#include "BuildCfg.h"

#pragma comment (lib,"Strmiids")
#pragma comment (lib,"quartz")
#pragma comment (lib,"vfw32")

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#define RESOLUTION_SUPPORTED 2

class Resolution
{
private:
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	CString strResolution;
public:
	Resolution(DWORD dwWidth, DWORD dwHeight)
	{
		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
		strResolution.Format(L"%d * %d", m_dwWidth, m_dwHeight);
	}

	DWORD getWidth()
	{
		return m_dwWidth;
	}

	DWORD getHeight()
	{
		return m_dwHeight;
	}

	CString& toString()
	{
		return strResolution;
	}
};

class Cortez
{
public:
	CString				m_devList[MAX_SUPPORTED_DEVICES];
	CString				m_devListAu[MAX_SUPPORTED_DEVICES];
	CString				m_output;
	CWnd*				m_prvwWin;
	DWORD				m_dwDevNum;
	DWORD				m_dwDevNumAu;
	BOOL			    m_Completed;
	REFERENCE_TIME      m_TotalTime;
	
private:
	IVideoWindow*		m_pVWin;
	IMediaControl*		m_pMediaControl; 
	IMoniker*			m_pMoniker;
	IBaseFilter*		m_pCap;
	IBaseFilter*		m_pCapAu;
	IBaseFilter*		m_pAviDec;
	IBaseFilter*		m_pMux; 
	IBaseFilter*		m_pCompressor;
	IBaseFilter*		m_pCompressorAu;
	IBaseFilter*		m_pRascal;
	IBaseFilter*		m_pRascalPreview;
	IFileSinkFilter2*	m_pSink;
	IGraphBuilder*		m_pGraph;
	ICaptureGraphBuilder2* m_pBuilder;
	
private:
	Resolution* m_pResolution;
	Resolution  m_ResolutionHigh;
	Resolution  m_ResolutionLow;

	bool m_isAudioDisabled;
	bool m_isInCaptureProgress;
	bool m_isCapturePaused;
	static 
	bool m_isRascalEnabled;
	bool m_isYUV2;

public:
	Cortez(void);
	~Cortez(void);

public:
	bool beginPreview(void);
	bool beginCapture(void);
	bool pauseCapture(void);
	bool resumeCapture(void);
	bool stopCapture(void);
	bool setOutputFile(CString& strFileName);
	Resolution* getHighResolution();
	Resolution* getLowResolution();
	Resolution* getCurrentResolution();

public:
	int  initDevice(void);
	bool bind2ui(CWnd* prvwWin);
	bool bind2device(DWORD dwDevIndex);
	bool bind2deviceAu(DWORD dwDevIndex);
	void setAudioDisabled(bool audioDisabled);
	void setVideoResolution(Resolution* resolution);
	bool isInCaptureProgress(void);
	bool isCapturePaused(void);
	void getDeviceList(CArray<CString>& deviceList);
	void getDeviceListAu(CArray<CString>& deviceList);
	void buildDefaultFileName(CString& strFile);
	static void setRascalEnabled(bool rascalEnabled);

private:
	bool setVideoFormat();
	bool initCompressor();
	bool initCompressorAu();
	bool initRascal();
	bool initFilters();
	bool initAviDec();
	bool initPreviewWin();
	bool renderAudioStream();
	bool renderVideoStream();
	bool renderVideoStreamForPreview();
	bool enumDevices(void);
	bool enumDevicesAu(void);
	IPin*  findPin(IBaseFilter *pFilter, PIN_DIRECTION dir);

};

static const GUID CLSID_RASCAL = 
{	
	0x6662e9ec, 0xcd4f, 0x4f2e,
	{0x94, 0x7c, 0x60, 0xa8, 0xbf, 0xd, 0x22, 0x41}
};
