#include "StdAfx.h"
#include "CaptureVideo.h"
#include "DoFile.h"
#include "CommonConvert.h"


extern void MyWriteConsole(wstring szStrValue);

CCaptureVideo::CCaptureVideo(void) : m_pCrossbar(NULL)
,m_fCapAudio(FALSE)
,m_fCaptureGraphBuilt(FALSE)
,m_fPreviewGraphBuilt(FALSE)
,m_fCapturing(FALSE)
,m_fPreviewing(FALSE)
,m_fPreviewFaked(FALSE)
,m_dwGraphRegister(0)
,m_iMasterStream(-1)
,m_fWantPreview(TRUE)
,m_fUseFrameRate(FALSE)
,m_dFrameRate(0)
,m_pSampleGrapBuilder(NULL)
,m_pCB(NULL)
{
	m_szCurrentSelectVideoFriendName[0] = 0;
	m_szCurrentSelectAudioFriendName[0] = 0;
	m_wszCaptureFileName[0] = 0;

	///////////////////////////////////////
	m_dFrameRate = 10000000. / 666667;
	m_dFrameRate = (int)(m_dFrameRate * 100) / 100;

	memset(m_prgpmVideo,0,sizeof(IMoniker*)*10);
	memset(m_prgpmAudio,0,sizeof(IMoniker*)*10);

	memset(m_wszAudioDviceName,0,sizeof(WCHAR*)*10);
	memset(m_wszVideoDviceName,0,sizeof(WCHAR*)*10);

	m_pCB = new CSampleGrabberCB;
}

CCaptureVideo::~CCaptureVideo(void)
{
	if(m_fCapturing)
		StopCapture();
	if(m_fPreviewing)
		StopPerview();
	
	if(m_pCB){
		delete m_pCB;
		m_pCB = NULL;
	}

	FreeCapFilters();
}

void CCaptureVideo::EnumDevice()
{
	int length = NUMELMS(m_prgpmVideo);
	for(int i = 0; i < NUMELMS(m_prgpmVideo); i++)
	{
		IMonRelease(m_prgpmVideo[i]);
	}
	for(int i = 0; i < NUMELMS(m_prgpmAudio); i++)
	{
		IMonRelease(m_prgpmAudio[i]);
	}

	// enumerate all video capture devices
	ICreateDevEnum* pCreateDevEnum = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,(void**)&pCreateDevEnum);
	if(FAILED(hr)) return;

	IEnumMoniker* pIEnumMoniker = NULL;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pIEnumMoniker,0);
	if(hr != S_OK)
	{
		goto EnumAudio;
	}

	pIEnumMoniker->Reset();
	ULONG cFetched = 0;
	IMoniker* pM = NULL;
	UINT uIndex = 0;
	while(S_OK == pIEnumMoniker->Next(1,&pM,&cFetched))
	{
		IPropertyBag* pBag;

		VARIANT var;
		var.vt = VT_BSTR;

		hr = pM->BindToStorage(0,0,IID_IPropertyBag,(void**)&pBag);
		if(SUCCEEDED(hr))
		{
			pBag->Read(L"FriendlyName",&var,NULL);


			WCHAR* wszName = new WCHAR[120];
			wcscpy_s(wszName,120,var.bstrVal);
			m_wszVideoDviceName[uIndex] = wszName;


			SysFreeString(var.bstrVal);
			pBag->Release();
		}
		ASSERT(m_prgpmVideo[uIndex] == NULL);
		m_prgpmVideo[uIndex] = pM;
		//		pM->AddRef();
		uIndex++;
	}
	pIEnumMoniker->Release();
	/////枚举音频
EnumAudio:
	uIndex = 0;
	ASSERT(pCreateDevEnum != NULL);
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,&pIEnumMoniker,0);
	if(hr != S_OK)
	{
		pCreateDevEnum->Release();
		return;
	}
	pIEnumMoniker->Reset();

	ULONG uFetched;
	IMoniker* pAudioMoniker;
	while(S_OK == pIEnumMoniker->Next(1,&pAudioMoniker,&uFetched))
	{
		IPropertyBag* pBag;

		VARIANT var;
		var.vt = VT_BSTR;

		hr = pAudioMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&pBag);
		if(SUCCEEDED(hr))
		{
			pBag->Read(L"FriendlyName",&var,NULL);


			WCHAR* wszName = new WCHAR[120];
			wcscpy_s(wszName,120,var.bstrVal);
			m_wszAudioDviceName[uIndex] = wszName;


			SysFreeString(var.bstrVal);
			pBag->Release();
		}

		ASSERT(m_prgpmAudio[uIndex] == NULL);
		m_prgpmAudio[uIndex] = pAudioMoniker;
		/*pAudioMoniker->AddRef();*/
		uIndex++;
	}
	pCreateDevEnum->Release();
	pIEnumMoniker->Release();
}

inline void CCaptureVideo::IMonRelease(IMoniker *&pm)
{
	if(pm)
	{
		pm->Release();
		pm = 0;
	}
}

void CCaptureVideo::ChooseDevices(IMoniker* pmVideo,IMoniker* pmAudio)
{
	if(!pmVideo || !pmAudio)
		return;

	//选择了一个新设备，重建graph
	if(m_pImonikerVideo != pmVideo || m_pImonikerAudio != pmAudio)
	{
		m_pImonikerAudio.Release();
		m_pImonikerVideo.Release();

		m_pImonikerAudio = pmAudio;
		m_pImonikerVideo = pmVideo;
	}
}

void CCaptureVideo::ChooseDevices(TCHAR* szVideo,TCHAR* szAudio)
{
	WCHAR wszVideo[1024], wszAudio[1024];

	StringCchCopyN(wszVideo, NUMELMS(wszVideo), szVideo, NUMELMS(wszVideo)-1);
	StringCchCopyN(wszAudio, NUMELMS(wszAudio), szAudio, NUMELMS(wszAudio)-1);

	wszVideo[1023] = wszAudio[1023] = 0;

	IBindCtx *lpBC=0;
	IMoniker *pmVideo = 0, *pmAudio = 0;

	HRESULT hr = CreateBindCtx(0,&lpBC);
	if(SUCCEEDED(hr))
	{
		DWORD dwEaten;
		hr = MkParseDisplayName(lpBC, wszVideo, &dwEaten, &pmVideo);
		hr = MkParseDisplayName(lpBC, wszAudio, &dwEaten, &pmAudio);

		lpBC->Release();
	}

	ChooseDevices(pmVideo, pmAudio);
}

void CCaptureVideo::FreeCapFilters()
{
	if(m_pCrossbar)
	{
		delete m_pCrossbar;
		m_pCrossbar = NULL;
	}

	if(m_pSampleGrapBuilder)
	{
		delete m_pSampleGrapBuilder;
		m_pSampleGrapBuilder = NULL;
	}

	for(int i=0;i<NUMELMS(m_prgpmVideo);i++)
	{
		IMonRelease(m_prgpmVideo[i]);
	}

	for(int i=0;i<NUMELMS(m_prgpmAudio);i++)
	{
		IMonRelease(m_prgpmAudio[i]);
	}

	for(int i=0;i<NUMELMS(m_wszAudioDviceName);i++)
	{
		if(m_wszAudioDviceName[i])
			delete [] m_wszAudioDviceName[i];
	}

	for(int i=0;i<NUMELMS(m_wszVideoDviceName);i++)
	{
		if(m_wszVideoDviceName[i])
			delete [] m_wszVideoDviceName[i];
	}
}

BOOL CCaptureVideo::InitCapFilters()
{
	HRESULT hr = S_OK;
	BOOL flag = MakeBulider();
	if(!flag)
	{
		return FALSE;
	}
	//
	// First, we need a Video Capture filter, and some interfaces
	//
	m_pVCap = NULL;
	if(m_pImonikerVideo != NULL)

	{
		//得到Filter
		hr = m_pImonikerVideo->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&m_pVCap);
	}

	if(m_pVCap == NULL)
	{
		goto InitCapFiltersFail;
	}

	flag = MakeGraph();
	if(!flag)
	{
		goto InitCapFiltersFail;
	}
	///设置graph
	hr = m_pSampleGrapBuilder->SetFiltergraph(m_pGraph);
	if(hr != NOERROR)
	{
		goto InitCapFiltersFail;
	}

	///// Add the video capture filter to the graph with its friendly name
	hr = m_pGraph->AddFilter(m_pVCap,m_szCurrentSelectVideoFriendName);
	if(hr != NOERROR)
	{
		goto InitCapFiltersFail;
	}

	/////////////////////////
	if(!GetNeedInterface())
	{
		goto InitCapFiltersFail;
	}


	UseCrossbar();


	if(!InitAudio())
	{
		goto InitCapFiltersFail;
	}
	return TRUE;
	/////////////////////////
InitCapFiltersFail:
	FreeCapFilters();
	return FALSE;
}

BOOL CCaptureVideo::GetNeedInterface()
{
	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVTuners or Crossbars we might
	// need).

	// we use this interface to get the name of the driver
	// Don't worry if it doesn't work:  This interface may not be available
	// until the pin is connected, or it may not be available at all.
	// (eg: interface may not be available for some DV capture)
	HRESULT hr = S_OK;
	hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,m_pVCap,IID_IAMVideoCompression,(void**)&m_pVC);
	if(hr != S_OK)
	{
		hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pVCap,IID_IAMVideoCompression,(void**)&m_pVC);
	}
	// !!! What if this interface isn't supported?
	// we use this interface to set the frame rate and get the capture size
	hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,m_pVCap,IID_IAMStreamConfig,(void**)&m_pVSC);
	if(hr != NOERROR)
	{
		hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pVCap,IID_IAMStreamConfig,(void**)&m_pVSC);
		if(hr != NOERROR)
		{
			////没有这些接口
			return FALSE;
		}
	}

//	m_fCapAudioIsRelevant = TRUE;//暂时不明白什么意思

	//缺省的视屏格式
	AM_MEDIA_TYPE *pmt;
	if(m_pVSC && m_pVSC->GetFormat(&pmt) == S_OK)
	{
		// DV capture does not use a VIDEOINFOHEADER
		if(pmt->formattype == FORMAT_VideoInfo)
		{
			// resize our window to the default capture size
			// 			ResizeWindow(HEADER(pmt->pbFormat)->biWidth,
			// 				ABS(HEADER(pmt->pbFormat)->biHeight));
		}
		if(pmt->majortype != MEDIATYPE_Video)
		{
			// This capture filter captures something other that pure video.
			// Maybe it's DV or something?  Anyway, chances are we shouldn't
			// allow capturing audio separately, since our video capture
			// filter may have audio combined in it already!
//			m_fCapAudioIsRelevant = FALSE;
			m_fCapAudio = FALSE;
		}
		DeleteMediaType(pmt);
	}

	////////////////////视频属性设置接口
	// we use this interface to bring up the 3 dialogs
	// NOTE:  Only the VfW capture filter supports this.  This app only brings
	// up dialogs for legacy VfW capture drivers, since only those have dialogs
	hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video,m_pVCap,IID_IAMVfwCaptureDialogs,(void**)&m_pDlg);

	return TRUE;
}

BOOL CCaptureVideo::UseCrossbar()
{
	IPin* pPin = NULL;
	IEnumPins* pEnumPins = NULL;
	ULONG CFetched;
	PIN_INFO pinInfo;
	BOOL Found = FALSE;
	IKsPropertySet* pKs = 0;//此接口设置/获取WDM设备属性
	GUID guid;
	DWORD dw;
	BOOL fMatch = FALSE;
	HRESULT hr = S_OK;

	m_pCrossbar = NULL;

	if(SUCCEEDED(m_pVCap->EnumPins(&pEnumPins)))
	{
		while(!Found && S_OK == pEnumPins->Next(1,&pPin,&CFetched))
		{
			if(S_OK == pPin->QueryPinInfo(&pinInfo))
			{
				if(pinInfo.dir == PINDIR_INPUT)
				{
					// is this pin an ANALOGVIDEOIN input pin?
					if(S_OK == pPin->QueryInterface(IID_IKsPropertySet,(void**)&pKs))
					{
						if(S_OK == pKs->Get(AMPROPSETID_Pin,AMPROPERTY_PIN_CATEGORY,
							NULL,0,&guid,sizeof(guid),&dw))
						{
							if(guid == PIN_CATEGORY_ANALOGVIDEOIN)
								fMatch = TRUE;
						}
						pKs->Release();

						if(fMatch)
						{
							HRESULT hrCreate = S_OK;
							m_pCrossbar = new CCrossbar(pPin,&hrCreate);
							if(!m_pCrossbar || FAILED(hrCreate))
								break;
							hr = m_pCrossbar->GetInputCount(&m_NumberOfVideoInputs);
							Found = TRUE;
						}
					}
				}
				pinInfo.pFilter->Release();
			}
			pPin->Release();
		}
		pEnumPins->Release();
	}

	return TRUE;
}

BOOL CCaptureVideo::InitAudio()
{
	// there's no point making an audio capture filter
//	if(m_fCapAudioIsRelevant == FALSE)
//		goto SkipAudio;
	if(m_pImonikerAudio == NULL)
	{
		m_fCapAudio = FALSE;
		goto SkipAudio;
	}
	m_pACap = NULL;

	HRESULT hr = m_pImonikerAudio->BindToObject(0,0,IID_IBaseFilter,(void**)&m_pACap);
	if(FAILED(hr))
	{
		return FALSE;
	}
	if(m_pACap == NULL)//没有音频Filter
	{
		m_fCapAudio = FALSE;
		goto SkipAudio;
	}

	//得到了audio filter
	WCHAR wachAudioFriendlyName[256]={0};
	IPropertyBag *pPBag;
	// Read the friendly name of the filter to assist with remote graph
	// viewing through GraphEdit
	hr = m_pImonikerAudio->BindToStorage(NULL,NULL,IID_IPropertyBag,(void**)&pPBag);
	if(SUCCEEDED(hr))
	{
		VARIANT var;
		var.vt = VT_BSTR;

		hr = pPBag->Read(L"FriendlyName",&var,NULL);
		if(hr == NOERROR)
		{
			hr = StringCchCopyW(wachAudioFriendlyName, 256, var.bstrVal);
			SysFreeString(var.bstrVal);
		}
		pPBag->Release();
	}
	//得到audio Filter添加到pgrap
	hr = m_pGraph->AddFilter(m_pACap,wachAudioFriendlyName);
	if(hr != NOERROR)
	{
		goto SkipAudio;
	}
	// Calling FindInterface below will result in building the upstream
	// section of the capture graph (any WDM TVAudio's or Crossbars we might
	// need).

	// !!! What if this interface isn't supported?
	// we use this interface to set the captured wave format
	hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Audio, m_pACap,
		IID_IAMStreamConfig, (void **)&m_pASC);

	if(hr != NOERROR)
	{
		//		ErrMsg(TEXT("Cannot find ACapture:IAMStreamConfig"));
	}
	return TRUE;
SkipAudio:
	// Can this filter do closed captioning?
	IPin *pPin;
	hr = m_pSampleGrapBuilder->FindPin(m_pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_VBI,
		NULL, FALSE, 0, &pPin);
	if(hr != S_OK)
		hr = m_pSampleGrapBuilder->FindPin(m_pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_CC,
		NULL, FALSE, 0, &pPin);
	if(hr == S_OK)
	{
		pPin->Release();
		//		m_fCCAvail = TRUE;
	}
	// potential debug output - what the graph looks like
	// DumpGraph(gcap.pFg, 1);
	return TRUE;
}

BOOL CCaptureVideo::MakeBulider()
{
	if(m_pSampleGrapBuilder)
		return TRUE;
	m_pSampleGrapBuilder = new CSampleCGB;
	if(NULL == m_pSampleGrapBuilder)
		return FALSE;

	return TRUE;
}

BOOL CCaptureVideo::MakeGraph()
{
	if(m_pGraph)
		return TRUE;
	HRESULT hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC,
		IID_IGraphBuilder,(void**)&m_pGraph);
	return (hr == NOERROR ? TRUE : FALSE);
}

wstring CCaptureVideo::GetCurrentVideoName()
{
	wstring name(m_szCurrentSelectVideoFriendName);
	return name;
}

wstring CCaptureVideo::GetCurrentAudioName()
{
	wstring name(m_szCurrentSelectAudioFriendName);
	return name;
}

void CCaptureVideo::SetCurrentAudioName(const WCHAR* szAudioFriendName)
{
	if(!szAudioFriendName || szAudioFriendName[0] == 0)
		return;
	wcscpy_s(m_szCurrentSelectAudioFriendName,120,szAudioFriendName);
}

void CCaptureVideo::SetCurrentVideoName(const WCHAR* szVideoFriendName)
{
	if(!szVideoFriendName || szVideoFriendName[0] == 0)
		return;
	wcscpy_s(m_szCurrentSelectVideoFriendName,120,szVideoFriendName);
}

BOOL CCaptureVideo::BuildCaptureGraph()
{
	HRESULT hr;
	AM_MEDIA_TYPE *pmt;//媒体类型结构

	if(m_fCaptureGraphBuilt)//已经创建了captureGraph
		return TRUE;
	if(m_fCapturing)
		return FALSE;
	if(m_pVCap == NULL)
		return FALSE;
	if(m_pACap == NULL && m_fCapAudio)
		return FALSE;

	if(m_szCurrentSelectVideoFriendName[0] = 0)//捕获设备
		return FALSE;

	// we already have another graph built... tear down the old one
	if(m_fPreviewGraphBuilt)
		TearDownGraph();

	//
	// We need a rendering section that will write the capture file out in AVI
	// file format
	//
	GUID guid = MEDIASUBTYPE_Avi;
	//这种方法将基于PTYPE参数的值多路复用器过滤器。
	//对于AVI，它会创建AVI复用过滤器。对于ASF，它创建了WM ASF writer。
	//对于其他价值，它会创建确定的CLSID的过滤器。
	//add multiplexer to the filter graph，并返回一个指针在PPF参数的IBaseFilter接口。
	//如果支持IFileSinkFilter同时返回IFileSinkFilter接口
	hr = m_pSampleGrapBuilder->SetOutputFileName(&guid,m_wszCaptureFileName,&m_pRender,&m_pSink);
	if(FAILED(hr))
	{
		goto SetupCapturefail;
	}

	//得到IConfigAviMux接口并设置master stream
	//SetMasterStream函数的参数取决于renderstream,如果先video后audi
	//m_iMasterStream=0,video就为0,audio就为1
	m_iMasterStream = 0;

	hr = m_pRender->QueryInterface(IID_IConfigAviMux,(void**)&m_pConfigAviMux);
	if(SUCCEEDED(hr) && m_pConfigAviMux)
	{
		m_pConfigAviMux->SetOutputCompatibilityIndex(TRUE);
		if(m_fCapAudio)
		{
			hr = m_pConfigAviMux->SetMasterStream(m_iMasterStream);
		}
	}

	//
	// Render the video capture and preview pins - even if the capture filter only
	// has a capture pin (and no preview pin) this should work... because the
	// capture graph builder will use a smart tee filter to provide both capture
	// and preview.  We don't have to worry.  It will just work.
	//

	// NOTE that we try to render the interleaved pin before the video pin, because
	// if BOTH exist, it's a DV filter and the only way to get the audio is to use
	// the interleaved pin.  Using the Video pin on a DV filter is only useful if
	// you don't want the audio.
	hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,m_pVCap,NULL,m_pRender);
	if(FAILED(hr))
	{
		hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,m_pVCap,NULL,m_pRender);
		if(FAILED(hr))
		{
			goto SetupCapturefail;
		}
	}

	if(m_fWantPreview)
	{
		hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Interleaved,m_pVCap,NULL,NULL);
		if(hr == VFW_S_NOPREVIEWPIN)
		{
			m_fPreviewFaked = TRUE;
		}
		else if(hr != S_OK)
		{
			hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,&MEDIATYPE_Video,m_pVCap,NULL,NULL);
			if(hr == VFW_S_NOPREVIEWPIN)
			{
				m_fPreviewFaked = TRUE;
			}
			else if(hr != S_OK)
			{
				goto SetupCapturefail;
			}
		}
	}

	//
	// Render the audio capture pin?
	//
	if(m_fCapAudio)//如果要捕获音频
	{
		hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Audio,m_pACap,NULL,m_pRender);
		if(hr != NOERROR)
		{
			goto SetupCapturefail;
		}
	}

	//
	// Get the preview window to be a child of our app's window
	//

	// This will find the IVideoWindow interface on the renderer.  It is
	// important to ask the filtergraph for this interface... do NOT use
	// ICaptureGraphBuilder2::FindInterface, because the filtergraph needs to
	// know we own the window so it can give us display changed messages, etc.
	//得到视频显示窗口设置的接口IVideoWindow

	if(m_fWantPreview)
	{
		hr = m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);	
	}

	//设置捕获的帧率
	// now tell it what frame rate to capture at.  Just find the format it
	// is capturing with, and leave everything alone but change the frame rate

	if(m_fUseFrameRate && m_pVSC)
	{
		m_pVSC->GetFormat(&pmt);
		if(hr == NOERROR)
		{
			if(pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				pvi->AvgTimePerFrame = (LONGLONG)(10000000 / m_dFrameRate);
				hr = m_pVSC->SetFormat(pmt);
			}
			DeleteMediaType(pmt);
		}
	}

	///得到事件通知接口
	hr = m_pGraph->QueryInterface(IID_IMediaEventEx,(void**)&m_pME);
	///注册到table
#ifdef REGISTER_FILTERGRAPH
	hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
	if(FAILED(hr))
	{
		m_dwGraphRegister = 0;
	}
#endif

	// All done.
	m_fCaptureGraphBuilt = TRUE;
	return TRUE;
SetupCapturefail:
	TearDownGraph();
	return FALSE;
}


void CCaptureVideo::TearDownGraph()
{
	if(m_pSink) 
		m_pSink.Release();
	if(m_pConfigAviMux) 
		m_pConfigAviMux.Release();
	if(m_pME) 
		m_pME.Release();
	if(m_pRender) 
		m_pRender.Release();
	if(m_pDF) 
		m_pDF.Release();
	if(m_pGrabberFilter)
		m_pGrabberFilter.Release();

	if(m_pVW)
	{
		m_pVW->put_Owner(NULL);
		m_pVW->put_Visible(OAFALSE);
		m_pVW.Release();
	}

	// destroy the graph downstream of our capture filters
	if(m_pVCap)
		RemoveDownstream(m_pVCap);
	if(m_pACap)
		RemoveDownstream(m_pACap);

	// potential debug output - what the graph looks like
	// if (gcap.pFg) DumpGraph(gcap.pFg, 1);

#ifdef REGISTER_FILTERGRAPH
	// Remove filter graph from the running object table
	if(m_dwGraphRegister)
	{
		RemoveGraphFromRot(m_dwGraphRegister);
		m_dwGraphRegister = 0;
	}
#endif

	m_fCaptureGraphBuilt = FALSE;
	m_fPreviewGraphBuilt = FALSE;
	m_fPreviewFaked = FALSE;
}

void CCaptureVideo::RemoveDownstream(IBaseFilter *pf)
{
	IPin* pP = 0, *pTo = 0;
	ULONG uFetched;
	IEnumPins *pEnumPins = NULL;
	PIN_INFO pininfo;

	if(!pf)
		return;

	HRESULT hr = pf->EnumPins(&pEnumPins);
	if(FAILED(hr)) return;

	pEnumPins->Reset();

	while(S_OK == pEnumPins->Next(1,&pP,&uFetched))
	{	
		if(pP)
		{
			hr = pP->ConnectedTo(&pTo);
			if(SUCCEEDED(hr) && pTo)
			{
				hr = pTo->QueryPinInfo(&pininfo);
				if(SUCCEEDED(hr))
				{
					if(pininfo.dir == PINDIR_INPUT)
					{
						RemoveDownstream(pininfo.pFilter);
						m_pGraph->Disconnect(pTo);
						m_pGraph->Disconnect(pP);
						m_pGraph->RemoveFilter(pininfo.pFilter);
					}
					pininfo.pFilter->Release();
				}
				pTo->Release();
			}
			pP->Release();
		}
	}
	if(pEnumPins)
		pEnumPins->Release();
}

BOOL CCaptureVideo::SetCaptureFile(WCHAR* szFileName)
{
	if(m_pSink && szFileName)
	{
		m_pSink->SetFileName(szFileName,NULL);
		//		SetCapturFileName(szFileName);
		return TRUE;
	}
	return FALSE;
}

BOOL CCaptureVideo::SaveCaptureFile(WCHAR* szFileNameSoure,WCHAR* wszFileNameDest)
{
	if(!m_pSampleGrapBuilder || !szFileNameSoure || !wszFileNameDest)
		return FALSE;
	ICaptureGraphBuilder2* pBuilder;

	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC,IID_ICaptureGraphBuilder2,(void**)&pBuilder);
	if(hr == NOERROR)
	{
		CProgress pProg;
		IAMCopyCaptureFileProgress* pIProg = NULL;
		hr = pProg.QueryInterface(IID_IAMCopyCaptureFileProgress,(void**)&pIProg);
		hr = pBuilder->CopyCaptureFile(szFileNameSoure,wszFileNameDest,TRUE,pIProg);
		pProg.Release();
		pBuilder->Release();
	}

	return ((hr == S_OK) ? TRUE : FALSE);
}

BOOL CCaptureVideo::AllocCaptureFile(DWORD dSize,WCHAR* szFileName)
{
	if(m_pSampleGrapBuilder && szFileName)
	{
		HRESULT hr = m_pSampleGrapBuilder->AllocCapFile(szFileName,dSize * 1024L * 1024L);
		return ((hr == NOERROR) ? TRUE : FALSE);
	}
	return FALSE;
}

BOOL CCaptureVideo::SetWindowStyle(long nStyle)
{
	if(m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_WindowStyle(nStyle);
		m_pVW->put_Visible(OATRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CCaptureVideo::SetWindowRect(const RECT rect)
{
	if(m_pVW)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Left(rect.left);
		m_pVW->put_Top(rect.top);
		m_pVW->put_Width(rect.right - rect.left);
		m_pVW->put_Height(rect.bottom - rect.top);
		m_pVW->put_Visible(OATRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CCaptureVideo::PutOwner(HWND hWnd)
{
	if(m_pVW)
	{
		return SUCCEEDED(m_pVW->put_Owner((OAHWND)hWnd));
	}
	return FALSE;
}

BOOL CCaptureVideo::SetWindowVisble(BOOL flag)
{
	if(m_pVW)
	{
		return SUCCEEDED(m_pVW->put_Visible(flag ? OATRUE : OAFALSE));
	}
	return FALSE;
}

BOOL CCaptureVideo::SetWindowPosition(long left,long top,long right,long bottom)
{
	if(m_pVW)
	{
		return SUCCEEDED(m_pVW->SetWindowPosition(left,top,right - left,bottom - top));
	}
	return FALSE;
}

BOOL CCaptureVideo::GetWindowPosition(long* left,long* top,long* width,long* height)
{
	if(m_pVW)
	{
		return SUCCEEDED(m_pVW->GetWindowPosition(left,top,width,height));
	}
	return FALSE;
}

BOOL CCaptureVideo::SetWindowEvent(HWND hWnd)
{
	if(m_pME)
	{
		return SUCCEEDED(m_pME->SetNotifyWindow((OAHWND)hWnd,WM_FGNOTIFY,0));
	}
	return TRUE;
}

BOOL CCaptureVideo::SetCapturFileName(const WCHAR* szVideoFileName)
{
	if(szVideoFileName != NULL && szVideoFileName[0] != 0)
	{
		wcscpy_s(m_wszCaptureFileName,MAX_PATH,szVideoFileName);
		return TRUE;
	}
	return FALSE;
}

BOOL CCaptureVideo::StartCapture()
{
	if(m_fCapturing)
		return TRUE;

	//	BOOL f, fHasStreamControl;
	HRESULT hr;

	if(m_fPreviewing)
		StopPerview();

	if(!m_fCaptureGraphBuilt)
		return FALSE;

	long lDroppedBase = 0;
	long lNotBase = 0;

	REFERENCE_TIME start = MAXLONGLONG, stop = MAXLONGLONG;
	///设置
	hr = m_pSampleGrapBuilder->ControlStream(&PIN_CATEGORY_CAPTURE,NULL,NULL,&start,NULL,0,0);

	// prepare to run the graph
	IMediaControl* pMC = NULL;
	hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&pMC);
	if(FAILED(hr))
	{
		return FALSE;
	}
	//run
	hr= pMC->Run();
	if(FAILED(hr))
	{
		pMC->Stop();
		pMC->Release();
	}

	//start capture now
	if(!m_pDF)
	{
		hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Interleaved,
			m_pVCap,IID_IAMDroppedFrames,(void**)&m_pDF);
		if(hr != NOERROR)
		{
			hr = m_pSampleGrapBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video,
				m_pVCap,IID_IAMDroppedFrames,(void**)&m_pDF);
		}
	}

	//打开capture pin
	hr = m_pSampleGrapBuilder->ControlStream(&PIN_CATEGORY_CAPTURE,NULL,NULL,NULL,
		&stop,0,0);
	if(m_pDF)
	{
		m_pDF->GetNumDropped(&lDroppedBase);
		m_pDF->GetNumNotDropped(&lNotBase);
	}

	pMC->Release();
	m_fCapturing = TRUE;
	return TRUE;
}

BOOL CCaptureVideo::StartPerview()
{
	if(m_fPreviewing)
		return TRUE;
	if(!m_fPreviewGraphBuilt)
		return FALSE;

	m_pSampleGrabber->SetBufferSamples(FALSE);
	m_pSampleGrabber->SetOneShot(FALSE);
	if(m_pCB)
		m_pSampleGrabber->SetCallback(m_pCB,1);

	//run
	IMediaControl* pMediaCtrl = NULL;
	HRESULT hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&pMediaCtrl);
	if(SUCCEEDED(hr))
	{
		hr = pMediaCtrl->Run();
		if(FAILED(hr))
		{
			pMediaCtrl->Stop();
		}
		pMediaCtrl->Release();
	}
	if(FAILED(hr))
		return FALSE;
	
	

	m_fPreviewing = TRUE;
	return TRUE;
}

BOOL CCaptureVideo::StopPerview()
{
	if(!m_fPreviewing)
		return FALSE;
	if(!m_fPreviewGraphBuilt)
		return FALSE;

	// stop the graph
	IMediaControl* pMediaCtr = NULL;
	HRESULT hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&pMediaCtr);
	if(FAILED(hr))
	{
		return FALSE;
	}
	pMediaCtr->Stop();
	pMediaCtr->Release();
	m_fPreviewing = FALSE;
	////////////////////
	return TRUE;
}

BOOL CCaptureVideo::BuildPreviewGraph()
{
	HRESULT hr = S_OK;
	AM_MEDIA_TYPE *pmt;

	if(m_fPreviewGraphBuilt)
		return TRUE;
	if(m_fPreviewing || m_fCapturing)
		return FALSE;
	
	if(m_pVCap == NULL)
		return FALSE;

	if(m_fCaptureGraphBuilt)
		TearDownGraph();
	
	if(!BuildSampleGrabber())
		return FALSE;

	//Render
	hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
		&MEDIATYPE_Interleaved, m_pVCap,m_pGrabberFilter, NULL);
	if(hr == VFW_S_NOPREVIEWPIN)
	{
		// preview was faked up for us using the (only) capture pin
		m_fPreviewFaked = TRUE;
	}
	else if(hr != S_OK)
	{
		// maybe it's DV?
		hr = m_pSampleGrapBuilder->RenderStream(&PIN_CATEGORY_PREVIEW,
			&MEDIATYPE_Video, m_pVCap, m_pGrabberFilter, NULL);
		if(hr == VFW_S_NOPREVIEWPIN)
		{
			// preview was faked up for us using the (only) capture pin
			m_fPreviewFaked = TRUE;
		}
		else if(hr != S_OK)
		{
			m_fPreviewGraphBuilt = FALSE;
			return FALSE;
		}
	}

	//得到IVideoWindow接口
	hr = m_pGraph->QueryInterface(IID_IVideoWindow, (void **)&m_pVW);
	//设置帧率
	if(m_fUseFrameRate && m_pVSC)
	{
		m_pVSC->GetFormat(&pmt);
		if(hr == NOERROR)
		{
			if(pmt->formattype == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
				pvi->AvgTimePerFrame = (LONGLONG)(10000000 / m_dFrameRate);
				hr = m_pVSC->SetFormat(pmt);
			}
			DeleteMediaType(pmt);
		}
	}

	///得到事件通知接口
	hr = m_pGraph->QueryInterface(IID_IMediaEventEx,(void**)&m_pME);
	///注册到table
#ifdef REGISTER_FILTERGRAPH
	hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
	if(FAILED(hr))
	{
		m_dwGraphRegister = 0;
	}
#endif

	// All done.
	m_fPreviewGraphBuilt = TRUE;
	return TRUE;
}

HRESULT CCaptureVideo::GetCaptureEvent( __RPC__out long *lEventCode, __RPC__out LONG_PTR *lParam1, __RPC__out LONG_PTR *lParam2, long msTimeout )
{
	return m_pME->GetEvent(lEventCode,lParam1,lParam2,msTimeout);
}

HRESULT CCaptureVideo::FreeCaptureEventParams( long lEvCode, LONG_PTR lParam1, LONG_PTR lParam2 )
{
	return m_pME->FreeEventParams(lEvCode,lParam1,lParam2);
}

BOOL CCaptureVideo::StopCapture()
{
	if(!m_fCapturing)
		return FALSE;

	IMediaControl* pMC = NULL;
	HRESULT hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&pMC);
	if(SUCCEEDED(hr))
	{
		pMC->Stop();
		pMC->Release();
	}
	if(FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;
}
/*
BOOL CCaptureVideo::TakePhoto( const WCHAR* szFilePath )
{
	if(m_pGraph)
	{
		IBasicVideo* pIBasicVideo = NULL;
		HRESULT hr = m_pGraph->QueryInterface(IID_IBasicVideo,(void**)&pIBasicVideo);
		if(hr != NOERROR)
		{
// #ifdef _DEBUG
// 			MyWriteConsole(_T("得到pIBasicVideo失败"));
// #endif
			return FALSE;
		}
		if(pIBasicVideo)
		{
			long bitMapsize = 0;
			//首先获取图像大小
			if(SUCCEEDED(pIBasicVideo->GetCurrentImage(&bitMapsize,NULL)))
			{
// #ifdef _DEBUG
// 				MyWriteConsole(_T("获取图像大小失败"));
// #endif
				//分配图像帧内存
				unsigned char* buffer = new unsigned char[bitMapsize];
				memset(buffer,0,bitMapsize);
				//获取图像帧数据
				if(SUCCEEDED(pIBasicVideo->GetCurrentImage(&bitMapsize,(long*)buffer)))
				{
// #ifdef _DEBUG
// 					MyWriteConsole(_T("获取图像帧失败"));
// #endif
					//写入文件
					BITMAPFILEHEADER hdr;
					LPBITMAPINFOHEADER lpbi;
					lpbi = (LPBITMAPINFOHEADER)buffer;
					DWORD nColors = ( 1 << lpbi->biBitCount );
					if(nColors > 256)
						nColors = 0;
					hdr.bfType = 0x4D42;//((WORD)('M' << 8) | 'B');

					hdr.bfSize = bitMapsize + sizeof(hdr);
					
					hdr.bfReserved1 = 0;
					hdr.bfReserved2 = 0;

					hdr.bfOffBits = (DWORD) (sizeof(BITMAPFILEHEADER) + lpbi->biSize + nColors * (sizeof(RGBQUAD)));
					
					CDoFile doFile;
					std::wstring path(szFilePath);
					if(doFile.MyOpenFile(path,CDoFile::modeCreate | CDoFile::modeWrite | CDoFile::typeBinary))
					{
						doFile.MyWriteFile(&hdr,sizeof(BITMAPFILEHEADER));
						doFile.MyWriteFile(buffer,bitMapsize);
						doFile.MyCloseFile();
						delete [] buffer;
// #ifdef _DEBUG
// 						MyWriteConsole(path);
// #endif
						return TRUE;
					}
				}
				delete [] buffer;
			}
		}
	}
// #ifdef _DEBUG
// 	MyWriteConsole(_T("failed"));
// #endif
	return FALSE;
}
*/
BOOL CCaptureVideo::TakePhoto(const wstring& szFileName)
{
	if(!m_pCB) return FALSE;
	
	

	AM_MEDIA_TYPE mt;
	HRESULT hr = m_pSampleGrabber->GetConnectedMediaType(&mt);
	if(hr != S_OK) return FALSE;
	VIDEOINFOHEADER *pVideoHeader = (VIDEOINFOHEADER*)mt.pbFormat;
	if(pVideoHeader == NULL) return FALSE;
	
	
	long size;
	unsigned char* pBuffer = NULL;
	m_pCB->GetBuffer(&size,&pBuffer);
	if(pBuffer == NULL)
	{
		FreeMediaType(mt); 
		return FALSE; 
	}
	//生成HBITMAP
// 	BITMAPINFO BitmapInfo;
// 	ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
// 	CopyMemory(&BitmapInfo.bmiHeader, &(pVideoHeader->bmiHeader), sizeof(BITMAPINFOHEADER));
// 
// 	HBITMAP hBitmap;
// 	hBitmap = ::CreateDIBitmap(::GetDC(NULL), &(pVideoHeader->bmiHeader), CBM_INIT, pBuffer, &BitmapInfo, DIB_RGB_COLORS);
// 	if(hBitmap == NULL) return FALSE;
	
	
	//DIB写入文件
	//写入文件
	BITMAPFILEHEADER bfh; 
	memset( &bfh, 0, sizeof( bfh ) ); 
	bfh.bfType = 'MB'; 
	bfh.bfSize = sizeof( bfh ) + size + sizeof( BITMAPINFOHEADER ); 
	bfh.bfOffBits = sizeof(BITMAPINFOHEADER)+sizeof( BITMAPFILEHEADER ); 
	
	BITMAPINFOHEADER bih; 
	memset( &bih, 0, sizeof( bih ) );
	bih.biSize = sizeof( bih ); 
	bih.biWidth = pVideoHeader->bmiHeader.biWidth; 
	bih.biHeight = pVideoHeader->bmiHeader.biHeight; 
	bih.biPlanes = pVideoHeader->bmiHeader.biPlanes; 
	bih.biBitCount = pVideoHeader->bmiHeader.biBitCount; 

	CDoFile doFile;
	if(doFile.MyOpenFile(szFileName,CDoFile::modeCreate | CDoFile::modeWrite | CDoFile::typeBinary))
	{
		doFile.MyWriteFile(&bfh,sizeof(BITMAPFILEHEADER));
		doFile.MyWriteFile(&bih,sizeof(BITMAPINFOHEADER));
		doFile.MyWriteFile(pBuffer,size);
		doFile.MyCloseFile();
#ifdef _DEBUG
		MyWriteConsole(szFileName);
#endif
	}

	FreeMediaType(mt); 
	return TRUE;
}

void CCaptureVideo::GetAcaptRGB(AM_MEDIA_TYPE* mt)
{
	HDC hdc = GetDC(NULL);
	int iBitDepth = GetDeviceCaps(hdc,BITSPIXEL);
	ReleaseDC(NULL,hdc);
	switch(iBitDepth)
	{
	case 1:
		mt->subtype = MEDIASUBTYPE_RGB1;
		break;
	case 4:
		mt->subtype = MEDIASUBTYPE_RGB4;
		break;
	case 8:
		mt->subtype = MEDIASUBTYPE_RGB8;
		break;
	case 16:
		mt->subtype = MEDIASUBTYPE_RGB555;
		break;
	case 24:
		mt->subtype = MEDIASUBTYPE_RGB24;
		break;
	case 32:
		mt->subtype = MEDIASUBTYPE_RGB32;
		break;
	}
}

BOOL CCaptureVideo::BuildSampleGrabber()
{
	HRESULT hr = CoCreateInstance(CLSID_SampleGrabber,NULL,CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void**)&m_pGrabberFilter);
	if(hr != NOERROR)
	{
		return FALSE;
	}

	hr = m_pGraph->AddFilter(m_pGrabberFilter,L"Sample Grabber");
	if(hr != NOERROR)
	{
		return FALSE;
	}

	//得到接口
	hr = m_pGrabberFilter->QueryInterface(IID_ISampleGrabber,(void**)&m_pSampleGrabber);
	if(hr != NOERROR)
	{
		return FALSE;
	}
	//设置mediatype媒体类型
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	GetAcaptRGB(&mt);

	hr = m_pSampleGrabber->SetMediaType(&mt);
	FreeMediaType(mt);
	if(hr != NOERROR)
	{
		return FALSE;
	}

	return TRUE;
}