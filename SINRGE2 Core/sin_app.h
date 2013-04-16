#ifndef __SIN_APPLICATION_H__
#define __SIN_APPLICATION_H__

#include "RbRenderTree.h"
#include "hge.h"
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
	inline unsigned long		GetFrameWidth() const { return m_frm_struct.m_screen_width; }
	inline unsigned long		GetFrameHeight() const { return m_frm_struct.m_screen_height; }

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
	void						AppInitialize();
	int							Eval(const char* script);
	int							RunScript();

	void						Quit();
	void						GraphicsUpdate();

	void						SetTitle(const wchar_t* title);

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

	static void					ShowErrorMsg(HWND hWnd, const wchar_t* szTitle, const wchar_t* szFormat, ...);
	static void					ShowErrorMsg(HWND hWnd, const char* szTitle, const char* szFormat, ...);

private:
	wchar_t						szAppPath[MAX_PATH];
	wchar_t						szIniPath[MAX_PATH];
	wchar_t						szScripts[MAX_PATH];
	
	char*						pScripts;

	HGE*						m_pHge;
	RbRenderState*				m_pRenderState;
	SinFrameStruct				m_frm_struct;

	IDirect3D8*					m_ref_d3d;
	IDirect3DDevice8*			m_ref_device;
	D3DCAPS8					m_d3d_caps;

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