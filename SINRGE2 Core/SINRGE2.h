#ifndef __SINRGE2_H__
#define __SINRGE2_H__
#include "RbRenderTree.h"
#include <Windows.h>
#include "hge.h"
#include "sin_color.h"
#include "sin_types.h"
#include <d3d8.h>

typedef struct __SinFrameStruct
{
	bool			m_fullscreen_start;
	bool			m_forbid_fullscreen;
	bool			m_forbid_switch;

	unsigned long	m_screen_width;
	unsigned long	m_screen_height;

	HWND			m_hwnd;

	__SinFrameStruct()
		: m_fullscreen_start(false), m_forbid_fullscreen(false), m_forbid_switch(false),
			m_screen_width(0), m_screen_height(0), m_hwnd(NULL)
	{
	}

	void Default()
	{
		m_screen_width	= 800;
		m_screen_height = 600;
	}

} SinFrameStruct;

#define SIN_NAME		"SINRGE2"
#define SIN_VERSION		"Ver. YAR"

namespace Sin
{
	SinFrameStruct*		GetFrmStructPtr();

	IDirect3D8*			GetD3DPtr();
	IDirect3DDevice8*	GetD3DDevicePtr();
	D3DCAPS8*			GetD3DCapsPtr();
	static DWORD		GetMaxTexW() { return GetD3DCapsPtr()->MaxTextureWidth; }
	static DWORD		GetMaxTexH() { return GetD3DCapsPtr()->MaxTextureHeight; }
	RbRenderState*		GetRenderState();// const { return m_pRenderState; }
	static unsigned long		GetFrameWidth() { return GetFrmStructPtr()->m_screen_width; }
	static unsigned long		GetFrameHeight() { return GetFrmStructPtr()->m_screen_height; }

	HGE*				GetHgePtr();
	void				CreateHge();
	void				InitRenderSys();
	bool				HackD3D();

	//char*				GetTitle();
	wchar_t*			GetTitleW();
	void				SINRGE2Initialize();
	int					SINRGE2Eval(const char* script);
	int					SINRGE2Entry(const char* rubyfile);
	void				SetRubyLibHandle(HINSTANCE hModule);
}

#endif	//	__SINRGE2_H__