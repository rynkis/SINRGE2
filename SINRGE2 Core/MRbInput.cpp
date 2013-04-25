/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Moudle Input
*/
#include "MRbInput.h"
#include "RbExport.h"
#include "sin_app.h"

VALUE mInput;

static int				mouse_posX				= 0;
static int				mouse_posY				= 0;
static bool				in_screen				= false;

namespace
{
	static int			s_iRepeatCount[256]		= { 0, };
	static int			s_iLastRepeatCount[256]	= { 0, };
	static bool			s_bRepeatBool[256]		= { false, };

	static const int	MAGIC_COUNT_PRESS		= 0;
	static const int	MAGIC_COUNT_TRIGGER		= 1;
	static const int	MAGIC_COUNT_REPEAT		= 2;
	static const int	MAGIC_REPEAT_DELAY		= 24;

	inline static bool isPress(int vk)
	{
		return s_iRepeatCount[vk] > MAGIC_COUNT_PRESS;
	}

	inline static bool isTrigger(int vk)
	{
		return s_iRepeatCount[vk] == MAGIC_COUNT_TRIGGER;
	}

	void update_vk_states()
	{
		SHORT vk_state = 0;

		for (int vk = 0; vk < 256; ++vk)
		{
			///<	获取状态
			vk_state = (MRbInput::OnFocus() ? GetAsyncKeyState(vk) : 0);
			///<	记录上一帧状态
			s_iLastRepeatCount[vk] = s_iRepeatCount[vk];
			///<	更新状态
			if ((vk_state & 0x8000) != 0)
			{
				if (++s_iRepeatCount[vk] == MAGIC_COUNT_REPEAT)
					++s_iRepeatCount[vk];
			}
			else
				s_iRepeatCount[vk] = 0;

			///<	更新repeat状态
			s_bRepeatBool[vk] = (isPress(vk) && (isTrigger(vk) || s_iRepeatCount[vk] > MAGIC_REPEAT_DELAY));
			if (s_bRepeatBool[vk] && s_iRepeatCount[vk] > MAGIC_COUNT_REPEAT)
				s_iRepeatCount[vk] = MAGIC_COUNT_REPEAT;
		}
	}

	int vk_is_press(int vk)
	{
		return (int)isPress(vk);
	}

	int vk_is_trigger(int vk)
	{
		return (int)isTrigger(vk);
	}

	int vk_is_repeat(int vk)
	{
		return (int)s_bRepeatBool[vk];
	}

	int vk_is_click(int vk)
	{
		return (int)(s_iLastRepeatCount[vk] > MAGIC_COUNT_PRESS && s_iRepeatCount[vk] == MAGIC_COUNT_PRESS);
	}
}

/*
**	call-seq:
**		SIN::Input.update	-> nil
**
*/
VALUE MRbInput::update_input()
{
	HWND hwnd = GetAppPtr()->GetMainHwnd();
	if (hwnd)
	{
		POINT point;
		if (GetCursorPos(&point))
		{
			WINDOWINFO wi;
			if (GetWindowInfo(hwnd, &wi))
			{
				mouse_posX = point.x - wi.rcClient.left;
				mouse_posY = point.y - wi.rcClient.top;
				in_screen = (point.x >= wi.rcClient.left) && (point.y >= wi.rcClient.top)	\
					&& (point.x <= wi.rcClient.right) && (point.y <= wi.rcClient.bottom);
			}
		}
	}
	update_vk_states();
	return Qnil;
}

/*
**	call-seq:
**		SIN::Input.mouse_over?	-> bool
**
*/
VALUE MRbInput::mouse_over()
{
	return C2RbBool(in_screen);
}

/*
**	call-seq:
**		SIN::Input.press?(vk)	-> bool
**
*/
VALUE MRbInput::is_press(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(vk_is_press(FIX2INT(key)));
}

/*
**	call-seq:
**		SIN::Input.trigger?(vk)	-> bool
**
*/
VALUE MRbInput::is_trigger(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(vk_is_trigger(FIX2INT(key)));
}

/*
**	call-seq:
**		SIN::Input.repeat?(vk)	-> bool
**
*/
VALUE MRbInput::is_repeat(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(vk_is_repeat(FIX2INT(key)));
}

/*
**	call-seq:
**		SIN::Input.click?(vk)	-> bool
**
*/
VALUE MRbInput::is_click(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(vk_is_click(FIX2INT(key)) && in_screen);
}

/*
**	call-seq:
**		SIN::Input.dir4	-> fixnum
**
*/
VALUE MRbInput::get_dir4()
{
	if (vk_is_press(VK_DOWN) || vk_is_press(VK_NUMPAD2))
		return INT2FIX(2);
	if (vk_is_press(VK_LEFT) || vk_is_press(VK_NUMPAD4))
		return INT2FIX(4);
	if (vk_is_press(VK_RIGHT) || vk_is_press(VK_NUMPAD6))
		return INT2FIX(6);
	if (vk_is_press(VK_UP) || vk_is_press(VK_NUMPAD8))
		return INT2FIX(8);
	
	return RUBY_0;
}

/*
**	call-seq:
**		SIN::Input.dir8	-> fixnum
**
*/
VALUE MRbInput::get_dir8()
{
	if (vk_is_press(VK_LEFT))
	{
		if (vk_is_press(VK_DOWN))
			return INT2FIX(1);
		if (vk_is_press(VK_UP))
			return INT2FIX(7);
		return INT2FIX(4);
	}
	if (vk_is_press(VK_DOWN))
	{
		if (vk_is_press(VK_LEFT))
			return INT2FIX(1);
		if (vk_is_press(VK_RIGHT))
			return INT2FIX(3);
		return INT2FIX(2);
	}
	if (vk_is_press(VK_RIGHT))
	{
		if (vk_is_press(VK_DOWN))
			return INT2FIX(3);
		if (vk_is_press(VK_UP))
			return INT2FIX(9);
		return INT2FIX(6);
	}
	if (vk_is_press(VK_UP))
	{
		if (vk_is_press(VK_LEFT))
			return INT2FIX(7);
		if (vk_is_press(VK_RIGHT))
			return INT2FIX(9);
		return INT2FIX(8);
	}
	if (vk_is_press(VK_NUMPAD1))
		return INT2FIX(1);
	if (vk_is_press(VK_NUMPAD3))
		return INT2FIX(3);
	if (vk_is_press(VK_NUMPAD7))
		return INT2FIX(7);
	if (vk_is_press(VK_NUMPAD9))
		return INT2FIX(9);
	
	return get_dir4();
}
/*
**	call-seq:
**		SIN::Input.show_mouse(show)	-> nil
**
*/
VALUE MRbInput::show_mouse(int argc, VALUE show)
{
	HideMouse(!RTEST(show));
	return Qnil;
}

/*
**	call-seq:
**		SIN::Input.on_focus?	-> bool
**
*/
VALUE MRbInput::on_focus()
{
	return C2RbBool(OnFocus());
}

/*
**	call-seq:
**		SIN::Input.mouse_wheel	-> fixnum
**
*/
VALUE MRbInput::mouse_wheel()
{
	return INT2FIX(MouseWheel());
}

/*
**	call-seq:
**		SIN::Input.dblclk?(vk)	-> bool
**
*/
VALUE MRbInput::mouse_dblclk(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(MouseDblClk(FIX2INT(key)) && in_screen);
}

/*
**	call-seq:
**		SIN::Input.mouse_pos	-> array
**
*/
VALUE MRbInput::get_mouse_pos()
{
	VALUE ary;

	ary = rb_ary_new2(2);
	rb_ary_push(ary, INT2FIX(mouse_posX));
	rb_ary_push(ary, INT2FIX(mouse_posY));
	rb_ary_freeze(ary);

	return ary;
}

void MRbInput::InitLibrary()
{
	mInput = rb_define_module_under(rb_mSin, "Input");

	rb_define_module_function(mInput, "show_mouse", RbFunc(show_mouse), 1);
	rb_define_module_function(mInput, "on_focus?", RbFunc(on_focus), 0);
	rb_define_module_function(mInput, "mouse_wheel", RbFunc(mouse_wheel), 0);
	rb_define_module_function(mInput, "dblclk?", RbFunc(mouse_dblclk), 1);
	rb_define_module_function(mInput, "mouse_over?", RbFunc(mouse_over), 0);
	rb_define_module_function(mInput, "mouse_pos", RbFunc(get_mouse_pos), 0);
	rb_define_module_function(mInput, "update", RbFunc(update_input), 0);
	rb_define_module_function(mInput, "press?", RbFunc(is_press), 1);
	rb_define_module_function(mInput, "trigger?", RbFunc(is_trigger), 1);
	rb_define_module_function(mInput, "repeat?", RbFunc(is_repeat), 1);
	rb_define_module_function(mInput, "click?", RbFunc(is_click), 1);
	rb_define_module_function(mInput, "dir4", RbFunc(get_dir4), 0);
	rb_define_module_function(mInput, "dir8", RbFunc(get_dir8), 0);
}