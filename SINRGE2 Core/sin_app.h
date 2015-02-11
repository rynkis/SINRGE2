/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Application
*/
#ifndef __SIN_APPLICATION_H__
#define __SIN_APPLICATION_H__

#include "CRbRenderTree.h"
#include "hge.h"
#include "sin_timer.h"
#include "TANGRAM.h"

/***
 *	应用程序核心类
 */
class CApplication
{
public:
	inline HWND					GetMainHwnd() const { return m_frm_struct.m_hwnd; }
	inline const wchar_t *		GetTitle() const { return m_frm_struct.m_title; }
	inline HGE *				GetHgePtr() const { return m_pHge; }
	inline CRbRenderState *		GetRenderState() const { return m_pRenderState; }
	inline int					GetFrameWidth() const { return m_frm_struct.m_screen_width; }
	inline int					GetFrameHeight() const { return m_frm_struct.m_screen_height; }

	inline IC7pkgReader *		Get7pkgReader() const { return m_7pkgReader; }
	inline double				GetCurrentDelta() const { return m_current_delta; }
	inline HMODULE				GetSealHmodule() const { return m_hSeal; }

//private:
	/*static bool					GainFocusProc();
	static bool					LostFocusProc();*/

private:
	CApplication();
	~CApplication();
	
	CApplication(const CApplication&);
	CApplication& operator = (const CApplication&) { return *this; }

public:
	int							Run();
	bool						InitVideo();
	bool						InitAudio();
	void						InitRubyInterpreter();
	int							Eval(const char *script);
	int							RunScript();

	void						Quit();
	void						GraphicsUpdate();
	void						SystemUpdate();
	void						BrightnessUpdate();

	bool						IsInited() const { return m_inited; }

	void						LimitFps(int limit);
	int							GetRealFps();
	
	IC7pkgWriter *				Open7pkgWriter(const wchar_t * filename, const wchar_t * password);
	void						Close7pkgWriter(IC7pkgWriter * pw);

private:
	//void						ShowError(const wchar_t *szFormat, ...);
	void						Dispose();

	void						InitRubyInnerClassExt();
	void						InitExportSinInterface();

	int							GetRuntimeInfos();

	static	VALUE				RunScriptInProtect(VALUE argv);
	static	void				OnFailed(VALUE err);
	
	void						GetTimeDelta();

public:
	static bool					IsFileExist(const wchar_t *pFileName);
	static bool					IsFileExist(const char *pFileName);

	//static void					ShowErrorMsg(HWND hWnd, const wchar_t *szTitle, const wchar_t *szFormat, ...);
	//static void					ShowErrorMsg(HWND hWnd, const char *szTitle, const char *szFormat, ...);

private:
	wchar_t						szAppPath[MAX_PATH];
	wchar_t						szIniPath[MAX_PATH];
	wchar_t						szScripts[MAX_PATH];

	bool						m_with_console;
	char *						pScripts;

	HGE *						m_pHge;
	CRbRenderState *			m_pRenderState;
	SinFrameStruct				m_frm_struct;

	bool						m_inited;

	hgeQuad						m_quad;
	int							m_brightness;
	int							m_saved_brghtness;

	u32							m_frame_count;
	//fps
	CTimer *					m_fps_timer;
	unsigned int				m_frame, m_t0, m_t1, m_last_fps, m_real_fps;
	float						m_seconds;
	int							m_last_limit;

	unsigned int				m_current_ticks, m_target_ticks, f_frame_count, m_last_ticks, m_the_delay;
	float						m_rate_ticks;

	unsigned int				m_last;
	double						m_current_delta;
	HMODULE						m_hSeal;

private:
	class CVideoMgr *			m_pVideoMgr;			///<	AVI播放
	
	IC7pkgReader *				m_7pkgReader;
	func_pr_open				pr_open;
	func_pr_close				pr_close;

	func_pw_open				pw_open;
	func_pw_close				pw_close;

private:
	static CApplication *		s_pApp;					///<	[静态]		应用程序单例类指针

	friend CApplication *		GetAppPtr();
	friend int					SINRGE2Entry();
	friend class				MRbSinCore;
};

inline CApplication *			GetAppPtr() { return CApplication::s_pApp; }

#endif //__SIN_APPLICATION_H__