/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Application
*/
#include "MRbSinCore.h"
#include "MRbInput.h"
#ifndef SIN_USE_SEAL
#include "MRbAudio.h"
#else
#include "MRbSeal.h"
#endif
#include "CRbRect.h"
#include "CRbColor.h"
#include "CRbTone.h"
#include "CRbFont.h"
#include "CRbBitmap.h"
#include "CRbViewport.h"
#include "CRbPlane.h"
#include "CRbSprite.h"
#include "CRbTable.h"
#include "CRbParticleSystem.h"
#include "CRbWin32API.h"
#include "CRb7pkgWriter.h"
#include "CRbCharmap.h"
#include "sin_common.h"
#include "sin_app.h"
#include "sin_video.h"
#include "CBrowseDir.h"

namespace
{
	/*
	**	call-seq:
	**		msgbox_p(...)		-> nil
	**
	*/
	static VALUE rdf_msgboxp(int argc, VALUE * argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);
		
		rb_encoding * enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_inspect(argv[i]));
			rb_str_buf_append(str, rb_default_rs);
		}
		// 清除多余消息队列
		GetAppPtr()->SystemUpdate();
		MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::UTF8ToUnicode(RSTRING_PTR(str)), GetAppPtr()->GetTitle(), 0);

		return Qnil;
	}
	/*
	**	call-seq:
	**		msgbox(...)		-> nil
	**
	*/
	static VALUE rdf_msgbox(int argc, VALUE * argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		rb_encoding * enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_enc_associate(NIL_P(argv[i]) ? rb_str_new2("nil") : rb_obj_as_string(argv[i]), enc));
		}
		// 清除多余消息队列
		GetAppPtr()->SystemUpdate();
		MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::UTF8ToUnicode(RSTRING_PTR(str)), GetAppPtr()->GetTitle(), 0);

		return Qnil;
	}
	/*
	**	call-seq:
	**		print(...)		-> nil
	**
	*/
	static VALUE rdf_print(int argc, VALUE * argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		rb_encoding * enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_enc_associate(NIL_P(argv[i]) ? rb_str_new2("nil") : rb_obj_as_string(argv[i]), enc));
		}
		
		printf(Kconv::UTF8ToAnsi(RSTRING_PTR(str)));

		return Qnil;
	}
	/*
	**	call-seq:
	**		puts(...)		-> nil
	**
	*/
	static VALUE rdf_puts(int argc, VALUE * argv/*, VALUE caller*/)
	{
		int i;

		VALUE str = rb_str_buf_new(0);

		rb_encoding * enc = rb_utf8_encoding();
		for (i = 0; i < argc; ++i)
		{
			rb_str_buf_append(str, rb_enc_associate(NIL_P(argv[i]) ? rb_str_new2("nil") : rb_obj_as_string(argv[i]), enc));
			rb_str_buf_append(str, rb_default_rs);
		}
		
		printf(Kconv::UTF8ToAnsi(RSTRING_PTR(str)));

		return Qnil;
	}
	
	static VALUE load_data(int argc, VALUE filename)
	{
		SafeStringValue(filename);
		VALUE rbread, rbdata;
		void * data = NULL;
		u32 file_size;
		wchar_t * filenameW = Kconv::UTF8ToUnicode(RSTRING_PTR(filename));

		if (GetAppPtr()->Get7pkgReader())
		{
			file_size = GetAppPtr()->Get7pkgReader()->FileLength(filenameW);
			if (!file_size)
				goto __load_from_hd;
			
			if (!(data = malloc(file_size)))
				goto __failed_return;

			if (!GetAppPtr()->Get7pkgReader()->LoadData(filenameW, data))
				goto __failed_return;
			
			rbread = rb_str_new((char *)data, file_size);
			rbdata = rb_marshal_load(rbread);
			rbread = Qnil;
			if (data)
			{
				free(data);
				data = 0;
			}
			return rbdata;
		}

__load_from_hd:
		if (GetFileAttributesW(filenameW) != INVALID_FILE_ATTRIBUTES)
		{
			rbread = rb_file_open(RSTRING_PTR(filename), "rb");
			rbdata = rb_marshal_load(rbread);
			(void)rb_io_close(rbread);
			return rbdata;
		}

__failed_return:
		if (data)
		{
			free(data);
			data = NULL;
		}
		return Qnil;
	}
}

//static const wchar_t * pDefaultConsole	= L"0";
static const wchar_t * pDefaultScripts	= L"main.rb";

bool CApplication::b_exited = false;

/***
 *	全局单例静态变量定义
 */
CApplication * CApplication::s_pApp = 0;


/***
 *	构造函数
 */
CApplication::CApplication()
	: m_pVideoMgr(0)
	, m_7pkgReader(0)

	, pr_open(0)
	, pr_close(0)

	, pScripts(0)
	, m_pRenderState(0)
	/*, m_ref_d3d(0)
	, m_ref_device(0)*/
	, m_with_console(false)
	, m_brightness(255)
	, m_saved_brghtness(255)

	, m_frame_count(0)

	, m_frame(0)
	, m_t0(0)
	, m_last_fps(0)
	, m_last(0)
	, m_last_limit(0)

	, m_current_delta(0)

	, m_hSeal(0)

	, m_inited(false)
{
	s_pApp = this;
	szAppPath[0] = 0;
	szIniPath[0] = 0;
	szScripts[0] = 0;

	//memset(&m_d3d_caps, 0, sizeof(m_d3d_caps));
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

	if (m_7pkgReader)
	{
		pr_close(m_7pkgReader);
		m_7pkgReader = 0;
	}

	if (m_hSeal != NULL)
	{
		SealCleanup();
		FreeLibrary(m_hSeal);
		m_hSeal = NULL;
	}
}

bool CApplication::Quit()
{
	b_exited = true;

	const ID	id_exit = rb_intern("exit");
	const VALUE	binding = rb_const_get(rb_mKernel, rb_intern("TOPLEVEL_BINDING"));

	rb_funcall(rb_mKernel, id_exit, 0);
	rb_exit(EXIT_SUCCESS);

	return false;
}

bool CApplication::LostFocus()
{
	const ID	id_lost_focus = rb_intern("lost_focus");
	const VALUE	binding = rb_const_get(rb_mKernel, rb_intern("TOPLEVEL_BINDING"));
	rb_funcall(rb_mFrame, id_lost_focus, 0);

	return true;
}

bool CApplication::GainFocus()
{
	const ID	id_gain_focus = rb_intern("gain_focus");
	const VALUE	binding = rb_const_get(rb_mKernel, rb_intern("TOPLEVEL_BINDING"));
	rb_funcall(rb_mFrame, id_gain_focus, 0);
	
	return true;
}

void CApplication::GraphicsUpdate()
{
	GetTimeDelta();
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
	if (GetRuntimeInfos()) return 1;
	InitRubyInterpreter();
	InitRubyInnerClassExt();
	InitExportSinInterface();
    return RunScript();
}

int CApplication::RunScript()
{
	if (m_7pkgReader)
	{
		if (m_7pkgReader->IsFileExist(szScripts))
			goto __run_with_data;
	}

	if (!IsFileExist(pScripts))
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
		return state;//ruby_cleanup(state);
	}
	else goto __run_with_data;

__run_with_data:
	VALUE rbdata = load_data(1, rb_str_new2(pScripts));
	if (NIL_P(rbdata)) return 0;

	int arylen = NUM2INT(rb_funcall(rbdata, rb_intern("size"), 0));

	for (int pos = 0; pos < arylen; ++pos)
	{
		VALUE temp = rb_ary_entry(rbdata, pos);
		VALUE code = rb_ary_entry(temp, 2);
		code = rb_funcall(mZlib, rb_intern("inflate"), 1, code);

		VALUE argv = rb_ary_new2(2);
		rb_ary_push(argv, rb_ary_entry(temp, 1));
		rb_ary_push(argv, code);
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
	return state;//ruby_cleanup(state);
}

void CApplication::InitRubyInterpreter()
{
	///<	初始化解释器
	int		argc = 0;
	char **	argv = 0;

	ruby_sysinit(&argc, &argv);
	{
		RUBY_INIT_STACK
		ruby_init();
		ruby_set_argv(argc - 1, argv + 1);
		//rb_set_kcode("utf8");
		//ruby_init_loadpath();
		ruby_incpush("./");
		ruby_script(SIN_CORENAME);
	}
}

int CApplication::GetRuntimeInfos()
{
	// app路径
	u32 len = ::GetModuleFileNameW(NULL, szAppPath, MAX_PATH);
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
	
	wchar_t szConsole[MAX_PATH], szPackage[MAX_PATH], szExtFunc[MAX_PATH];
	// ini文件存在
	if (IsFileExist(szIniPath))
	{
		GetPrivateProfileStringW(L"Setting", L"Scripts", pDefaultScripts, szScripts, MAX_PATH, szIniPath);
		GetPrivateProfileStringW(L"Setting", L"Console", L"", szConsole, MAX_PATH, szIniPath);
		GetPrivateProfileStringW(L"Setting", L"Package", L"", szPackage, MAX_PATH, szIniPath);
		GetPrivateProfileStringW(L"Setting", L"ExtFunc", L"", szExtFunc, MAX_PATH, szIniPath);
	}
	else
	{
		wcscpy_s(szScripts, pDefaultScripts);
		szConsole[0] = 0;
		szPackage[0] = 0;
		szExtFunc[0] = 0;
	}

	len = WideCharToMultiByte(CP_OEMCP, NULL, szScripts, -1, NULL, 0, NULL, FALSE);
	pScripts = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, szScripts, -1, pScripts, len, NULL, FALSE);

	if (szConsole[0] == '1')
		m_with_console = true;

	if (szExtFunc[0] != 0)
		m_hSeal = ::LoadLibraryW(szExtFunc);

	if (m_hSeal)
	{
		pr_open = (func_pr_open)::GetProcAddress(m_hSeal, "pr_open");
		pr_close = (func_pr_close)::GetProcAddress(m_hSeal, "pr_close");
		pw_open = (func_pw_open)::GetProcAddress(m_hSeal, "pw_open");
		pw_close = (func_pw_close)::GetProcAddress(m_hSeal, "pw_close");

		if (szPackage[0] != 0)
		{
			if (!IsFileExist(szPackage))
			{
				MessageBoxW(m_frm_struct.m_hwnd, L"Failed to find 7pkg file.", m_frm_struct.m_title,  MB_ICONWARNING);
				return 1;
			}
			else if (!pr_open && !pr_close && !pw_open && !pw_close)
			{
				MessageBoxW(m_frm_struct.m_hwnd, L"Failed to load expansion functions.", m_frm_struct.m_title,  MB_ICONWARNING);
				return 1;
			}
			else if (pr_open && pr_close)
				m_7pkgReader = pr_open(szPackage, L"sinrge2");
		}
	}
	return 0;
}

int CApplication::EvalRbString(const char * script)
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

//void CApplication::ShowError(const wchar_t * szFormat, ...)
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
//void CApplication::ShowErrorMsg(HWND hWnd, const wchar_t * szTitle, const wchar_t * szFormat, ...)
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
	if (m_with_console)
	{
		rb_define_global_function("print",		(RbFunc)rdf_print,			-1);
		rb_define_global_function("puts",		(RbFunc)rdf_puts,			-1);
	}
	else
	{
		rb_define_global_function("p",			(RbFunc)rdf_msgboxp,		-1);
		rb_define_global_function("print",		(RbFunc)rdf_msgbox,			-1);
		rb_define_global_function("puts",		(RbFunc)rdf_msgbox,			-1);
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
	
	CRbRect::InitLibrary();
	CRbColor::InitLibrary();
	CRbTone::InitLibrary();
	CRbFont::InitLibrary();
	CRbBitmap::InitLibrary();
	CRbViewport::InitLibrary();
	CRbPlane::InitLibrary();
	CRbSprite::InitLibrary();
	CRbTable::InitLibrary();
	CRbParticleSystem::InitLibrary();
	CRbWin32API::InitLibrary();

	CRbCharmap::InitLibrary();
	
	rb_define_module_function(rb_mSin, "msgbox_p",	RbFunc(rdf_msgboxp), -1);
	rb_define_module_function(rb_mSin, "msgbox",	RbFunc(rdf_msgbox), -1);
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

	const char * clsname		= rb_obj_classname(err);
	const char * msg			= RSTRING_PTR(message_str);

	if (rb_obj_is_kind_of(err, rb_eSyntaxError))
		errmsg = rb_sprintf("Script '%s' line %d: %s occurred.", RSTRING_PTR(sourcefile), NUM2INT(sourceline), clsname);
	else
		errmsg = rb_sprintf("Script '%s' line %d: %s occurred.\n\n%s", RSTRING_PTR(sourcefile), NUM2INT(sourceline), clsname, msg);

	MessageBoxW(GetAppPtr()->GetMainHwnd(), Kconv::AnsiToUnicode(RSTRING_PTR(errmsg)), GetAppPtr()->GetTitle(), MB_ICONWARNING);
}

bool CApplication::InitVideo()
{
	if (m_inited) return true;
	// Set our render proc
	m_pHge->System_SetState(HGE_RENDERFUNC, CRbRenderTree::RenderProc);
	m_pHge->System_SetState(HGE_TEXTUREFILTER, false);
	m_pHge->System_SetState(HGE_EXITFUNC, CApplication::Quit);
	m_pHge->System_SetState(HGE_FOCUSLOSTFUNC, CApplication::LostFocus);
	m_pHge->System_SetState(HGE_FOCUSGAINFUNC, CApplication::GainFocus);

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
	m_pHge->System_SetState(HGE_FPS, m_pHge->System_GetState(HGE_FPS));
	m_pHge->System_SetState(SIN_TOOL_WINDOW, m_frm_struct.m_tool_window);
	
	if(!m_pHge->System_Initiate())
		return false;
	
	//	start HGE system 
	m_pHge->System_Start();
	//	Save the window's hwnd
	m_frm_struct.m_hwnd = m_pHge->System_GetState(HGE_HWND);

	//CRbRenderTree::Init();

	m_pRenderState = new CRbRenderState();
	m_pRenderState->Save(0, 0, GetFrameWidth(), GetFrameHeight());

	// video
	m_pVideoMgr = new CVideoMgr();
	if (!m_pVideoMgr)
		return false;
	if (!m_pVideoMgr->Init())
		return false;

	CStatDir statdir;
	if (statdir.SetInitDir("Fonts"))
	{
		//开始遍历
		vector<string>file_vec = statdir.BeginBrowseFilenames("*.*");
		for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
		{
			//std::cout << *it << std::endl;

			int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, (*it).c_str(), (*it).size(), NULL, 0);
			if (bufferlen == 0)
				continue;

			LPWSTR widestr = new WCHAR[bufferlen + 1];
			::MultiByteToWideChar(CP_ACP, 0, (*it).c_str(), (*it).size(), widestr, bufferlen);
			widestr[bufferlen] = 0;

			::AddFontResourceExW(widestr, FR_PRIVATE, NULL);

			delete[] widestr;
		}
	}
	::SetCurrentDirectoryW(szAppPath);

	m_inited = true;

	return m_inited;
}

bool CApplication::InitAudio()
{
#if SIN_USE_SEAL
	if (!m_hSeal) return false;
	CRb7pkgWriter::InitLibrary();
	return MRbSeal::InitLibrary();
#else
	MRbAudio::InitLibrary();
	return true;
#endif
}

void CApplication::LimitFps(int limit)
{
	if (!m_fps_timer->IsStarted())
	{
		m_fps_timer->Start();
		m_current_ticks = 0;
		m_target_ticks = 0;
		m_last_ticks = 0;
		f_frame_count = 0;
		return;
	}
	if (m_last_limit != limit)
	{
		m_rate_ticks = 1000.0f / limit;
		m_last_limit = limit;
	}
	f_frame_count++;
	m_current_ticks = m_fps_timer->GetTicks();
	m_target_ticks = m_last_ticks + (unsigned int)((float)f_frame_count * m_rate_ticks);

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

void CApplication::GetTimeDelta()
{
	if (!m_fps_timer)
	{
		m_current_delta = 0;
		return;
	}
	
	m_current_delta = (m_fps_timer->GetTicks() - m_last) / 1000.0;
	m_last = m_fps_timer->GetTicks();

	if (m_current_delta > 1.0) m_current_delta = 0.0;
}

/**
 *	判断指定文件是否存在
 */
bool CApplication::IsFileExist(const wchar_t * pFileName)
{
	return (GetFileAttributesW(pFileName) != INVALID_FILE_ATTRIBUTES);
}

bool CApplication::IsFileExist(const char * pFileName)
{
	return (GetFileAttributesA(pFileName) != INVALID_FILE_ATTRIBUTES);
}

IC7pkgWriter * CApplication::Open7pkgWriter(const wchar_t * filename, const wchar_t * password)
{
	return pw_open(filename, password);
}

void CApplication::Close7pkgWriter(IC7pkgWriter * pw)
{
	pw_close(pw);
}
