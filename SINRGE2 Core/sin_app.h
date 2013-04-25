/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Application
*/
#ifndef __SIN_APPLICATION_H__
#define __SIN_APPLICATION_H__

#include "RbRenderTree.h"
#include "hge.h"
#include "nge_timer.h"
#include <d3d8.h>

/***
 *	应用程序核心类
 */
class CApplication
{
public:
	inline HWND					GetMainHwnd() const { return m_frm_struct.m_hwnd; }
	inline const wchar_t* 		GetTitle() const { return m_frm_struct.m_title; }
	inline HGE*					GetHgePtr() const { return m_pHge; }
	inline RbRenderState*		GetRenderState() const { return m_pRenderState; }
	inline IDirect3D8*			GetD3DPtr() const { return m_ref_d3d; }
	inline IDirect3DDevice8*	GetD3DDevicePtr() const { return m_ref_device; }
	inline int					GetFrameWidth() const { return m_frm_struct.m_screen_width; }
	inline int					GetFrameHeight() const { return m_frm_struct.m_screen_height; }

	//inline int					GetBrightness() const { return m_brightness; }

	//inline nge_timer*			GetSysTimer() const { return m_sys_timer; }

	inline DWORD				GetMaxTexW() const { return m_d3d_caps.MaxTextureWidth; }
	inline DWORD				GetMaxTexH() const { return m_d3d_caps.MaxTextureHeight; }
	
private:
	static bool					GainFocusProc();
	static bool					LostFocusProc();

private:
	CApplication();
	~CApplication();
	
	CApplication(const CApplication&);
	CApplication& operator = (const CApplication&) { return *this; }

public:
	int							Run();
	bool						InitVideo();
	void						InitRubyInterpreter();
	int							Eval(const char* script);
	int							RunScript();

	void						Quit();
	void						GraphicsUpdate();
	void						SystemUpdate();
	void						BrightnessUpdate();
	void						FrameCountUpdate() { m_frame_count++; };

	//void						SetBrightness(int brightness) { m_brightness = brightness; };

	u32							GetTick() { return m_sys_timer->get_ticks(m_sys_timer); };

private:
	void						ShowError(const wchar_t* szFormat, ...);
	void						Dispose();

	void						InitRubyInnerClassExt();
	void						InitExportSinInterface();

	void						GetRuntimeInfos();

	static	VALUE				RunScriptInProtect(VALUE argv);
	static	void				OnFailed(VALUE err);

public:
	static bool					IsFileExist(const wchar_t* pFileName);
	static bool					IsFileExist(const char* pFileName);

	/*static void					ShowErrorMsg(HWND hWnd, const wchar_t* szTitle, const wchar_t* szFormat, ...);
	static void					ShowErrorMsg(HWND hWnd, const char* szTitle, const char* szFormat, ...);*/

private:
	wchar_t						szAppPath[MAX_PATH];
	wchar_t						szIniPath[MAX_PATH];
	wchar_t						szScripts[MAX_PATH];
	
	char*						pScripts;
	bool						m_with_console;

	HGE*						m_pHge;
	RbRenderState*				m_pRenderState;
	SinFrameStruct				m_frm_struct;

	IDirect3D8*					m_ref_d3d;
	IDirect3DDevice8*			m_ref_device;
	D3DCAPS8					m_d3d_caps;

	nge_timer*					m_sys_timer;

	hgeQuad						m_quad;
	int							m_brightness;
	int							m_saved_brghtness;

	u32							m_frame_count;

private:
	class CVideoMgr*			m_pVideoMgr;			///<	AVI播放

private:
	static CApplication*		s_pApp;					///<	[静态]		应用程序单例类指针

	friend CApplication*		GetAppPtr();
	friend int					SINRGE2Entry();
	friend class				MRbSinCore;
};

inline CApplication*			GetAppPtr() { return CApplication::s_pApp; }

#endif //__SIN_APPLICATION_H__