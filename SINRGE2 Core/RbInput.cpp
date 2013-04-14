#include "RbInput.h"
#include "RbExport.h"
#include "SINRGE2.h"

using namespace Sin;

static VALUE			mInput;

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
			vk_state = (OnFocus() ? GetAsyncKeyState(vk) : 0);
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
static VALUE update_input()
{
	HWND hwnd = GetFrmStructPtr()->m_hwnd;
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
static VALUE mouse_over()
{
	return in_screen ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.press?(vk)	-> bool
**
*/
static VALUE is_press(int argv, VALUE key)
{
	SafeFixnumValue(key);
	return vk_is_press(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.trigger?(vk)	-> bool
**
*/
static VALUE is_trigger(int argv, VALUE key)
{
	SafeFixnumValue(key);
	return vk_is_trigger(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.repeat?(vk)	-> bool
**
*/
static VALUE is_repeat(int argv, VALUE key)
{
	SafeFixnumValue(key);
	return vk_is_repeat(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.click?(vk)	-> bool
**
*/
static VALUE is_click(int argv, VALUE key)
{
	SafeFixnumValue(key);
	return vk_is_click(FIX2INT(key)) ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.dir4	-> fixnum
**
*/
static VALUE get_dir4()
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
static VALUE get_dir8()
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
static VALUE show_mouse(int argv, VALUE show)
{
	HideMouse(!RTEST(show));
	return Qnil;
}

/*
**	call-seq:
**		SIN::Input.on_focus?	-> bool
**
*/
static VALUE on_focus()
{
	return OnFocus() ? Qtrue : Qfalse;
}

/*
**	call-seq:
**		SIN::Input.mouse_wheel	-> fixnum
**
*/
static VALUE mouse_wheel()
{
	return rb_int_new(MouseWheel());
}

/*
**	call-seq:
**		SIN::Input.dblclk?(vk)	-> bool
**
*/
static VALUE mouse_dblclk(int argv, VALUE key)
{
	SafeFixnumValue(key);
	return MouseDblClk(FIX2INT(key));
}

/*
**	call-seq:
**		SIN::Input.mouse_pos	-> array
**
*/
static VALUE get_mouse_pos()
{
	VALUE ary;

	ary = rb_ary_new2(2);
	rb_ary_push(ary, rb_int_new(mouse_posX));
	rb_ary_push(ary, rb_int_new(mouse_posY));
	rb_ary_freeze(ary);

	return ary;
}

static void bind_input()
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

void Sin::InitRbInput()
{
	bind_input();
}