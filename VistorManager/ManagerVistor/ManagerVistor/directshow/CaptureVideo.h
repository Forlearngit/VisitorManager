#pragma once
#include "crossbar.h"
#include "smartptr.h"
#include "SampleCGB.h"
using namespace std;


//#define ABS(x) (((x) > 0) ? (x) : -(x))

#ifdef  _DEBUG
#define REGISTER_FILTERGRAPH
#endif

#define WM_FGNOTIFY WM_USER+100




class CProgress : public IAMCopyCaptureFileProgress
{
public:

	CProgress()
	{
	};
	~CProgress()
	{
	};

	STDMETHODIMP_(ULONG) AddRef()
	{
		return 1;
	};
	STDMETHODIMP_(ULONG) Release()
	{
		return 0;
	};

	STDMETHODIMP QueryInterface(REFIID iid, void **p)
	{
		if (p == NULL)
		{
			return E_POINTER;
		}
		if (iid == IID_IUnknown)
		{
			*p = reinterpret_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		else if (iid == IID_IAMCopyCaptureFileProgress)
		{
			*p = reinterpret_cast<IAMCopyCaptureFileProgress*>(this);
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	};
	STDMETHODIMP Progress(int i)
	{
		TCHAR tach[80];
		HRESULT hr = StringCchPrintf(tach, 80, TEXT("Save File Progress: %d%%\0"), i);
		return S_OK;
	};
};

class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
	CSampleGrabberCB() : m_pBuffer(NULL){};
	~CSampleGrabberCB(){};

	STDMETHODIMP_(ULONG) AddRef ()
	{
		return 1;
	}

	STDMETHODIMP_(ULONG) Release ()
	{
		return 2;
	}
	
	STDMETHODIMP QueryInterface(REFIID iid, void **p)
	{
		if (p == NULL)
		{
			return E_POINTER;
		}
		if (iid == IID_IUnknown)
		{
			*p = reinterpret_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		else if (iid == IID_ISampleGrabberCB)
		{
			*p = reinterpret_cast<ISampleGrabberCB*>(this);
			AddRef();
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	};

	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )

	{
		HRESULT hr = S_OK;
		double time = dblSampleTime;
		m_lSize = lBufferSize; 
		m_pBuffer = pBuffer;
		return hr;
//		return E_NOTIMPL;
	}

	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
	{
		//Callback method that receives a pointer to the media sample.
// 		HRESULT hr = S_OK;
// 		BYTE *pBuffer;
// 		hr = pSample->GetPointer(&pBuffer);
// 		char* temp = (char*)pBuffer;
// 		string str(temp);
// 		long size = pSample->GetSize();
// 		return hr;
		return E_NOTIMPL;
	}
	void GetBuffer(long* size,unsigned char** pBuffer){
		*size = m_lSize;
		*pBuffer = m_pBuffer;
	}
private:
	unsigned char* m_pBuffer;
	long m_lSize;
};


class CCaptureVideo
{
public:
	CCaptureVideo(void);
	~CCaptureVideo(void);
	void EnumDevice(void);//枚举系统设备
	void ChooseDevices(IMoniker* pmVideo,IMoniker* pmAudio);
	void ChooseDevices(TCHAR* szVideo,TCHAR* szAudio);//根据系统设备friendname选择设备得到IMoniker接口
//	void ChooseDevices(void);

	BOOL InitCapFilters(void);//初始化
	

	BOOL BuildCaptureGraph(void);//创建捕获graph
	BOOL BuildPreviewGraph(void);//创建预览graph

	//当前选择的视频音频名称
	wstring GetCurrentVideoName(void);
	wstring GetCurrentAudioName(void);
	void SetCurrentAudioName(const WCHAR* szAudioFriendName);
	void SetCurrentVideoName(const WCHAR* szVideoFriendName);
	//是否捕获音频
	BOOL GetIsCaptureAudio(){return m_fCapAudio;}
	void SetIsCaptureAudio(BOOL f){m_fCapAudio = f;}
	//获取是否正在捕获或者预览
	BOOL GetIsPerviewing(){return m_fPreviewing;}
	BOOL GetIsCaptureing(){return m_fCapturing;}

	//捕获事件处理
	HRESULT GetCaptureEvent(/* [out] */ __RPC__out long *lEventCode,
		/* [out] */ __RPC__out LONG_PTR *lParam1,
		/* [out] */ __RPC__out LONG_PTR *lParam2,
		/* [in] */ long msTimeout );
	HRESULT FreeCaptureEventParams(/* [in] */ long lEvCode,
		/* [in] */ LONG_PTR lParam1,
		/* [in] */ LONG_PTR lParam2 );
public:
	BOOL StartPerview(void);
	BOOL StartCapture(void);

	BOOL StopPerview(void);
	BOOL StopCapture(void);
	/////////////////////////////////////
	BOOL SetCaptureFile(WCHAR* szFileName);
	BOOL SaveCaptureFile(WCHAR* szFileNameSoure,WCHAR* wszFileNameDest);
	BOOL AllocCaptureFile(DWORD dSize,WCHAR* szFileName);
	///////////////////////////////////
	BOOL SetWindowStyle(long nStyle);
	BOOL SetWindowRect(const RECT rect);
	BOOL PutOwner(HWND hWnd);
	BOOL SetWindowVisble(BOOL flag);
	BOOL SetWindowPosition(long left,long top,long right,long bottom);
	BOOL GetWindowPosition(long* left,long* top,long* width,long* height);
	//////////////////////////////////
	BOOL SetWindowEvent(HWND hWnd);
	//////////////////////////////////
	BOOL SetCapturFileName(const WCHAR* szVideoFileName);
	///Video Render使用了DirectDraw加速，因此会失败
//	BOOL TakePhoto(const WCHAR* szFilePath);
	//采用VMR
	BOOL TakePhoto(const wstring& szFileName);
	//建立samplegrabberfilter
	BOOL BuildSampleGrabber(void);
private:
	inline void IMonRelease(IMoniker *&pm);//释放IMoniker接口
	void FreeCapFilters(void);//释放资源
	BOOL MakeBulider(void);//创建ICaptureGraphBuilder2接口
	BOOL MakeGraph(void);//创建IGraphBuilder接口
	BOOL GetNeedInterface(void);//通过ICaptureGraphBuilder2得到需要的各种接口
	BOOL UseCrossbar(void);//使用crossbar
	BOOL InitAudio(void);//初始化音频filter并添加到graph
	void TearDownGraph(void);//释放掉创建捕获、预览所产生的接口
	void RemoveDownstream(IBaseFilter *pf);//递归删除filter，并断开连接
	
	void GetAcaptRGB(AM_MEDIA_TYPE* mt);//根据当前显示器配置，获取可接受的RGB
	
public:
	IMoniker* m_prgpmVideo[10];//指针数组系统枚举视频设备
	IMoniker* m_prgpmAudio[10];//指针数组系统枚举音频设备
	WCHAR* m_wszVideoDviceName[10];//指针数组存贮系统枚举视频设备名字
	WCHAR* m_wszAudioDviceName[10];//指针数组存贮系统枚举音频设备名字


private:
	SmartPtr<IMoniker> m_pImonikerVideo;//videoImoniker
	SmartPtr<IMoniker> m_pImonikerAudio;//audioImoniker

	CCrossbar *m_pCrossbar;//输入端子crossbar的选择

	CSampleCGB* m_pSampleGrapBuilder;//捕获grapbuilder

	SmartPtr<IBaseFilter> m_pVCap;//videofilter
	SmartPtr<IBaseFilter> m_pACap;//audiofilter

	SmartPtr<IGraphBuilder> m_pGraph;//graphbuilder

	WCHAR m_szCurrentSelectVideoFriendName[120];//当前选择video设备friendName
	WCHAR m_szCurrentSelectAudioFriendName[120];//当前选择的Audio friendName
	
	SmartPtr<IAMVideoCompression> m_pVC;//视频压缩接口
	SmartPtr<IAMStreamConfig> m_pASC;//for audio设置视屏或音频格式，属性的接口
	SmartPtr<IAMStreamConfig> m_pVSC;//for video
	SmartPtr<IAMVfwCaptureDialogs> m_pDlg;//视频属性设置接口(仅对VFW设备生效)
	///采用vfw的老设备自带三个属性设置对话框。（属性页，通过此接口获取)


	LONG m_NumberOfVideoInputs;


private:
	BOOL m_fCapAudio;//是否捕获声音

	BOOL m_fCaptureGraphBuilt;//是否创建了捕获grap
	BOOL m_fPreviewGraphBuilt;//是否创建预览了预览grap

	BOOL m_fCapturing;//是否正在捕获
	BOOL m_fPreviewing;//是否正在预览
	BOOL m_fPreviewFaked;//有的设备不支持preview


private:
	SmartPtr<IFileSinkFilter> m_pSink;//将捕获的视频写入文件的接口

	SmartPtr<IConfigAviMux> m_pConfigAviMux;//该IConfigAviMux接口配置AVI复用的过滤器。应用程序可以使用这个接口来设置主流，并创建一个AVI1.0指数

	SmartPtr<IBaseFilter> m_pRender;

	SmartPtr<IVideoWindow> m_pVW;//设置视频显示窗口接口
	SmartPtr<IMediaEventEx> m_pME;//接收media事件接口

	SmartPtr<IAMDroppedFrames> m_pDF;//得到视频拍摄时的性能，如包括有多少帧被撤销，有多少传输。应用程序可以使用这个接口来确定拍摄性能在运行时。


private:
	DWORD m_dwGraphRegister;//debug版本时注册到graphedit便于调试

	WCHAR m_wszCaptureFileName[MAX_PATH];//捕获视频储存的文件路径

	int m_iMasterStream;//设置主stream

	BOOL m_fWantPreview;//创建捕获时，是否预览
	BOOL m_fUseFrameRate;//是否使用捕获帧率

	double m_dFrameRate;//捕获视频的帧率

	SmartPtr<IBaseFilter> m_pGrabberFilter;//samplegrabberfilter
	SmartPtr<ISampleGrabber> m_pSampleGrabber;

	CSampleGrabberCB* m_pCB;//ISampleGrabberCB回调接口类
};
