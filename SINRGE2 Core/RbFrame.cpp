#include "RbExport.h"
#include "SINRGE2.h"
#include "nge_timer.h"

using namespace Sin;

static VALUE rb_mFrame;

//static bool		inited			= false;
//static char*	gameTitle		= "SINRGE2";
static wchar_t*	gameTitleW		= L"SINRGE2";
static wchar_t	titleWithFps[MAX_PATH];
//fps count
static uint32_t m_frame = 0;
static uint32_t m_t0 = 0;
static char inited = 0;
static int cacheid = 0;
static uint8_t tex_ret = 0;
static nge_timer* fps_timer;
static float lastFps;

float GetRealFps()
{
	int t;
	float seconds;
	float realFps;
	m_frame++;
	t = nge_get_tick();
	if ( (t - m_t0) >= 1000) {
		seconds = (t - m_t0) / 1000.0;
		lastFps = realFps = m_frame / seconds;
		m_t0 = t;
		m_frame = 0;
		return realFps;
	}
	return lastFps;
}

void LimitFps(uint32_t limit)
{
	static uint32_t current_ticks,target_ticks,frame_count,last_ticks,the_delay;
	static float rate_ticks;

	if (fps_timer == NULL) {
		fps_timer = nge_timer_create();
		fps_timer->start(fps_timer);
		current_ticks = 0;
		target_ticks  = 0;
		last_ticks    = 0;
		frame_count   = 0;
		rate_ticks    = 1000.0f/limit;
		return;
	}
	frame_count++;
	current_ticks = fps_timer->get_ticks(fps_timer);
	target_ticks = last_ticks + (uint32_t) ((float) frame_count * rate_ticks);

	if (current_ticks <= target_ticks) {
		the_delay = target_ticks - current_ticks;
		RB_SLEEP(the_delay);
	} else {
		frame_count = 0;
		last_ticks = fps_timer->get_ticks(fps_timer);
	}
}

//char* Sin::GetTitle()
//{
//	return gameTitle;
//}

wchar_t* Sin::GetTitleW()
{
	return gameTitleW;
}

static VALUE peek_message()
{
	MSG msg;
	if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return Qfalse;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return Qtrue;
}

static VALUE get_real_fps()
{
	return rb_float_new(GetRealFps());
}

static VALUE limit_fps(int argc, VALUE* argv)
{
    VALUE rint;

    rb_scan_args(argc, argv, "01", &rint);
	LimitFps(NIL_P(rint) ? 60 : NUM2INT(rint));

    return Qnil;
}

static VALUE show_fps()
{
	wsprintfW(titleWithFps, L"%s - %d FPS", gameTitleW, (int)GetRealFps());
	SetWindowTextW(GetFrmStructPtr()->m_hwnd, titleWithFps);
	return Qnil;
}

static VALUE get_hwnd()
{
	return LONG2FIX(GetFrmStructPtr()->m_hwnd);
}

static VALUE get_title()
{
	int len = WideCharToMultiByte(CP_UTF8, 0, gameTitleW, -1, NULL, 0, NULL, NULL);
	char* tempTitle = (char*)malloc(len);
	WideCharToMultiByte(CP_UTF8, 0, gameTitleW, -1, tempTitle, len, NULL, NULL);
	return rb_str_new2(tempTitle);
}

static VALUE set_title(int argc, VALUE title)
{
	/*if (inited)
		rb_raise(rb_eRuntimeError, "NGE has been inited");*/
	SafeStringValue(title);

	char* tempTitle = RSTRING_PTR(title);
	
	int len = MultiByteToWideChar(CP_UTF8, 0, tempTitle, -1, NULL, 0);
	gameTitleW = (wchar_t*)malloc(len);
	MultiByteToWideChar(CP_UTF8, 0, tempTitle, -1, gameTitleW, len);

	/*len = WideCharToMultiByte(CP_ACP, 0, gameTitleW, -1, NULL, 0, NULL, NULL);
	gameTitle = (char*)malloc(len);
	WideCharToMultiByte(CP_ACP, 0, gameTitleW, -1, gameTitle, len, NULL, NULL);*/
	return Qnil;
}

static VALUE get_width()
{
	return rb_int_new(GetFrmStructPtr()->m_screen_width);
}

static VALUE set_width(int argc, VALUE width)
{
	/*if (inited)
		rb_raise(rb_eRuntimeError, "NGE has been inited");*/
	SafeFixnumValue(width);
	GetFrmStructPtr()->m_screen_width = FIX2INT(width);
	return Qnil;
}

static VALUE get_height()
{
	return rb_int_new(GetFrmStructPtr()->m_screen_height);
}

static VALUE set_height(int argc, VALUE height)
{
	/*if (inited)
		rb_raise(rb_eRuntimeError, "NGE has been inited");*/
	SafeFixnumValue(height);
	GetFrmStructPtr()->m_screen_height = FIX2INT(height);
	return Qnil;
}

static VALUE get_fs_start()
{
	return C2RbBool(GetFrmStructPtr()->m_fullscreen_start);
}

static VALUE set_fs_start(int argc, VALUE fs_start)
{
	GetFrmStructPtr()->m_fullscreen_start = RTEST(fs_start);
	return Qnil;
}

static VALUE get_forbid_fs()
{
	return C2RbBool(GetFrmStructPtr()->m_forbid_fullscreen);
}

static VALUE set_forbid_fs(int argc, VALUE forbid_fs)
{
	GetFrmStructPtr()->m_forbid_fullscreen = RTEST(forbid_fs);
	return Qnil;
}

static VALUE get_forbid_switch()
{
	return C2RbBool(GetFrmStructPtr()->m_forbid_switch);
}

static VALUE set_forbid_switch(int argc, VALUE forbid_switch)
{
	GetFrmStructPtr()->m_forbid_switch = RTEST(forbid_switch);
	return Qnil;
}

static VALUE set_width_and_height(int argc, VALUE width, VALUE height)
{
	SafeFixnumValue(width);
	SafeFixnumValue(height);

	GetFrmStructPtr()->m_screen_width = FIX2INT(width);
	GetFrmStructPtr()->m_screen_height = FIX2INT(height);
	return Qnil;
}

void bind_frame()
{
	rb_mFrame = rb_define_module_under(rb_mSin, "Frame");

	rb_iv_set(rb_mFrame,"__title__", rb_str_freeze(rb_str_new2(SIN_DEFAULT_TITLE_NAME)));

	rb_define_module_function(rb_mFrame, "hwnd",	RbFunc(get_hwnd), 0);
	rb_define_module_function(rb_mFrame, "title",	RbFunc(get_title), 0);
	rb_define_module_function(rb_mFrame, "title=",	RbFunc(set_title), 1);
	rb_define_module_function(rb_mFrame, "width",	RbFunc(get_width), 0);
	rb_define_module_function(rb_mFrame, "width=",	RbFunc(set_width), 1);
	rb_define_module_function(rb_mFrame, "height",	RbFunc(get_height), 0);
	rb_define_module_function(rb_mFrame, "height=",	RbFunc(set_height), 1);

	rb_define_module_function(rb_mFrame, "fullscreen_start",	RbFunc(get_fs_start), 0);
	rb_define_module_function(rb_mFrame, "fullscreen_start=",	RbFunc(set_fs_start), 1);
	rb_define_module_function(rb_mFrame, "forbid_fullscreen",	RbFunc(get_forbid_fs), 0);
	rb_define_module_function(rb_mFrame, "forbid_fullscreen=",	RbFunc(set_forbid_fs), 1);
	rb_define_module_function(rb_mFrame, "forbid_switch",		RbFunc(get_forbid_switch), 0);
	rb_define_module_function(rb_mFrame, "forbid_switch=",		RbFunc(set_forbid_switch), 1);

	rb_define_module_function(rb_mFrame, "set_size",	RbFunc(set_width_and_height), 2);

	//rb_define_module_function(rb_mFrame, "peek_message", RbFunc(peek_message), 0);
	rb_define_module_function(rb_mSin, "limit_fps", RbFunc(limit_fps), -1);
	rb_define_module_function(rb_mSin, "real_fps", RbFunc(get_real_fps), 0);
	rb_define_module_function(rb_mSin, "show_fps", RbFunc(show_fps), 0);
	rb_define_module_function(rb_mSin, "peek_message", RbFunc(peek_message), 0);
}

namespace Sin
{
	void InitRbFrame()
	{
		bind_frame();
	}
}