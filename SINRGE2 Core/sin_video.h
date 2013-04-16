#ifndef __C_VIDEO_H__
#define __C_VIDEO_H__

#include <windows.h>
#include <dshow.h>

#include <vector>
#include <map>
#include <string>
#include <xstring>

#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "Qedit.h"

template<class T> class CAutoReleasePool
{
private:
	typedef std::vector<T*>	ListPtr;

public:
	CAutoReleasePool()
	{
		m_vPointers.clear();
	}

	~CAutoReleasePool()
	{
		Release();
	}

	void	Push(T* p)
	{
		m_vPointers.push_back(p);
	}

	void	Release()
	{
		for (ListPtr::iterator it = m_vPointers.begin(); it != m_vPointers.end(); ++it)
			(*it)->Release();

		m_vPointers.clear();
	}

private:
	ListPtr	m_vPointers;
};

class CVideoMgr
{
private:
	typedef std::vector<IPin*>			ListPinPtr;
	typedef std::vector<IBaseFilter*>	ListBaseFilterPtr;
	typedef std::vector<std::wstring>	ListWString;

public:
	bool					LoadMovie(const wchar_t* pFileName, int &pOutWidth, int &pOutHeight);
	void					PlayMovie(/*DWORD* pDstBitmapAddr, */long iVolume);
	bool					IsMoviePlaying();
	void					UpdateMovieTexture(DWORD* pDstBitmapAddr);
	void					StopMovie();
	void					RewindMovie();
	long					GetVolume() const;
	void					SetVolume(long iVolume);
	bool					IsOccupying() const { return m_is_occupying; };

private:
	void					CollectFilterPins(IBaseFilter* pFilter, ListPinPtr& vInPins, ListPinPtr& vOutPins);
	void					CollectFilters(IGraphBuilder* pGraph, ListBaseFilterPtr& vFilters);
	IPin*					FindPin(IBaseFilter* pFilter, PIN_DIRECTION PinDir, bool bFreePin = true);

	HRESULT					AddFilter(IGraphBuilder* pGraph, const GUID& gClsID, LPCWSTR pName, IBaseFilter** pOutFilter);
	void					ReleasePins(ListPinPtr& vPins);
	void					AuxFreeMediaType(AM_MEDIA_TYPE& mt);
	HRESULT					ReplaceVideoRenderer(IBaseFilter* pNullRendererFilter);

public:
	bool					Init();
	void					Destroy();

private:
	IGraphBuilder*			m_pGraphBuilder;
	IMediaControl*			m_pMediaControl;
	IMediaEvent*			m_pMediaEvent;
	IMediaSeeking*			m_pMediaSeeking;
	ISampleGrabber*			m_pSampleGrabber;
	IBasicAudio*			m_pBasicAudio;

	//DWORD*					m_pDstBitmapAddr;
	BYTE*					m_pTempBuffer;
	long					m_iVideoWidth;
	long					m_iVideoHeight;
	bool					m_is_occupying;

public:
	CVideoMgr();
	~CVideoMgr();

private:
	static CVideoMgr*		s_pVideoMgr;
	friend CVideoMgr*		GetVideoMgr();
};

inline CVideoMgr*			GetVideoMgr() { return CVideoMgr::s_pVideoMgr; }

#endif	//	__C_VIDEO_H__