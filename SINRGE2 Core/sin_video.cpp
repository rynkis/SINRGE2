/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** SINRGE2 Class Video
*/
#include "sin_video.h"
#include <comdef.h>

#include <string>
#include <xstring>

#pragma comment (lib,"strmiids.lib")

#define VD_SAFE_RELEASE(ptr)	{if (ptr) { (ptr)->Release(); (ptr) = NULL; }}

/**
 *	加载视频文件（准备播放
 */
bool CVideoMgr::LoadMovie(const wchar_t * pFileName, int &pOutWidth, int &pOutHeight)
{
    HRESULT hr;

	///<	先停止当前视频
    StopMovie();

	///<	创建一个自动释放池
	CAutoReleasePool<IUnknown> cLocalPool;

	///<	创建过滤器图管理器
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if (FAILED(hr))	return false;

    ///<	获取相关过滤器
    hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,		(void**)&m_pMediaControl);
	if (FAILED(hr))	return false;

    hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent,		(void**)&m_pMediaEvent);
	if (FAILED(hr))	return false;

    hr = m_pGraphBuilder->QueryInterface(IID_IMediaSeeking,		(void**)&m_pMediaSeeking);
	if (FAILED(hr)) return false;

	hr = m_pGraphBuilder->QueryInterface(IID_IBasicAudio,		(void**)&m_pBasicAudio);
	if (FAILED(hr)) return false;

    ///<	添加采样过滤器
	IBaseFilter * pTempFilter;
	hr = AddFilter(m_pGraphBuilder, CLSID_SampleGrabber, L"Sample Grabber", &pTempFilter);
	if (FAILED(hr)) return false;
	cLocalPool.Push(pTempFilter);
    hr = pTempFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabber);
	if (FAILED(hr)) return false;

    ///<	设置采样过滤器相关参数
	AM_MEDIA_TYPE mt = { 0, };
	{
		mt.majortype	= MEDIATYPE_Video;
		mt.subtype		= MEDIASUBTYPE_RGB32;		///<	MEDIASUBTYPE_RGB24
		mt.formattype	= FORMAT_VideoInfo;
	}
    hr = m_pSampleGrabber->SetMediaType(&mt);		///<	设置采样类型
	if (FAILED(hr)) return false;
	hr = m_pSampleGrabber->SetOneShot(FALSE);		///<	快照后停止
	if (FAILED(hr)) return false;
	hr = m_pSampleGrabber->SetBufferSamples(TRUE);	///<	缓冲区采样
	if (FAILED(hr)) return false;

	///<	创建源过滤器
	IBaseFilter * pSourceFilter = NULL;
	IFileSourceFilter * pFileSourceFilter = NULL;
	hr = AddFilter(m_pGraphBuilder, CLSID_AsyncReader, L"Sin Source", &pSourceFilter);
	if (FAILED(hr)) return false;
	cLocalPool.Push(pSourceFilter);	
	pSourceFilter->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSourceFilter);
	if (FAILED(hr)) return false;
	cLocalPool.Push(pFileSourceFilter);

	///<	加载视频源
	hr = pFileSourceFilter->Load(pFileName, NULL);
	if (FAILED(hr)) return false;

	///<	构造过滤器图（基本等同于RenderFile）
	IPin * pSourceFilterOutPint = FindPin(pSourceFilter, PINDIR_OUTPUT);
	if (!pSourceFilterOutPint) return false;
	cLocalPool.Push(pSourceFilterOutPint);
	hr = m_pGraphBuilder->Render(pSourceFilterOutPint);
	if (FAILED(hr)) return false;

	///<	放在Graph构建完毕之后（否则在构建图的时候会采用Null Renderer等导致不能播放
	IBaseFilter * pNullRendererFilter = NULL;
	hr = AddFilter(m_pGraphBuilder, CLSID_NullRenderer, L"Null Renderer", &pNullRendererFilter);
	if (FAILED(hr)) return false;
	cLocalPool.Push(pNullRendererFilter);

	///<	替换 Renderer 过滤器为Null Renderer
	hr = ReplaceVideoRenderer(pNullRendererFilter);
	if (FAILED(hr)) return false;

	///<	获取相关参数
    AM_MEDIA_TYPE mtt;
    hr = m_pSampleGrabber->GetConnectedMediaType(&mtt);
	if (FAILED(hr)) return false;
    
    VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*)mtt.pbFormat;
    m_iVideoWidth	= vih->bmiHeader.biWidth;
    m_iVideoHeight	= vih->bmiHeader.biHeight;

	pOutWidth		= vih->bmiHeader.biWidth;
	pOutHeight		= vih->bmiHeader.biHeight;

	m_pTempBuffer	= (BYTE*)malloc(m_iVideoWidth * m_iVideoHeight * 4);
	m_is_occupied = true;

    ///<	释放媒体类型
	AuxFreeMediaType(mtt);

	return true;
}

/**
 *	开始播放视频
 */
void CVideoMgr::PlayMovie(long iVolume)
{
	if (m_pMediaControl)	m_pMediaControl->Run();
	if (m_pBasicAudio)		m_pBasicAudio->put_Volume(iVolume);
}

/**
 *	判断视频是否在播放中
 */
bool CVideoMgr::IsMoviePlaying()
{
	HRESULT hr;

	///<	判断事件
	if (m_pMediaEvent)
	{
		long ev;
		LONG_PTR p1, p2;

		while (SUCCEEDED(m_pMediaEvent->GetEvent(&ev, &p1, &p2, 0)))
		{
			hr = m_pMediaEvent->FreeEventParams(ev, p1, p2);
			if (FAILED(hr))			return false;
			if (ev == EC_COMPLETE)	return false;
		}
	}

	///<	获取状态
	if (m_pMediaControl)
	{
		OAFilterState pfs;
		hr = m_pMediaControl->GetState(0, &pfs);
		if (FAILED(hr))	return false;
		return (pfs == State_Running);
	}

	return false;
}

/**
 *	更新视频贴图
 */
void CVideoMgr::UpdateMovieTexture(DWORD * pDstBitmapAddr)
{
    HRESULT hr;
 
    if (!m_pGraphBuilder || !m_pSampleGrabber)
        return;

    long cbBuffer = 0;
    hr = m_pSampleGrabber->GetCurrentBuffer(&cbBuffer, NULL);
	if (FAILED(hr)) return;
    if (cbBuffer <= 0)
        return;
	
    hr = m_pSampleGrabber->GetCurrentBuffer(&cbBuffer, (long*)m_pTempBuffer);
    if (FAILED(hr)) return;

	BYTE r, g, b;
	int i, h, v;
	for (int ly = 0; ly < m_iVideoHeight; ++ly)
	{
		h = m_iVideoWidth * ly * 4;
		v = m_iVideoWidth * (m_iVideoHeight - ly - 1);
		for (int lx = 0; lx < m_iVideoWidth; ++lx)
		{
			i = h + lx * 4;
			b = m_pTempBuffer[i];
			g = m_pTempBuffer[i + 1];
			r = m_pTempBuffer[i + 2];
			pDstBitmapAddr[v + lx] = 0xFF000000 + (r << 16) + (g << 8) + b;//b | (g << 8) | (r << 16) | (255 << 24);
		}
	}
}

/**
 *	停止视频播放
 */
void CVideoMgr::StopMovie()
{
	if (!m_pGraphBuilder)
		return;

	if (m_pMediaControl)
		m_pMediaControl->Stop();

	VD_SAFE_RELEASE(m_pBasicAudio);
	VD_SAFE_RELEASE(m_pSampleGrabber);
	VD_SAFE_RELEASE(m_pMediaSeeking);
	VD_SAFE_RELEASE(m_pMediaControl);
	VD_SAFE_RELEASE(m_pMediaEvent);
	VD_SAFE_RELEASE(m_pGraphBuilder);

	m_iVideoWidth = 0;
	m_iVideoHeight = 0;
	m_is_occupied = false;

	if (m_pTempBuffer)
		free(m_pTempBuffer);
}

/**
 *	重置视频播放位置
 */
void CVideoMgr::RewindMovie()
{
	if (!m_pMediaSeeking) 
		return;

	LONGLONG iCurrPos = 0;
	LONGLONG iStopPos = 0;

	m_pMediaSeeking->SetPositions(&iCurrPos, AM_SEEKING_AbsolutePositioning, &iStopPos, AM_SEEKING_NoPositioning);
}

/**
 *	调整视频音量
 */
long CVideoMgr::GetVolume() const
{
	if (!m_pBasicAudio)
		return 0;

	long iVolume = 0;
	m_pBasicAudio->get_Volume(&iVolume);
	return iVolume;
}

void CVideoMgr::SetVolume(long iVolume)
{
	if (!m_pBasicAudio)
		return;

	m_pBasicAudio->put_Volume(iVolume);
}

/**************************************************************
 *	以下开始为私有函数
 /************************************************************/

IPin * CVideoMgr::FindPin(IBaseFilter * pFilter, PIN_DIRECTION PinDir, bool bFreePin)
{
	IEnumPins*		pEnumerator;   
	IPin*			pPin;
	IPin*			pLastPin;
	PIN_DIRECTION	tmpDir;

	if (FAILED(pFilter->EnumPins(&pEnumerator)))
		return 0;

	while (S_OK == pEnumerator->Next(1, &pPin, NULL))
	{
		if (SUCCEEDED(pPin->QueryDirection(&tmpDir)) && tmpDir == PinDir)
		{
			if (!bFreePin)
				return pPin;

			HRESULT hr = pPin->ConnectedTo(&pLastPin);
			if (VFW_E_NOT_CONNECTED == hr)
				return pPin;

			pLastPin->Release();
			pPin->Release();
		}
		else
			pPin->Release();
	}

	return 0;
}

void CVideoMgr::CollectFilterPins(IBaseFilter * pFilter, ListPinPtr& vInPins, ListPinPtr& vOutPins)
{
	IPin*			pPin;
	IEnumPins*		pEnumerator;
	PIN_DIRECTION	dir;

	if (FAILED(pFilter->EnumPins(&pEnumerator)))
		return;

	vInPins.clear();
	vOutPins.clear();

	while (S_OK == pEnumerator->Next(1, &pPin, NULL))
	{
		if (SUCCEEDED(pPin->QueryDirection(&dir)))
		{
			if (dir == PINDIR_INPUT)
				vInPins.push_back(pPin);
			else
				vOutPins.push_back(pPin);
		}
		else
			pPin->Release();
	}

	pEnumerator->Release();
}

void CVideoMgr::CollectFilters(IGraphBuilder * pGraph, ListBaseFilterPtr& vFilters)
{
	IEnumFilters*	pEnumerator;
	IBaseFilter*	pFilter;

	if (FAILED(pGraph->EnumFilters(&pEnumerator)))
		return;

	vFilters.clear();

	while (S_OK == pEnumerator->Next(1, &pFilter, NULL))
		vFilters.push_back(pFilter);

	pEnumerator->Release();  
}

HRESULT CVideoMgr::AddFilter(IGraphBuilder * pGraph, const GUID& gClsID, LPCWSTR pName, IBaseFilter ** pOutFilter)
{
	*pOutFilter = 0;

	IBaseFilter * pFilter = 0;
	HRESULT hr = CoCreateInstance(gClsID, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pFilter);

	if (SUCCEEDED(hr))
	{
		hr = pGraph->AddFilter(pFilter, pName);
		if (SUCCEEDED(hr))
			*pOutFilter = pFilter;
		else
			pFilter->Release();
	}

	return hr;
}

void CVideoMgr::ReleasePins(ListPinPtr& vPins)
{
	for (ListPinPtr::iterator it = vPins.begin(); it != vPins.end(); ++it)
		(*it)->Release();
}

HRESULT CVideoMgr::ReplaceVideoRenderer(IBaseFilter * pNullRendererFilter)
{
	HRESULT				hr;

	ListPinPtr			vInPins;
	ListPinPtr			vOutPins;
	ListBaseFilterPtr	vFilters;

	IPin*				pCurrInPin	= NULL;
	IPin*				pLastOutPin	= NULL;
	AM_MEDIA_TYPE		sMediaType;

	///<	枚举所有过滤器
	CollectFilters(m_pGraphBuilder, vFilters);

	///<	遍历所有过滤器（查找最后一个有效的Renderer Filter）
	for (ListBaseFilterPtr::iterator it = vFilters.begin(); it != vFilters.end(); ++it)
	{
		///<	获取过滤器上所有的输入输出Pin
		CollectFilterPins(*it, vInPins, vOutPins);

		///<	过滤不需要的过滤器（Renderer过滤应该只有一个输入&没有输出）
		if (vInPins.size() != 1 || vOutPins.size() != 0)
		{
			///<	释放过滤器所有Pin以及过滤器自身
			ReleasePins(vInPins);
			ReleasePins(vOutPins);
			continue;
		}
	
		///<	获取过滤器输入Pin上的媒体类型
		pCurrInPin = vInPins[0];
		hr = pCurrInPin->ConnectionMediaType(&sMediaType);

		///<	获取媒体类型失败作为未知Pin处理（略过 or 略过非视频类过滤器
		if (FAILED(hr) || sMediaType.majortype != MEDIATYPE_Video)
		{
			///<	释放输入Pin以及过滤器自身
			VD_SAFE_RELEASE(pCurrInPin);
			continue;
		}

		///<	获取输入Pin上连接着的输出Pin
		hr = pCurrInPin->ConnectedTo(&pLastOutPin);
		if (VFW_E_NOT_CONNECTED == hr)
		{
			VD_SAFE_RELEASE(pCurrInPin);
			VD_SAFE_RELEASE(pLastOutPin);
			continue;
		}

		break;
	}

	///<	释放所有过滤器
	for (ListBaseFilterPtr::iterator it = vFilters.begin(); it != vFilters.end(); ++it)
		(*it)->Release();
	vFilters.clear();

	if (!pCurrInPin || !pLastOutPin)
	{
		VD_SAFE_RELEASE(pCurrInPin);
		VD_SAFE_RELEASE(pLastOutPin);
		return E_UNEXPECTED;
	}

	///<	替换Video Renderer为 Null Renderer
	IPin * pNullRendererInPin = FindPin(pNullRendererFilter, PINDIR_INPUT);   
	if (!pNullRendererInPin)
	{
		VD_SAFE_RELEASE(pCurrInPin);
		VD_SAFE_RELEASE(pLastOutPin);
		return E_UNEXPECTED;
	}

	///<	断开输出Pin
	hr = pLastOutPin->Disconnect();
	if (FAILED(hr))
	{
		VD_SAFE_RELEASE(pCurrInPin);
		VD_SAFE_RELEASE(pLastOutPin);
		VD_SAFE_RELEASE(pNullRendererInPin);
		return hr;
	}

	///<	断开输入Pin
	hr = pCurrInPin->Disconnect();
	if (FAILED(hr))
	{
		VD_SAFE_RELEASE(pCurrInPin);
		VD_SAFE_RELEASE(pLastOutPin);
		VD_SAFE_RELEASE(pNullRendererInPin);
		return hr;
	}

	///<	重新连接输出Pin到NullRenderer的输入Pin
	hr = m_pGraphBuilder->Connect(pLastOutPin, pNullRendererInPin); 

	///<	释放
	VD_SAFE_RELEASE(pCurrInPin);
	VD_SAFE_RELEASE(pLastOutPin);
	VD_SAFE_RELEASE(pNullRendererInPin);

	return hr;
}

void CVideoMgr::AuxFreeMediaType(AM_MEDIA_TYPE& mt)
{
	if (mt.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL)
	{
		// Unecessary because pUnk should not be used, but safest.
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}
}

CVideoMgr * CVideoMgr::s_pVideoMgr = 0;

CVideoMgr::CVideoMgr()
	: m_pGraphBuilder(0)

	, m_pMediaControl(0)
	, m_pMediaEvent(0)
	, m_pMediaSeeking(0)

	, m_pSampleGrabber(0)
	, m_pBasicAudio(0)

	, m_iVideoWidth(0)
	, m_iVideoHeight(0)
	, m_pTempBuffer(0)
	, m_is_occupied(false)
{
	s_pVideoMgr = this;
}

CVideoMgr::~CVideoMgr()
{
}

bool CVideoMgr::Init()
{
	if (FAILED(CoInitialize(NULL)))
		return false;

	return true;
}

void CVideoMgr::Destroy()
{
	StopMovie();

	CoUninitialize();
}
