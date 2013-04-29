/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Application
*/
#include "MRbSinCore.h"
#include "MRbInput.h"
#if SIN_USE_SEAL
#include "MRbSeal.h"
#else
#include "MRbAudio.h"
#endif
#include "RbRect.h"
#include "RbColor.h"
#include "RbTone.h"
#include "RbFont.h"
#include "RbBitmap.h"
#include "RbViewport.h"
#include "RbPlane.h"
#include "RbSprite.h"
#include "RbTable.h"
#include "RbParticleSystem.h"
#include "RbWin32API.h"
#include "sin_app.h"
#include "sin_video.h"


namespace
{
	/*
	**	call-seq:
	**		msgbox_p(...)		-> nil
	**
	*/
	static VALUE rdf_msgboxp(int argc, VALUE *argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_inspect(argv[i]));
			rb_str_buf_append(str, rb_default_rs);
		}
		// 清除多余消息队列
		MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::UTF8ToUnicode(RSTRING_PTR(str)), GetAppPtr()->GetTitle(), 0);

		return Qnil;
	}
	/*
	**	call-seq:
	**		p(...)		-> nil
	**
	*/
	static VALUE rdf_p(int argc, VALUE *argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_inspect(argv[i]));
			rb_str_buf_append(str, rb_default_rs);
		}
		
		printf(Kconv::UTF8ToAnsi(RSTRING_PTR(str)));

		return Qnil;
	}
	/*
	**	call-seq:
	**		msgbox(...)		-> nil
	**
	*/
	static VALUE rdf_msgbox(int argc, VALUE *argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		rb_encoding* enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_enc_associate(NIL_P(argv[i]) ? rb_str_new2("nil") : rb_obj_as_string(argv[i]), enc));
		}
		// 清除多余消息队列
		MSG msg; PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::UTF8ToUnicode(RSTRING_PTR(str)), GetAppPtr()->GetTitle(), 0);

		return Qnil;
	}
	/*
	**	call-seq:
	**		print(...)		-> nil
	**
	*/
	static VALUE rdf_print(int argc, VALUE *argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		rb_encoding* enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_enc_associate(NIL_P(argv[i]) ? rb_str_new2("nil") : rb_obj_as_string(argv[i]), enc));
		}
		
		printf(Kconv::UTF8ToAnsi(RSTRING_PTR(str)));

		return Qnil;
	}
	
	static VALUE load_data(int argc, VALUE filename)
	{
		SafeStringValue(filename);
		VALUE rbread = rb_file_open(RSTRING_PTR(filename), "rb");
		VALUE rbdata = rb_marshal_load(rbread);
		(void)rb_io_close(rbread);
		
		return rbdata;
	}
}

static const wchar_t* pDefaultConsole	= L"0";
static const wchar_t* pDefaultScripts	= L"main.rb";

/***
 *	全局单例静态变量定义
 */
CApplication* CApplication::s_pApp = 0;


/***
 *	构造函数
 */
CApplication::CApplication()
	: m_pVideoMgr(0)
	, pScripts(0)
	, m_pHge(0)
	, m_pRenderState(0)

	, m_ref_d3d(0)
	, m_ref_device(0)
	, m_with_console(false)
	, m_brightness(255)
	, m_saved_brghtness(255)

	, m_frame_count(0)

	, m_frame(0)
	, m_t0(0)
	, m_last_fps(0)
	, m_last(0)
{
	s_pApp = this;
	szAppPath[0] = 0;
	szIniPath[0] = 0;
	szScripts[0] = 0;
	memset(&m_d3d_caps, 0, sizeof(m_d3d_caps));
	//m_sys_timer = nge_timer_create();
	m_pHge = hgeCreate(HGE_VERSION);
	
	m_quad.v[0].z = 
	m_quad.v[1].z = 
	m_quad.v[2].z = 
	m_quad.v[3].z = 0.5f;
 	m_quad.tex	  = 0;
	m_quad.blend = BLEND_DEFAULT;
	m_quad.blend_color = 0x00000000;

	m_quad.v[0].x = 0;									m_quad.v[0].y = 0;
	m_quad.v[1].x = (float)m_frm_struct.m_screen_width;	m_quad.v[1].y = 0;
	m_quad.v[2].x = (float)m_frm_struct.m_screen_width;	m_quad.v[2].y = (float)m_frm_struct.m_screen_height;
	m_quad.v[3].x = 0;									m_quad.v[3].y = (float)m_frm_struct.m_screen_height;
	
	m_fps_timer = new CTimer();
}

/***
 *	析构函数
 */
CApplication::~CApplication()
{
	Dispose();
}

/***
 *	释放相关资源
 */
void CApplication::Dispose()
{
	if (m_pHge)
	{
		m_pHge->System_Shutdown();
		m_pHge->Release();
		m_pHge = 0;
	}

	if (m_pVideoMgr)
	{
		m_pVideoMgr->Destroy();
		delete m_pVideoMgr;
		m_pVideoMgr = 0;
	}

	if (m_fps_timer)
	{
		delete m_fps_timer;
		m_fps_timer = 0;
	}
}

void CApplication::Quit()
{
	rb_exit(EXIT_SUCCESS);
}

void CApplication::GraphicsUpdate()
{
    if (!m_pHge->System_Update())
		Quit();
}

void CApplication::SystemUpdate()
{
	if (!m_pHge->System_PeekMessage())
		Quit();
}

void CApplication::BrightnessUpdate()
{
	if (m_saved_brghtness != m_brightness)
	{
		m_quad.v[0].col =
		m_quad.v[1].col =
		m_quad.v[2].col =
		m_quad.v[3].col = ((255 - m_brightness) << 24);
		m_saved_brghtness = m_brightness;
	}
	if (m_brightness == 255) return;
	m_pHge->Gfx_RenderQuad(&m_quad);
}

int CApplication::Run()
{
	GetRuntimeInfos();
	InitRubyInterpreter();
	InitRubyInnerClassExt();
	InitExportSinInterface();
	//m_sys_timer->start(m_sys_timer);
    return RunScript();
}

int CApplication::RunScript()
{
	if(!IsFileExist(pScripts))
	{
		MessageBoxW(m_frm_struct.m_hwnd, L"Failed to load script.", m_frm_struct.m_title,  MB_ICONWARNING);
		return 1;
	}

	int state = 0;

	int len = strlen(pScripts);
	if (pScripts[len - 1] == 'b' && pScripts[len -2] == 'r')
	{
		VALUE rbread = rb_file_open(pScripts, "rb");
		VALUE script = rb_funcall(rbread, rb_intern("read"), 0);
		(void)rb_io_close(rbread);
		VALUE name	 = rb_str_new2(pScripts);

		VALUE argv = rb_ary_new2(2);
		rb_ary_push(argv, name);
		rb_ary_push(argv, script);
		rb_ary_freeze(argv);

		VALUE result = rb_protect(RunScriptInProtect, argv, &state);
		if (state)
		{
			VALUE err = rb_errinfo();
			if (!rb_obj_is_kind_of(err, rb_eSystemExit))
			{
				OnFailed(err);
			}
		}
	}
	else
	{
		VALUE rbdata = load_data(1, rb_str_new2(pScripts));

		int arylen = NUM2INT(rb_funcall(rbdata, rb_intern("size"), 0));

		VALUE ary, f_name, script;
		VALUE argv = rb_ary_new2(2);
		//VALUE cInflate = rb_class_new;//rb_eval_string("Zlib::Inflate");
		for (int pos = 0; pos < arylen; ++pos)
		{
			ary = rb_ary_entry(rbdata, pos);
			f_name = rb_ary_entry(ary, 0);
			script = rb_ary_entry(ary, 1);
			//script = rb_funcall(cInflate, rb_intern("inflate"), 1, rb_ary_entry(ary, 1));//rb_ary_entry(ary, 1);
			rb_ary_clear(argv);
			rb_ary_push(argv, f_name);
			rb_ary_push(argv, script);
			VALUE result = rb_protect(RunScriptInProtect, rb_ary_entry(rbdata, pos), &state);

			if (state)
			{
				VALUE err = rb_errinfo();
				if (!rb_obj_is_kind_of(err, rb_eSystemExit))
				{
					OnFailed(err);
				}
			}
		}
	}
	return state;//ruby_cleanup(state);
}

void CApplication::InitRubyInterpreter()
{
	///<	初始化解释器
	int		argc = 0;
	char**	argv = 0;

	ruby_sysinit(&argc, &argv);
	{
		RUBY_INIT_STACK
		ruby_init();
		ruby_set_argv(argc - 1, argv + 1);
		//	rb_set_kcode("utf8");
		//  ruby_init_loadpath();
		ruby_incpush("./");
		ruby_script("SINRGE2");
	}
}

void CApplication::GetRuntimeInfos()
{
	// app路径
	DWORD len = ::GetModuleFileNameW(NULL, szAppPath, MAX_PATH);
	for (--len; len > 0; --len)
	{
		if (szAppPath[len] == L'\\' || szAppPath[len] == L'/')
		{
			szAppPath[len] = 0;
			break;
		}
	}
	::SetCurrentDirectoryW(szAppPath);
	
	// ini文件路径
	len = ::GetModuleFileNameW(NULL, szIniPath, MAX_PATH);
	szIniPath[len - 1] = L'i';
	szIniPath[len - 2] = L'n';
	szIniPath[len - 3] = L'i';
	
	wchar_t szConsole[MAX_PATH];
	// ini文件存在
	if (IsFileExist(szIniPath))
	{
		GetPrivateProfileStringW(L"Setting", L"Scripts", pDefaultScripts, szScripts, MAX_PATH, szIniPath);
		GetPrivateProfileStringW(L"Setting", L"Console", pDefaultConsole, szConsole, MAX_PATH, szIniPath);
	}
	else
	{
		wcscpy_s(szScripts, pDefaultScripts);
		wcscpy_s(szConsole, pDefaultConsole);
	}

	len = WideCharToMultiByte(CP_OEMCP, NULL, szScripts, -1, NULL, 0, NULL, FALSE);
	pScripts = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, szScripts, -1, pScripts, len, NULL, FALSE);

	if (szConsole[0] == '1')
		m_with_console = true;
}

int CApplication::Eval(const char* script)
{
	int status = -1;

	rb_eval_string_protect(script, &status);

	if ( status )
	{
		rb_eval_string_protect("print $!", &status);
		return 1;
	}
	return 0;
}

//void CApplication::ShowError(const wchar_t* szFormat, ...)
//{
//	wchar_t szError[1024];
//
//	va_list ap;
//	va_start(ap, szFormat);
//	vswprintf_s(szError, szFormat, ap);
//	va_end(ap);
//
//	MessageBoxW(m_frm_struct.m_hwnd, szError, m_frm_struct.m_title, MB_ICONERROR);
//}
//
//void CApplication::ShowErrorMsg(HWND hWnd, const wchar_t* szTitle, const wchar_t* szFormat, ...)
//{
//	wchar_t szError[1024];
//
//	va_list ap;
//	va_start(ap, szFormat);
//	vswprintf_s(szError, szFormat, ap);
//	va_end(ap);
//
//	MessageBoxW(hWnd, szError, szTitle, MB_ICONERROR);
//}

void CApplication::InitRubyInnerClassExt()
{
	rb_define_global_function("msgbox_p",		(RbFunc)rdf_msgboxp,		-1);
	rb_define_global_function("msgbox",			(RbFunc)rdf_msgbox,			-1);
	if (m_with_console)
	{
		rb_define_global_function("p",			(RbFunc)rdf_p,				-1);
		rb_define_global_function("print",		(RbFunc)rdf_print,			-1);
	}
	else
	{
		rb_define_global_function("p",			(RbFunc)rdf_msgboxp,		-1);
		rb_define_global_function("print",		(RbFunc)rdf_msgbox,			-1);
	}

	ruby_Init_Fiber_as_Coroutine();
	Init_zlib();
	Init_nonblock();
	Init_wait();
}

void CApplication::InitExportSinInterface()
{
	MRbSinCore::InitLibrary();
	MRbInput::InitLibrary();

#if SIN_USE_SEAL
	MRbSeal::InitLibrary();
#else
	MRbAudio::InitLibrary();
#endif
	
	RbRect::InitLibrary();
	RbColor::InitLibrary();
	RbTone::InitLibrary();
	RbFont::InitLibrary();
	RbBitmap::InitLibrary();
	RbViewport::InitLibrary();
	RbPlane::InitLibrary();
	RbSprite::InitLibrary();
	RbTable::InitLibrary();
	RbParticleSystem::InitLibrary();
	RbWin32API::InitLibrary();
		
	rb_define_module_function(rb_mSin, "load_data",	RbFunc(load_data), 1);
}

VALUE CApplication::RunScriptInProtect(VALUE argv)
{
	const ID	id_eval = rb_intern("eval");
	const VALUE	binding = rb_const_get(rb_mKernel, rb_intern("TOPLEVEL_BINDING"));
	const VALUE	line_no = INT2FIX(1);
		
	const VALUE f_name	= rb_ary_entry(argv, 0);
	const VALUE script	= rb_ary_entry(argv, 1);

	return rb_funcall(rb_mKernel, id_eval, 4, script, binding, f_name, line_no);
}

void CApplication::OnFailed(VALUE err)
{
	VALUE errmsg;

	// 以下有被注入的风险，建议在发行版本中做下加密或直接剔除
	VALUE source_ary	= rb_eval_string("matchstr = $!.is_a?(SyntaxError) ? $!.to_s : $@[0]; matchstr =~ /(.*):([0-9]+)/; ary = [$1.to_s, $2.to_i]; ary");
	VALUE sourcefile	= rb_ary_entry(source_ary, 0);
	VALUE sourceline	= rb_ary_entry(source_ary, 1);

	const VALUE message			= rb_funcall(err, rb_intern("message"), 0);
	const VALUE message_str		= rb_funcall(message, rb_intern("gsub"), 2, rb_str_new2("\n"), rb_str_new2("\r\n"));

	const char* clsname			= rb_obj_classname(err);
	const char* msg				= RSTRING_PTR(message_str);

	if (rb_obj_is_kind_of(err, rb_eSyntaxError))
		errmsg = rb_sprintf("Script '%s' line %d: %s occurred.", RSTRING_PTR(sourcefile), NUM2INT(sourceline), clsname);
	else
		errmsg = rb_sprintf("Script '%s' line %d: %s occurred.\n\n%s", RSTRING_PTR(sourcefile), NUM2INT(sourceline), clsname, msg);

	MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::AnsiToUnicode(RSTRING_PTR(errmsg)), GetAppPtr()->GetTitle(), MB_ICONWARNING);
}

bool CApplication::InitVideo()
{
	// Set our render proc
	//m_pHge->System_SetState(HGE_FOCUSLOSTFUNC, LostFocusProc);
	m_pHge->System_SetState(HGE_RENDERFUNC, RbRenderTree::RenderProc);
	m_pHge->System_SetState(HGE_TEXTUREFILTER, false);

	bool	isFullScreen;

	if (m_frm_struct.m_forbid_fullscreen)
		isFullScreen = false;
	else
	{
		if (m_frm_struct.m_fullscreen_start)
			isFullScreen = true;
		else
		{
			isFullScreen = m_frm_struct.m_screen_width >= GetSystemMetrics(SM_CXSCREEN) && 
				m_frm_struct.m_screen_height >= GetSystemMetrics(SM_CYSCREEN);
		}
	}
	m_pHge->System_SetState(HGE_TITLE, m_frm_struct.m_title);
	m_pHge->System_SetState(HGE_SCREENWIDTH, m_frm_struct.m_screen_width);
	m_pHge->System_SetState(HGE_SCREENHEIGHT, m_frm_struct.m_screen_height);
	m_pHge->System_SetState(HGE_SCREENBPP, 32);
 	m_pHge->System_SetState(HGE_WINDOWED, !isFullScreen);
	m_pHge->System_SetState(HGE_FPS, 60);
	
	if(!m_pHge->System_Initiate())
		return false;
	
	//	start HGE system 
	m_pHge->System_Start();
	//	Save the window's hwnd
	m_frm_struct.m_hwnd = m_pHge->System_GetState(HGE_HWND);

	RbRenderTree::Init();

	m_pRenderState = new RbRenderState();
	m_pRenderState->Save(0, 0, GetFrameWidth(), GetFrameHeight());

	//	Hack the D3D pointer & D3DDevice pointer
	HTEXTURE pTmpTex = m_pHge->Texture_Create(2, 2); 
	if (!pTmpTex)
		goto failed_return;

	if (FAILED((reinterpret_cast<LPDIRECT3DTEXTURE8>(pTmpTex))->GetDevice(&m_ref_device)))
		goto failed_return;

	if (FAILED(m_ref_device->GetDirect3D(&m_ref_d3d)))
		goto failed_return;

	if (FAILED(m_ref_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3d_caps)))
		goto failed_return;

	m_pHge->Texture_Free(pTmpTex);
	pTmpTex = NULL;

	// video
	m_pVideoMgr = new CVideoMgr();
	if (!m_pVideoMgr)
		goto failed_return;
	if (!m_pVideoMgr->Init())
		goto failed_return;

	return true;

failed_return:
	if (pTmpTex)
	{
		m_pHge->Texture_Free(pTmpTex);
		pTmpTex = NULL;
	}
	return false;
}

void CApplication::LimitFps(int limit)
{
	if (!m_fps_timer->IsStarted())
	{
		m_fps_timer->Start();
		m_current_ticks = 0;
		m_target_ticks  = 0;
		m_last_ticks    = 0;
		f_frame_count   = 0;
		m_rate_ticks    = 1000.0f / limit;
		return;
	}
	f_frame_count++;
	m_current_ticks = m_fps_timer->GetTicks();
	m_target_ticks = m_last_ticks + (unsigned int) ((float) f_frame_count * m_rate_ticks);

	m_frame_count++;

	if (m_current_ticks <= m_target_ticks)
	{
		m_the_delay = m_target_ticks - m_current_ticks;
		RB_SLEEP(m_the_delay);
	}
	else
	{
		f_frame_count = 0;
		m_last_ticks = m_fps_timer->GetTicks();
	}
}

int CApplication::GetRealFps()
{
	m_frame++;
	m_t0 = m_fps_timer->GetTicks();
	if ( (m_t0 - m_t1) >= 1000)
	{
		m_seconds = (m_t0 - m_t1) / 1000.0;
		m_last_fps = m_real_fps = m_frame / m_seconds;
		m_t1 = m_t0;
		m_frame = 0;
		return m_real_fps;
	}
	return m_last_fps;
}

double CApplication::GetTimeDelta()
{
	if (!m_fps_timer)
		return 0;
		
	double fDeltaTime = (m_fps_timer->GetTicks() - m_last) / 1000.0;
	m_last = m_fps_timer->GetTicks();

	if (fDeltaTime > 1.0) fDeltaTime = 0.0;

	return fDeltaTime;
}

/**
 *	判断指定文件是否存在
 */
bool CApplication::IsFileExist(const wchar_t* pFileName)
{
	return (GetFileAttributesW(pFileName) != INVALID_FILE_ATTRIBUTES);
}

bool CApplication::IsFileExist(const char* pFileName)
{
	return (GetFileAttributesA(pFileName) != INVALID_FILE_ATTRIBUTES);
}

//bool CApplication::LostFocusProc()
//{
//	GetTimeDelta();
//	return false;
//}