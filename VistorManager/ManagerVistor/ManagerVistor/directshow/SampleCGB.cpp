#include "StdAfx.h"
#include "SampleCGB.h"

#ifndef __STREAMS__
#define __STREAMS__
#endif
#include "ksproxy.h"

static 
BYTE
Mpeg2ProgramVideo [] = {
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcSource.left              = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcSource.top               = 0x00000000
	0xD0, 0x02, 0x00, 0x00,                         //  .hdr.rcSource.right             = 0x000002d0
	0xE0, 0x01, 0x00, 0x00,                         //  .hdr.rcSource.bottom            = 0x000001e0
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcTarget.left              = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcTarget.top               = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcTarget.right             = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.rcTarget.bottom            = 0x00000000
	0x00, 0x09, 0x3D, 0x00,                         //  .hdr.dwBitRate                  = 0x003d0900
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.dwBitErrorRate             = 0x00000000
	0x63, 0x17, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, //  .hdr.AvgTimePerFrame            = 0x0000000000051763
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.dwInterlaceFlags           = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.dwCopyProtectFlags         = 0x00000000
	0x04, 0x00, 0x00, 0x00,                         //  .hdr.dwPictAspectRatioX         = 0x00000004
	0x03, 0x00, 0x00, 0x00,                         //  .hdr.dwPictAspectRatioY         = 0x00000003
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.dwReserved1                = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.dwReserved2                = 0x00000000
	0x28, 0x00, 0x00, 0x00,                         //  .hdr.bmiHeader.biSize           = 0x00000028
	0xD0, 0x02, 0x00, 0x00,                         //  .hdr.bmiHeader.biWidth          = 0x000002d0
	0xE0, 0x01, 0x00, 0x00,                         //  .hdr.bmiHeader.biHeight         = 0x00000000
	0x00, 0x00,                                     //  .hdr.bmiHeader.biPlanes         = 0x0000
	0x00, 0x00,                                     //  .hdr.bmiHeader.biBitCount       = 0x0000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.bmiHeader.biCompression    = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.bmiHeader.biSizeImage      = 0x00000000
	0xD0, 0x07, 0x00, 0x00,                         //  .hdr.bmiHeader.biXPelsPerMeter  = 0x000007d0
	0x27, 0xCF, 0x00, 0x00,                         //  .hdr.bmiHeader.biYPelsPerMeter  = 0x0000cf27
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.bmiHeader.biClrUsed        = 0x00000000
	0x00, 0x00, 0x00, 0x00,                         //  .hdr.bmiHeader.biClrImportant   = 0x00000000
	0x98, 0xF4, 0x06, 0x00,                         //  .dwStartTimeCode                = 0x0006f498
	0x56, 0x00, 0x00, 0x00,                         //  .cbSequenceHeader               = 0x00000056
	0x02, 0x00, 0x00, 0x00,                         //  .dwProfile                      = 0x00000002
	0x02, 0x00, 0x00, 0x00,                         //  .dwLevel                        = 0x00000002
	0x00, 0x00, 0x00, 0x00,                         //  .Flags                          = 0x00000000
	//  .dwSequenceHeader [1]
	0x00, 0x00, 0x01, 0xB3, 0x2D, 0x01, 0xE0, 0x24,
	0x09, 0xC4, 0x23, 0x81, 0x10, 0x11, 0x11, 0x12, 
	0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x14, 0x14, 
	0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x15, 
	0x15, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 
	0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 
	0x18, 0x18, 0x18, 0x19, 0x18, 0x18, 0x18, 0x19, 
	0x1A, 0x1A, 0x1A, 0x1A, 0x19, 0x1B, 0x1B, 0x1B, 
	0x1B, 0x1B, 0x1C, 0x1C, 0x1C, 0x1C, 0x1E, 0x1E, 
	0x1E, 0x1F, 0x1F, 0x21, 0x00, 0x00, 0x01, 0xB5, 
	0x14, 0x82, 0x00, 0x01, 0x00, 0x00
} ;

static
BYTE
MPEG1AudioFormat [] = {
	0x50, 0x00, 0x02, 0x00, 0x80, 0xBB, 0x00, 0x00,
	0x00, 0x7D, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
	0x16, 0x00, 0x02, 0x00, 0x00, 0xE8, 0x03, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x1C, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
} ;

CSampleCGB::CSampleCGB(void)
{
	AudPID_ = 0xC0;
	VidPID_ = 0xE0;
	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, 
		IID_ICaptureGraphBuilder2, (void**)&graphBuilder2_ ); 
	ASSERT( S_OK == hr );
}

CSampleCGB::~CSampleCGB(void)
{
}

void CSampleCGB::ReleaseFilters()
{
	// 	pMultiplexer_.Release();
	// 	pEncoder_.Release();
	// 	pMPEG2Demux_.Release();
	pMediaControl_.Release();
	pAudioPin_.Release();
	pVideoPin_.Release();
}

HRESULT CSampleCGB::AllocCapFile(LPCOLESTR lpwstr, DWORDLONG dwlSize)
{
	return graphBuilder2_->AllocCapFile( lpwstr, dwlSize );
}

HRESULT CSampleCGB::ControlStream(const GUID *pCategory,const GUID *pType,
								  IBaseFilter *pFilter,REFERENCE_TIME *pstart,REFERENCE_TIME *pstop,
								  WORD wStartCookie,WORD wStopCookie)
{
	return graphBuilder2_->ControlStream(pCategory,pType,pFilter,pstart,pstop,wStartCookie,wStopCookie);
}

HRESULT CSampleCGB::CopyCaptureFile(LPOLESTR lpwstrOld,
									LPOLESTR lpwstrNew,
									int fAllowEscAbort,
									IAMCopyCaptureFileProgress *pCallback)
{
	return graphBuilder2_->CopyCaptureFile( lpwstrOld, lpwstrNew, 
		fAllowEscAbort, pCallback );
}

HRESULT CSampleCGB::FindInterface(const GUID *pCategory,
								  const GUID *pType,
								  IBaseFilter *pf,
								  REFIID riid,
								  void **ppint)
{
	return graphBuilder2_->FindInterface( pCategory, pType, pf, riid, ppint );
}

HRESULT CSampleCGB::FindPin(IUnknown *pSource,
							PIN_DIRECTION pindir,
							const GUID *pCategory,
							const GUID *pType,
							BOOL fUnconnected,
							int num,
							IPin **ppPin
							)
{
	return graphBuilder2_->FindPin(pSource,pindir,pCategory,pType,fUnconnected,num,ppPin);
}

HRESULT CSampleCGB::GetFiltergraph(IGraphBuilder **ppfg)
{
	return graphBuilder2_->GetFiltergraph(ppfg);
}

HRESULT CSampleCGB::SetFiltergraph(IGraphBuilder* pfg)
{
	return graphBuilder2_->SetFiltergraph(pfg);
}

HRESULT CSampleCGB::SetOutputFileName(const GUID *pType,
									  LPCOLESTR lpwstrFile,
									  IBaseFilter **ppf,
									  IFileSinkFilter **pSink
									  )
{
	if(!pType || !lpwstrFile || !ppf || !pSink || lpwstrFile[0]==0)
	{
		return E_INVALIDARG;
	}
	if(!::IsEqualGUID(*pType,MEDIASUBTYPE_Mpeg2))
	{
		return graphBuilder2_->SetOutputFileName(pType,lpwstrFile,ppf,pSink);
	}

	HRESULT hr;
	if(!graph_)
	{
		hr = GetFiltergraph(&graph_);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	//
	//  Configure the dump filter
	//
	SmartPtr<IFileSinkFilter> pDump;
	hr = CoCreateInstance(CLSID_Dump,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void**)&pDump);
	if(FAILED(hr))return hr;

	hr = pDump->SetFileName(lpwstrFile,NULL);
	if(FAILED(hr))return hr;

	hr = pDump->QueryInterface(&pMPEG2Demux_);
	if(FAILED(hr))return hr;

	hr = graph_->AddFilter(pMPEG2Demux_,L"Dump");
	if(FAILED(hr))
	{
		pMPEG2Demux_ = NULL;
		return hr;
	}

	*pSink = pDump;
	return S_OK;
}

HRESULT CSampleCGB::RenderStream( const GUID *pCategory,
								 const GUID *pType,
								 IUnknown *pSource,
								 IBaseFilter *pIntermediate,
								 IBaseFilter *pSink )
{
	if( !pType ||  !::IsEqualGUID( MEDIATYPE_Stream, *pType ) )
	{
		return graphBuilder2_->RenderStream( pCategory, pType, pSource, 
			pIntermediate, pSink );
	}

	HRESULT hr;
	if(!graph_)
	{
		hr = GetFiltergraph(&graph_);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	//
	//  try to build MPEG2 graph
	//
	SmartPtr< IBaseFilter > captureFilter;
	hr = pSource->QueryInterface(& captureFilter);
	if(FAILED(hr))
	{
		return E_INVALIDARG;
	}

	return hr;
}

HRESULT CSampleCGB::CreateVideoPin( IMpeg2Demultiplexer *pIMpeg2Demux )
{
	if(!pIMpeg2Demux)return E_INVALIDARG;

	AM_MEDIA_TYPE amTypeVideo;
	amTypeVideo.majortype = MEDIATYPE_Video;
	amTypeVideo.subtype = MEDIASUBTYPE_MPEG2_VIDEO;
	amTypeVideo.bFixedSizeSamples = TRUE;
	amTypeVideo.bTemporalCompression = 0;
	amTypeVideo.formattype = FORMAT_MPEG2_VIDEO;
	amTypeVideo.pUnk = NULL;
	amTypeVideo.cbFormat = sizeof(Mpeg2ProgramVideo);
	amTypeVideo.pbFormat = Mpeg2ProgramVideo;

	//
	// Create video pin
	//

	SmartPtr<IPin> pVideoOutPin;
	HRESULT hr = pIMpeg2Demux->CreateOutputPin(&amTypeVideo,L"MpegVideo",&pVideoOutPin);
	if(FAILED(hr))return hr;

	SmartPtr<IMPEG2StreamIdMap> pIVideoPIDMap;
	hr = pVideoOutPin->QueryInterface(&pIVideoPIDMap);
	if(FAILED(hr))return hr;


	hr = pIVideoPIDMap->MapStreamId(VidPID_,MPEG2_PROGRAM_ELEMENTARY_STREAM,0,0);
	if(FAILED(hr))return hr;
	return hr;
}

HRESULT CSampleCGB::CreateAudioPin( IMpeg2Demultiplexer *pIMpeg2Demux )
{
	if(!pIMpeg2Demux)return E_INVALIDARG;

	//
	// for audio: could be Mpeg1, Mpeg2, AC3: if Mpeg1 failed (connect failed) try Mpeg2.if failed tried AC3
	// Audio struct of AC3 can be copied from dev code.
	//
	AM_MEDIA_TYPE amTypeAudio;
	amTypeAudio.majortype = MEDIATYPE_Audio;
	amTypeAudio.subtype = MEDIASUBTYPE_MPEG2_AUDIO;
	amTypeAudio.bFixedSizeSamples = TRUE;
	amTypeAudio.bTemporalCompression = 0;
	amTypeAudio.formattype = FORMAT_WaveFormatEx;
	amTypeAudio.pUnk = NULL;
	amTypeAudio.cbFormat = sizeof( MPEG1AudioFormat );
	amTypeAudio.pbFormat = MPEG1AudioFormat;

	SmartPtr<IPin> pAudioOutPin;
	HRESULT hr = pIMpeg2Demux->CreateOutputPin(&amTypeAudio,L"MpegAudio",&pAudioOutPin);
	if(FAILED(hr))return hr;

	SmartPtr<IMPEG2StreamIdMap> pIAudioPIDMap;
	hr = pAudioOutPin->QueryInterface(&pIAudioPIDMap);
	if(FAILED(hr))return hr;

	hr = pIAudioPIDMap->MapStreamId(AudPID_,MPEG2_PROGRAM_ELEMENTARY_STREAM,0,0);
	if(FAILED(hr))return hr;
	return hr;
}

HRESULT CSampleCGB::ConfigureMPEG2Demux(IBaseFilter *pFilter)
{
	if(!pFilter)return E_INVALIDARG;

	//
	// Create video pin and render it
	//
	SmartPtr<IMpeg2Demultiplexer> pIMpeg2Demux;
	HRESULT hr = pFilter->QueryInterface(IID_IMpeg2Demultiplexer,(void**)&pIMpeg2Demux);
	if(FAILED(hr))return hr;
	hr = CreateVideoPin(pIMpeg2Demux);
	if(FAILED(hr))return hr;
	hr = CreateAudioPin(pIMpeg2Demux);
	if(FAILED(hr))return hr;
	return S_OK;
}

HRESULT CSampleCGB::FindMPEG2Pin( IBaseFilter *pFilter, IPin** ppPin )
{
	if(!pFilter)return E_INVALIDARG;

	SmartPtr<IEnumPins> pEnumPins;
	HRESULT hr = pFilter->EnumPins(&pEnumPins);
	if(FAILED(hr))return hr;
	SmartPtr<IPin> pTempPin;
	ULONG fetched;
	PIN_DIRECTION dir;

	hr = pEnumPins->Reset();

	while(pTempPin.Release(),S_OK == pEnumPins->Next(1,&pTempPin,&fetched))
	{
		hr = pTempPin->QueryDirection(&dir);
		if(FAILED(hr) || PINDIR_INPUT == dir)
		{
			continue;
		}
		if(IsMPEG2Pin(pTempPin))
		{
			(*ppPin) = pTempPin.Detach();
			return S_OK;
		}
	}
	return E_FAIL;
}

//
//  A device can stream directly MPEG2 stream,
//  or it can be linked with a video codec and multiplexer
//
//

//
//  Loop through every media type supported by this pin
//  to see if there is one which can be considered MPEG2
//

BOOL CSampleCGB::IsMPEG2Pin( IPin *pPin )
{
	if(!pPin)return FALSE;
	SmartPtr<IEnumMediaTypes> pMediaTypes;
	HRESULT hr = pPin->EnumMediaTypes(&pMediaTypes);
	if(FAILED(hr))return FALSE;
	hr = pMediaTypes->Reset();
	if(FAILED(hr))return FALSE;


	ULONG fetched;
	AM_MEDIA_TYPE* pTempMediaType;

	while(S_OK == pMediaTypes->Next(1,&pTempMediaType,&fetched))
	{
		if(
			(
			::IsEqualGUID(pTempMediaType->majortype,MEDIATYPE_Video) ||
			::IsEqualGUID(pTempMediaType->majortype,MEDIATYPE_Stream)
			)
			&&
			(
			::IsEqualGUID(pTempMediaType->subtype,MEDIASUBTYPE_MPEG2_VIDEO) ||
			::IsEqualGUID(pTempMediaType->subtype,MEDIASUBTYPE_MPEG2_PROGRAM)
			)
			)
		{
			DeleteMediaType(pTempMediaType);
			return TRUE;
		}
		DeleteMediaType(pTempMediaType);
	}
	return FALSE;
}

HRESULT CSampleCGB::FindPin(IBaseFilter *pFilter, 
							const REGPINMEDIUM& regPinMedium,
							PIN_DIRECTION direction,
							BOOL video, 
							IPin **ppPin)
{
	if(!pFilter)return E_POINTER;

	SmartPtr<IEnumPins> pEnumPins;
	HRESULT hr = pFilter->EnumPins(&pEnumPins);
	if(FAILED(hr))return hr;

	SmartPtr<IPin> pTempPin;
	ULONG cFetched;
	REGPINMEDIUM regPinMediumTemp;
	PIN_DIRECTION dir;

	hr = pEnumPins->Reset();
	while(pTempPin.Release(),S_OK == pEnumPins->Next(1,&pTempPin,&cFetched))
	{
		ASSERT(pTempPin);

		hr = pTempPin->QueryDirection(&dir);
		if(FAILED(hr) || dir != direction)
		{
			continue;
		}

		hr = GetMedium(pTempPin,regPinMediumTemp);
		if(FAILED(hr))
		{
			continue;
		}

		if(!IsVideoPin(pTempPin))
		{
			continue;
		}

		if(::IsEqualGUID(regPinMediumTemp.clsMedium,regPinMedium.clsMedium) &&
			(regPinMediumTemp.dw1 == regPinMedium.dw1)
			)
		{
			(*ppPin) = pTempPin.Detach();
			return S_OK;
		}
	}
	return E_FAIL;
}

//获取pin支持的媒体类型

HRESULT CSampleCGB::GetMedium( IPin *pPin, REGPINMEDIUM& regPinMedium )
{
	if(!pPin)return E_POINTER;

	SmartPtr<IKsPin> pKsPin;
	HRESULT hr = pPin->QueryInterface(IID_IKsPin,(void**)&pKsPin);
	if(FAILED(hr))return hr;

	PKSMULTIPLE_ITEM pmi;
	hr = pKsPin->KsQueryMediums(&pmi);
	if(FAILED(hr))return hr;

	REGPINMEDIUM* pMedium = (REGPINMEDIUM*) (pmi+1);
	for(ULONG i=0; i<pmi->Count; i++)
	{
		if( 
			!::IsEqualGUID(pMedium->clsMedium,GUID_NULL) &&
			!::IsEqualGUID(pMedium->clsMedium,KSMEDIUMSETID_Standard)
			)
		{
			regPinMedium.clsMedium = pMedium->clsMedium;
			regPinMedium.dw1 = pMedium->dw1;
			regPinMedium.dw2 = pMedium->dw2;
			CoTaskMemFree(pmi);
			return S_OK;
		}
	}

	regPinMedium.clsMedium = GUID_NULL;
	regPinMedium.dw1 = 0;
	regPinMedium.dw2 = 0;
	return S_OK;
}

BOOL CSampleCGB::IsVideoPin( IPin *pPin )
{
	if(!pPin)return FALSE;
	return HasMediaType(pPin,MEDIATYPE_Video);
}

BOOL CSampleCGB::IsAudioPin( IPin *pPin )
{
	if(!pPin)return FALSE;
	return HasMediaType(pPin,MEDIATYPE_Audio);
}

BOOL CSampleCGB::HasMediaType(IPin *pPin, REFGUID majorType )
{
	if(!pPin)
	{
		return FALSE;
	}

	SmartPtr<IEnumMediaTypes> pMediaTypes;
	HRESULT hr = pPin->EnumMediaTypes(&pMediaTypes);
	if(FAILED(hr))return FALSE;

	hr = pMediaTypes->Reset();
	if(FAILED(hr))return FALSE;

	ULONG cFetched;
	AM_MEDIA_TYPE* pTempMediaType;
	while(S_OK == pMediaTypes->Next(1,&pTempMediaType,&cFetched))
	{
		if(::IsEqualGUID(pTempMediaType->majortype,majorType))
		{
			DeleteMediaType(pTempMediaType);
			return TRUE;
		}
		DeleteMediaType(pTempMediaType);
	}
	return FALSE;
}

HRESULT CSampleCGB::AddMPEG2Demux()
{
	if(pMPEG2Demux_)
	{
		return S_OK;
	}

	HRESULT hr = CoCreateInstance(CLSID_MPEG2Demultiplexer,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void**)&pMPEG2Demux_);
	if(FAILED(hr))
	{
		return hr;
	}

	return graph_->AddFilter(static_cast<IBaseFilter*>(pMPEG2Demux_),L"MPEG2 Demux");
}

HRESULT CSampleCGB::FindEncoder( IEnumMoniker *pEncoders, REGPINMEDIUM pinMedium, IBaseFilter **ppEncoder )
{
	if( ! pEncoders )
	{
		return E_INVALIDARG;
	}

	if(IsEqualGUID(pinMedium.clsMedium,GUID_NULL) || IsEqualGUID(pinMedium.clsMedium,KSMEDIUMSETID_Standard))
	{
		return E_INVALIDARG;
	}

	HRESULT hr;
	SmartPtr<IBaseFilter> pFilter;
	SmartPtr<IMoniker>   pMoniker;
	ULONG cFetched;
	SmartPtr<IPin> pPin;

	while(pFilter.Release(),pMoniker.Release(),S_OK == pEncoders->Next(1,&pMoniker,&cFetched))
	{
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter,(void**)&pFilter);
		if(FAILED(hr))continue;

		hr = FindPin(pFilter,pinMedium,PINDIR_INPUT,TRUE,&pPin);
		if(SUCCEEDED(hr))
		{
			*ppEncoder = pFilter.Detach();
			return hr;
		}
	}
	return E_FAIL;
}

HRESULT CSampleCGB::FindVideoPin( IBaseFilter *pFilter, IPin **ppPin )
{
	if(!pFilter)return E_POINTER;

	SmartPtr<IEnumPins> pEnumPins;
	HRESULT hr = pFilter->EnumPins(&pEnumPins);
	if(FAILED(hr))return hr;

	SmartPtr<IPin> pTempPin;
	ULONG cFetched;
	hr = pEnumPins->Reset();
	if(FAILED(hr))return hr;

	while(pTempPin.Release(),S_OK == pEnumPins->Next(1,&pTempPin,&cFetched))
	{
		if(IsVideoPin(pTempPin))
		{
			*ppPin = pTempPin.Detach();
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CSampleCGB::FindAudioPin( IBaseFilter *pFilter, IPin **ppPin )
{
	if(!pFilter)return E_POINTER;

	SmartPtr<IEnumPins> pEnumPins;
	HRESULT hr = pFilter->EnumPins(&pEnumPins);
	if(FAILED(hr))return hr;

	SmartPtr<IPin> pTempPin;
	ULONG cFetched;
	hr = pEnumPins->Reset();
	if(FAILED(hr))return hr;

	while(pTempPin.Release(),pEnumPins->Next(1,&pTempPin,&cFetched))
	{
		if(IsAudioPin(pTempPin))
		{
			*ppPin = pTempPin.Detach();
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CSampleCGB::BuildMPEG2Segment( IBaseFilter *pFilter )
{
	if(!pFilter)return E_POINTER;

	HRESULT hr = AddMPEG2Demux();
	if(FAILED(hr))return hr;

	//在filter上查找MPEG2 pin
	SmartPtr<IPin> pPin;
	hr = FindMPEG2Pin(pFilter,&pPin);
	if(SUCCEEDED(hr))
	{
		hr = ConnectPin(pPin,pMPEG2Demux_);
		if(FAILED(hr))
		{
			graph_->RemoveFilter(pMPEG2Demux_);
			return E_FAIL;
		}
		return S_OK;
	}
	//
	//  no pins that streams directly MPEG2 stream
	//
	hr = FindVideoPin(pFilter,&pPin);
	if(FAILED(hr))
	{
		graph_->RemoveFilter(pMPEG2Demux_);
		return hr;// no video pin
	}


	return hr;
}

HRESULT CSampleCGB::ConnectPin( IPin *pPin, IBaseFilter *pFilter )
{
	if(!pPin || !pFilter)return E_INVALIDARG;

	PIN_DIRECTION pinDirection;
	HRESULT hr = pPin->QueryDirection(&pinDirection);
	if(FAILED(hr) || PINDIR_INPUT == pinDirection)return E_FAIL;

	//Add the filter to the graph
	BOOL bConnected = FALSE;
	ULONG cFetched = 0;
	SmartPtr<IPin> pDownFilterPin;

	//
	//  Loop through every input pin from downstream filter
	//  and try to connect the pin
	//  

	SmartPtr<IEnumPins> pEnumDownFiltersPins;
	hr = pFilter->EnumPins(&pEnumDownFiltersPins);
	if(FAILED(hr))return hr;

	while(pDownFilterPin.Release(),pEnumDownFiltersPins->Next(1,&pDownFilterPin,&cFetched))
	{
		hr = pDownFilterPin->QueryDirection(&pinDirection);
		if(FAILED(hr))
		{
			continue;
		}

		if(pinDirection == PINDIR_OUTPUT)
		{
			continue;
		}

		hr = graph_->ConnectDirect(pPin,pDownFilterPin,NULL);
		if(SUCCEEDED(hr))
		{
			bConnected = TRUE;
			break;
		}
	}

	if(!bConnected)
	{
		graph_->RemoveFilter(pFilter);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CSampleCGB::ConnectFilters(IBaseFilter *pUpFilter,IBaseFilter *pDownFilter)
{
	if(!pUpFilter || !pDownFilter)return E_INVALIDARG;

	// All the need pin & pin enumerator pointers
	SmartPtr<IEnumPins> pEnumUpFilterPins,pEnumDownFilterPins;

	SmartPtr<IPin> pUpFilterPin, pDownFilterPin;

	HRESULT hr = S_OK;
	// Get the pin enumerators for both the filtera
	hr = pUpFilter->EnumPins(&pEnumUpFilterPins);
	if(FAILED(hr))return hr;

	hr = pDownFilter->EnumPins(&pEnumDownFilterPins);
	if(FAILED(hr))return hr;

	// Loop on every pin on the Upstream Filter

	BOOL bConnected = FALSE;
	PIN_DIRECTION pinDir;
	ULONG cFetched = 0;
	while(pUpFilterPin.Release(),S_OK == pEnumUpFilterPins->Next(1,&pUpFilterPin,&cFetched))
	{
		// Make sure that we have the output pin of the upstream filter
		hr = pUpFilterPin->QueryDirection(&pinDir);
		if(FAILED(hr) || pinDir != PINDIR_OUTPUT)
		{
			continue;
		}

		//
		// I have an output pin; loop on every pin on the Downstream Filter
		//
		while(pDownFilterPin.Release(),S_OK == pEnumDownFilterPins->Next(1,&pDownFilterPin,&cFetched))
		{
			//Make sure that we have the input pin of the downstream filter
			hr = pDownFilterPin->QueryDirection(&pinDir);
			if(FAILED(hr) || pinDir != PINDIR_INPUT)
			{
				continue;
			}
			// Try to connect them and exit if u can else loop more until you can
			if(SUCCEEDED(graph_->ConnectDirect(pUpFilterPin,pDownFilterPin,NULL)))
			{
				bConnected = TRUE;
				break;
			}
		}
		hr = pEnumDownFilterPins->Reset();
		if(FAILED(hr))
		{
			return hr;
		}
	}

	if(!bConnected)
		return E_FAIL;
	return S_OK;
}

HRESULT CSampleCGB::RenderToMPEG2Demux( IPin *pPin )
{
	if(!pPin)
	{
		return E_INVALIDARG;
	}

	REGPINMEDIUM pinMedium;//pin所支持的媒体类型如果媒体类型的CLSID为GUID_NULL或者KSMEDIUMSETID_Standard，那么pin就没法连接。
	HRESULT hr = GetMedium(pPin,pinMedium);
	if(FAILED(hr))return hr;

	SmartPtr<IEnumMoniker> pEncoders;
	if(::IsEqualGUID(pinMedium.clsMedium,GUID_NULL))
	{
		hr = GetEncodersByCategory(&pEncoders);
		if(FAILED(hr))return hr;

		hr = RenderToMPEG2Demux( pPin, pEncoders );
		if( SUCCEEDED( hr ) )
		{
			return S_OK;
		}
	}
	else
	{
		//
		//  search through encoders category; identify
		//  the encoder using the medium
		//
		hr = GetEncodersByCategory( &pEncoders );
		if( FAILED( hr ) )
		{
			return hr;
		}

		hr = RenderToMPEG2Demux( pPin, pinMedium, pEncoders  );
		if( SUCCEEDED( hr ) )
		{
			return S_OK;
		}

		pEncoders = NULL;
		hr = GetEncodersByEnumerating( pinMedium, &pEncoders );
		if( FAILED( hr ) )
		{
			return hr;
		}

		hr = RenderToMPEG2Demux( pPin, pinMedium, pEncoders );
		if( FAILED( hr ) )
		{
			return hr;
		}
	}
	return S_OK;
}

//创建一个解码器设备类型枚举器

HRESULT CSampleCGB::GetEncodersByCategory( IEnumMoniker **ppEncoders )
{
	SmartPtr<ICreateDevEnum> pDeviceEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,IID_ICreateDevEnum,(void**)&pDeviceEnum);
	if(FAILED(hr))return hr;
	return pDeviceEnum->CreateClassEnumerator(KSCATEGORY_ENCODER,ppEncoders,0);
}

HRESULT CSampleCGB::GetEncodersByEnumerating( const REGPINMEDIUM& pinMedium, IEnumMoniker **ppEncoders )
{
	SmartPtr<IFilterMapper2> pFilterMapper2;

	HRESULT hr = CoCreateInstance(CLSID_FilterMapper2,NULL,CLSCTX_INPROC_SERVER,IID_IFilterMapper2,(void**)&pFilterMapper2);
	if(FAILED(hr))return hr;

	hr = pFilterMapper2->EnumMatchingFilters(
		ppEncoders,
		0,
		FALSE,
		0,
		TRUE,
		0,
		NULL,
		&pinMedium,
		NULL,
		FALSE,
		TRUE,
		0,
		NULL,
		NULL,
		NULL
		);
	return hr;
}


//创建多路复用器枚举

HRESULT CSampleCGB::GetMultiplexersByCategory(IEnumMoniker **ppMultiplexers)
{
	SmartPtr<ICreateDevEnum> pDeviceEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,IID_ICreateDevEnum,(void**)&pDeviceEnum);
	if(FAILED(hr))return hr;
	return pDeviceEnum->CreateClassEnumerator(KSCATEGORY_MULTIPLEXER,ppMultiplexers,0);
}

HRESULT CSampleCGB::GetMultiplexersByFilterMapper(IEnumMoniker **ppMultiplexers, 
												  const REGPINMEDIUM& pinMedium)
{
	// 	SmartPtr<IFilterMapper2> pFilterMapper2;
	// 	HRESULT hr = CoCreateInstance(CLSID_FilterMapper2,NULL,CLSCTX_INPROC_SERVER,IID_IFilterMapper2,(void**)&pFilterMapper2);
	// 	if(FAILED(hr))return hr;
	// 	hr = pFilterMapper2->EnumMatchingFilters(ppMultiplexers,)
	return E_NOTIMPL;
}

HRESULT CSampleCGB::RenderToMPEG2Demux(IPin *pPin, IEnumMoniker *pEncoders)
{
	if(!pPin || !pEncoders)
	{
		return E_INVALIDARG;
	}

	REGPINMEDIUM pinMedium;
	pinMedium.clsMedium = GUID_NULL;
	pinMedium.dw1 = 0;
	pinMedium.dw2 = 0;

	SmartPtr<IBaseFilter> pFilter;
	SmartPtr<IMoniker> pMoniker;
	ULONG cFetched;
	HRESULT hr;

	while(pFilter.Release(),pMoniker.Release(),pEncoders->Next(1,&pMoniker,&cFetched))
	{
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter,(void**)(&pFilter));
		if(FAILED(hr))
		{
			continue;
		}
		hr = graph_->AddFilter(pFilter,L"Encoder");
		if(FAILED(hr))
		{
			continue;
		}

		hr = ConnectPin(pPin,pFilter);
		if(FAILED(hr))
		{
			graph_->RemoveFilter(pFilter);
			continue;
		}

		hr = ConnectEncoderToMPEG2Demux( pFilter, pinMedium );
		if( SUCCEEDED( hr ) )
		{
			pEncoder_ = pFilter;
			return S_OK;
		}
		graph_->RemoveFilter( pFilter );
	}
	return E_FAIL;
}

//
//  Loop through every encoder available on system.
//  Look first after the one that has a special medium if
//  there is a special one on pPin.
//  Otherwise, try to render using ICaptureGrapBuilder2
//  and the encoder that matches this will be the chosen one/
//  If the encoder is found, then this will be rendered to the 
//  MPEG2 demux.
//

HRESULT CSampleCGB::RenderToMPEG2Demux( IPin *pPin, const REGPINMEDIUM& pinMedium, IEnumMoniker *pEncoders )
{
	//
	//  The pin has a special medium, 
	//  there shold be an encoder with the same 
	//  medium
	//
	SmartPtr<IBaseFilter> pFilterEncoder;
	HRESULT hr = FindEncoder(pEncoders,pinMedium,&pFilterEncoder);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = graph_->AddFilter(pFilterEncoder,L"Encoder");
	if(FAILED(hr))
	{
		return hr;
	}

	hr = ConnectPin(pPin,pFilterEncoder);
	if( FAILED(  hr ) )
	{
		hr = graph_->RemoveFilter( pFilterEncoder );
		return hr;
	}

	//
	//  the video pin was rendered to the same 
	//  ( hardware? ) encoder with the same mediu
	//
	hr = ConnectEncoderToMPEG2Demux(pFilterEncoder,pinMedium);
	if( FAILED(  hr ) )
	{
		hr = graph_->RemoveFilter( pFilterEncoder );
		return hr;
	}
	pEncoder_ = pFilterEncoder;
	return S_OK;
}


HRESULT CSampleCGB::ConnectEncoderToMPEG2Demux( IBaseFilter *pEncoder, const REGPINMEDIUM& pinMedium )
{
	REGPINMEDIUM regPinMedium;
	regPinMedium.clsMedium = GUID_NULL;
	regPinMedium.dw1 = 0;
	regPinMedium.dw2 = 0;

	//
	//  try a direct connection between 
	//  codec and MPEG2Demux
	//

	HRESULT hr = ConnectFilters(pEncoder,pMPEG2Demux_);
	if(SUCCEEDED(hr))
	{
		return S_OK;
	}

	//
	//  no luck
	//  maybe I need a multiplexer 
	//
	SmartPtr<IEnumMoniker> pEnumMultiplexers;
	hr = GetMultiplexersByCategory(&pEnumMultiplexers);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = ConnectMultiplexerToMPEG2Demux(pEncoder,pEnumMultiplexers);
	if(SUCCEEDED(hr))
	{
		return S_OK;
	}

	if(FALSE == ::IsEqualGUID(pinMedium.clsMedium,GUID_NULL))
	{
		//
		//  get the multiplexers using IFilterMapper2
		//  assuming that the encoder and the multiplexer have the same medium
		//
		pEnumMultiplexers = NULL;
		hr = GetMultiplexersByFilterMapper( &pEnumMultiplexers, pinMedium );
		if( FAILED( hr ) )
		{
			return hr;
		}
		hr = ConnectMultiplexerToMPEG2Demux( pEncoder, pEnumMultiplexers );
		if( SUCCEEDED( hr ) )
		{
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CSampleCGB::ConnectMultiplexerToMPEG2Demux( IBaseFilter *pEncoder, IEnumMoniker *pMultiplexers )
{
	if(!pEncoder || !pMultiplexers)
	{
		return E_INVALIDARG;
	}

	SmartPtr<IBaseFilter> pFilter;
	SmartPtr<IMoniker> pMoniker;
	ULONG cFetched;
	HRESULT hr;

	while(pFilter.Release(),pMoniker.Release(),S_OK == pMultiplexers->Next(1,&pMoniker,&cFetched))
	{
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter,(void**)(&pFilter));
		if(FAILED(hr))
		{
			continue;
		}

		hr = graph_->AddFilter(pFilter,NULL);
		if(FAILED(hr))
		{
			continue;
		}

		//
		//  connect the encoder to the multiplexer
		//
		hr = ConnectFilters( pEncoder, pFilter );
		if( FAILED( hr ) )
		{
			graph_->RemoveFilter( pFilter );
			continue;
		}

		//
		//  connect the multiplexer to the encoder
		//
		hr = ConnectFilters( pFilter, pMPEG2Demux_ );
		if( SUCCEEDED( hr ) )
		{
			pMultiplexer_ = pFilter;
			return S_OK;
		}
	}
	return E_FAIL;
}