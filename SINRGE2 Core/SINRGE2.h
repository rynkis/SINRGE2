#ifndef __SINRGE2_H__
#define __SINRGE2_H__
#include "RbRenderTree.h"
#include <Windows.h>
#include "hge.h"
#include "sin_common.h"
#include "sin_color.h"
#include "sin_types.h"
#include <d3d8.h>

namespace Sin
{
	SinFrameStruct*		GetFrmStructPtr();

	IDirect3D8*			GetD3DPtr();
	IDirect3DDevice8*	GetD3DDevicePtr();
	D3DCAPS8*			GetD3DCapsPtr();
	static DWORD		GetMaxTexW() { return GetD3DCapsPtr()->MaxTextureWidth; }
	static DWORD		GetMaxTexH() { return GetD3DCapsPtr()->MaxTextureHeight; }
	RbRenderState*		GetRenderState();
	static u32			GetFrameWidth() { return GetFrmStructPtr()->m_screen_width; }
	static u32			GetFrameHeight() { return GetFrmStructPtr()->m_screen_height; }

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