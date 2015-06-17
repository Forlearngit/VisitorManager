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
	void EnumDevice(void);//ö��ϵͳ�豸
	void ChooseDevices(IMoniker* pmVideo,IMoniker* pmAudio);
	void ChooseDevices(TCHAR* szVideo,TCHAR* szAudio);//����ϵͳ�豸friendnameѡ���豸�õ�IMoniker�ӿ�
//	void ChooseDevices(void);

	BOOL InitCapFilters(void);//��ʼ��
	

	BOOL BuildCaptureGraph(void);//��������graph
	BOOL BuildPreviewGraph(void);//����Ԥ��graph

	//��ǰѡ�����Ƶ��Ƶ����
	wstring GetCurrentVideoName(void);
	wstring GetCurrentAudioName(void);
	void SetCurrentAudioName(const WCHAR* szAudioFriendName);
	void SetCurrentVideoName(const WCHAR* szVideoFriendName);
	//�Ƿ񲶻���Ƶ
	BOOL GetIsCaptureAudio(){return m_fCapAudio;}
	void SetIsCaptureAudio(BOOL f){m_fCapAudio = f;}
	//��ȡ�Ƿ����ڲ������Ԥ��
	BOOL GetIsPerviewing(){return m_fPreviewing;}
	BOOL GetIsCaptureing(){return m_fCapturing;}

	//�����¼�����
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
	///Video Renderʹ����DirectDraw���٣���˻�ʧ��
//	BOOL TakePhoto(const WCHAR* szFilePath);
	//����VMR
	BOOL TakePhoto(const wstring& szFileName);
	//����samplegrabberfilter
	BOOL BuildSampleGrabber(void);
private:
	inline void IMonRelease(IMoniker *&pm);//�ͷ�IMoniker�ӿ�
	void FreeCapFilters(void);//�ͷ���Դ
	BOOL MakeBulider(void);//����ICaptureGraphBuilder2�ӿ�
	BOOL MakeGraph(void);//����IGraphBuilder�ӿ�
	BOOL GetNeedInterface(void);//ͨ��ICaptureGraphBuilder2�õ���Ҫ�ĸ��ֽӿ�
	BOOL UseCrossbar(void);//ʹ��crossbar
	BOOL InitAudio(void);//��ʼ����Ƶfilter����ӵ�graph
	void TearDownGraph(void);//�ͷŵ���������Ԥ���������Ľӿ�
	void RemoveDownstream(IBaseFilter *pf);//�ݹ�ɾ��filter�����Ͽ�����
	
	void GetAcaptRGB(AM_MEDIA_TYPE* mt);//���ݵ�ǰ��ʾ�����ã���ȡ�ɽ��ܵ�RGB
	
public:
	IMoniker* m_prgpmVideo[10];//ָ������ϵͳö����Ƶ�豸
	IMoniker* m_prgpmAudio[10];//ָ������ϵͳö����Ƶ�豸
	WCHAR* m_wszVideoDviceName[10];//ָ���������ϵͳö����Ƶ�豸����
	WCHAR* m_wszAudioDviceName[10];//ָ���������ϵͳö����Ƶ�豸����


private:
	SmartPtr<IMoniker> m_pImonikerVideo;//videoImoniker
	SmartPtr<IMoniker> m_pImonikerAudio;//audioImoniker

	CCrossbar *m_pCrossbar;//�������crossbar��ѡ��

	CSampleCGB* m_pSampleGrapBuilder;//����grapbuilder

	SmartPtr<IBaseFilter> m_pVCap;//videofilter
	SmartPtr<IBaseFilter> m_pACap;//audiofilter

	SmartPtr<IGraphBuilder> m_pGraph;//graphbuilder

	WCHAR m_szCurrentSelectVideoFriendName[120];//��ǰѡ��video�豸friendName
	WCHAR m_szCurrentSelectAudioFriendName[120];//��ǰѡ���Audio friendName
	
	SmartPtr<IAMVideoCompression> m_pVC;//��Ƶѹ���ӿ�
	SmartPtr<IAMStreamConfig> m_pASC;//for audio������������Ƶ��ʽ�����ԵĽӿ�
	SmartPtr<IAMStreamConfig> m_pVSC;//for video
	SmartPtr<IAMVfwCaptureDialogs> m_pDlg;//��Ƶ�������ýӿ�(����VFW�豸��Ч)
	///����vfw�����豸�Դ������������öԻ��򡣣�����ҳ��ͨ���˽ӿڻ�ȡ)


	LONG m_NumberOfVideoInputs;


private:
	BOOL m_fCapAudio;//�Ƿ񲶻�����

	BOOL m_fCaptureGraphBuilt;//�Ƿ񴴽��˲���grap
	BOOL m_fPreviewGraphBuilt;//�Ƿ񴴽�Ԥ����Ԥ��grap

	BOOL m_fCapturing;//�Ƿ����ڲ���
	BOOL m_fPreviewing;//�Ƿ�����Ԥ��
	BOOL m_fPreviewFaked;//�е��豸��֧��preview


private:
	SmartPtr<IFileSinkFilter> m_pSink;//���������Ƶд���ļ��Ľӿ�

	SmartPtr<IConfigAviMux> m_pConfigAviMux;//��IConfigAviMux�ӿ�����AVI���õĹ�������Ӧ�ó������ʹ������ӿ�������������������һ��AVI1.0ָ��

	SmartPtr<IBaseFilter> m_pRender;

	SmartPtr<IVideoWindow> m_pVW;//������Ƶ��ʾ���ڽӿ�
	SmartPtr<IMediaEventEx> m_pME;//����media�¼��ӿ�

	SmartPtr<IAMDroppedFrames> m_pDF;//�õ���Ƶ����ʱ�����ܣ�������ж���֡���������ж��ٴ��䡣Ӧ�ó������ʹ������ӿ���ȷ����������������ʱ��


private:
	DWORD m_dwGraphRegister;//debug�汾ʱע�ᵽgraphedit���ڵ���

	WCHAR m_wszCaptureFileName[MAX_PATH];//������Ƶ������ļ�·��

	int m_iMasterStream;//������stream

	BOOL m_fWantPreview;//��������ʱ���Ƿ�Ԥ��
	BOOL m_fUseFrameRate;//�Ƿ�ʹ�ò���֡��

	double m_dFrameRate;//������Ƶ��֡��

	SmartPtr<IBaseFilter> m_pGrabberFilter;//samplegrabberfilter
	SmartPtr<ISampleGrabber> m_pSampleGrabber;

	CSampleGrabberCB* m_pCB;//ISampleGrabberCB�ص��ӿ���
};
