#include "nge_timer.h"
#include "RbSinCore.h"
#include "sin_common.h"
#include "sin_app.h"

VALUE rb_mFrame;
VALUE rb_mSin;
VALUE rb_eSinError;
VALUE rb_mGraphics;

VALUE MRbSinCore::init()
{
	if (GetAppPtr()->InitVideo())
		return Qfalse;

	return Qtrue;
}

VALUE MRbSinCore::quit()
{
	GetAppPtr()->Quit();
	return Qnil;
}

VALUE MRbSinCore::update()
{
	GetAppPtr()->GraphicsUpdate();
	return Qnil;
}

VALUE MRbSinCore::wait(int argc, VALUE duration)
{
	SafeFixnumValue(duration);
	int dt = FIX2INT(duration);
	do
	{
		update();
		dt--;
	} while (dt);
	return Qnil;
}

//VALUE MRbSinCore::snap_to_bitmap()
//{
//	VALUE __argv[] = {INT2FIX(2), INT2FIX(2)};
//	VALUE bitmap = rb_class_new_instance(2, __argv, rb_cBitmap);
//	RbBitmap* pRbBmp = (RbBitmap*)DATA_PTR(bitmap);
//	if (RbBitmap::ScreenToBitmap(pRbBmp->GetBitmapPtr()))
//		return bitmap;
//	else
//	{
//		delete pRbBmp;
//		bitmap = NULL;
//		return Qnil;
//	}
//}

VALUE MRbSinCore::peek_message()
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

VALUE MRbSinCore::get_real_fps()
{
	return rb_float_new(GetRealFps());
}

VALUE MRbSinCore::get_hwnd()
{
	return LONG2FIX(GetAppPtr()->GetMainHwnd());
}

VALUE MRbSinCore::get_title()
{
	return rb_str_new2(Kconv::UnicodeToUTF8(GetAppPtr()->GetTitle()));
}

VALUE MRbSinCore::set_title(int argc, VALUE title)
{
	SafeStringValue(title);

	wchar_t* tempTitle = Kconv::UTF8ToUnicode(RSTRING_PTR(title));
	//wcscpy_s(GetAppPtr()->, tempTitle);
	GetAppPtr()->SetTitle(tempTitle);
	return Qnil;
}

VALUE MRbSinCore::get_width()
{
	return rb_int_new(GetAppPtr()->GetFrameWidth());
}

VALUE MRbSinCore::set_width(int argc, VALUE width)
{
	/*if (inited)
		rb_raise(rb_eRuntimeError, "NGE has been inited");*/
	SafeFixnumValue(width);
	//GetFrmStructPtr()->m_screen_width = FIX2INT(width);
	GetAppPtr()->m_frm_struct.m_screen_width = FIX2INT(width);
	return Qnil;
}

VALUE MRbSinCore::get_height()
{
	return rb_int_new(GetAppPtr()->m_frm_struct.m_screen_height);
}

VALUE MRbSinCore::set_height(int argc, VALUE height)
{
	/*if (inited)
		rb_raise(rb_eRuntimeError, "NGE has been inited");*/
	SafeFixnumValue(height);
	//GetFrmStructPtr()->m_screen_height = FIX2INT(height);
	GetAppPtr()->m_frm_struct.m_screen_height = FIX2INT(height);
	return Qnil;
}

VALUE MRbSinCore::get_fs_start()
{
	return C2RbBool(GetAppPtr()->m_frm_struct.m_fullscreen_start);
}

VALUE MRbSinCore::set_fs_start(int argc, VALUE fs_start)
{
	GetAppPtr()->m_frm_struct.m_fullscreen_start = RTEST(fs_start);
	return Qnil;
}

VALUE MRbSinCore::get_forbid_fs()
{
	return C2RbBool(GetAppPtr()->m_frm_struct.m_forbid_fullscreen);
}

VALUE MRbSinCore::set_forbid_fs(int argc, VALUE forbid_fs)
{
	GetAppPtr()->m_frm_struct.m_forbid_fullscreen = RTEST(forbid_fs);
	return Qnil;
}

VALUE MRbSinCore::get_forbid_switch()
{
	return C2RbBool(GetAppPtr()->m_frm_struct.m_forbid_switch);
}

VALUE MRbSinCore::set_forbid_switch(int argc, VALUE forbid_switch)
{
	GetAppPtr()->m_frm_struct.m_forbid_switch = RTEST(forbid_switch);
	return Qnil;
}

VALUE MRbSinCore::set_width_and_height(int argc, VALUE width, VALUE height)
{
	SafeFixnumValue(width);
	SafeFixnumValue(height);

	GetAppPtr()->m_frm_struct.m_screen_width = FIX2INT(width);
	GetAppPtr()->m_frm_struct.m_screen_height = FIX2INT(height);
	return Qnil;
}

void MRbSinCore::InitLibrary()
{
	rb_mSin = rb_define_module("SINRGE2");
		
	rb_define_const(rb_mSin, "SINRGE2_COPYRIGHT",		rb_str_freeze(rb_str_new2(SIN_COPYRIGHT)));
	rb_define_const(rb_mSin, "SINRGE2_DESCRIPTION",		rb_str_freeze(rb_str_new2(SIN_DESCRIPTION)));
	rb_define_const(rb_mSin, "SINRGE2_ENGINE",			rb_str_freeze(rb_str_new2(SIN_ENGINE)));
	rb_define_const(rb_mSin, "SINRGE2_RELEASE_DATE",	rb_str_freeze(rb_str_new2(SIN_RELEASE_DATE)));
	rb_define_const(rb_mSin, "SINRGE2_THANKS",			rb_str_freeze(rb_str_new2(SIN_THANKS)));
	rb_define_const(rb_mSin, "SINRGE2_VERSION",			rb_str_freeze(rb_str_new2(SIN_VERSION)));
	
	rb_eSinError = rb_define_class_under(rb_mSin, "SINRGE2Error", rb_eStandardError);

	rb_mGraphics = rb_define_module_under(rb_mSin, "Graphics");

	rb_define_singleton_method(rb_mSin, "init", RbFunc(init), 0);
	rb_define_singleton_method(rb_mSin, "quit", RbFunc(quit), 0);
	
	rb_define_module_function(rb_mGraphics, "update", RbFunc(update), 0);
	rb_define_module_function(rb_mGraphics, "wait", RbFunc(wait), 1);
	rb_define_module_function(rb_mGraphics, "width", RbFunc(get_width), 0);
	rb_define_module_function(rb_mGraphics, "height", RbFunc(get_height), 0);
	//rb_define_module_function(rb_mGraphics, "snap_to_bitmap", RbFunc(snap_to_bitmap), 0);

	rb_mFrame = rb_define_module_under(rb_mSin, "Frame");
	//rb_iv_set(rb_mFrame,"__title__", rb_str_freeze(rb_str_new2(SIN_DEFAULT_TITLE_NAME)));

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
	//rb_define_module_function(rb_mSin, "limit_fps", RbFunc(limit_fps), -1);
	rb_define_module_function(rb_mSin, "real_fps", RbFunc(get_real_fps), 0);
	//rb_define_module_function(rb_mSin, "show_fps", RbFunc(show_fps), 0);
	rb_define_module_function(rb_mSin, "peek_message", RbFunc(peek_message), 0);
}

//namespace Sin
//{
//	void InitRbFrame()
//	{
//		bind_frame();
//	}
//}