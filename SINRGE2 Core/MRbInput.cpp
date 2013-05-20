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

	/*int vk_is_press(int vk)
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
	}*/

	bool vk_is_click(int vk)
	{
		return s_iLastRepeatCount[vk] > MAGIC_COUNT_PRESS && s_iRepeatCount[vk] == MAGIC_COUNT_PRESS;
	}
}

/*
**	call-seq:
**		SINRGE2::Input.update	-> nil
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
**		SINRGE2::Input.mouse_over?	-> bool
**
*/
VALUE MRbInput::mouse_over()
{
	return C2RbBool(in_screen);
}

/*
**	call-seq:
**		SINRGE2::Input.press?(vk)	-> bool
**
*/
VALUE MRbInput::is_press(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(isPress(FIX2INT(key)));
}

/*
**	call-seq:
**		SINRGE2::Input.trigger?(vk)	-> bool
**
*/
VALUE MRbInput::is_trigger(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(isTrigger(FIX2INT(key)));
}

/*
**	call-seq:
**		SINRGE2::Input.repeat?(vk)	-> bool
**
*/
VALUE MRbInput::is_repeat(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(s_bRepeatBool[FIX2INT(key)]);
}

/*
**	call-seq:
**		SINRGE2::Input.click?(vk)	-> bool
**
*/
VALUE MRbInput::is_click(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(vk_is_click(FIX2INT(key)) && in_screen);
}

/*
**	call-seq:
**		SINRGE2::Input.dir4	-> fixnum
**
*/
VALUE MRbInput::get_dir4()
{
	if (isPress(VK_DOWN) || isPress(VK_NUMPAD2))
		return INT2FIX(2);
	if (isPress(VK_LEFT) || isPress(VK_NUMPAD4))
		return INT2FIX(4);
	if (isPress(VK_RIGHT) || isPress(VK_NUMPAD6))
		return INT2FIX(6);
	if (isPress(VK_UP) || isPress(VK_NUMPAD8))
		return INT2FIX(8);
	
	return RUBY_0;
}

/*
**	call-seq:
**		SINRGE2::Input.dir8	-> fixnum
**
*/
VALUE MRbInput::get_dir8()
{
	if (isPress(VK_LEFT))
	{
		if (isPress(VK_DOWN))
			return INT2FIX(1);
		if (isPress(VK_UP))
			return INT2FIX(7);
		return INT2FIX(4);
	}
	if (isPress(VK_DOWN))
	{
		if (isPress(VK_LEFT))
			return INT2FIX(1);
		if (isPress(VK_RIGHT))
			return INT2FIX(3);
		return INT2FIX(2);
	}
	if (isPress(VK_RIGHT))
	{
		if (isPress(VK_DOWN))
			return INT2FIX(3);
		if (isPress(VK_UP))
			return INT2FIX(9);
		return INT2FIX(6);
	}
	if (isPress(VK_UP))
	{
		if (isPress(VK_LEFT))
			return INT2FIX(7);
		if (isPress(VK_RIGHT))
			return INT2FIX(9);
		return INT2FIX(8);
	}
	if (isPress(VK_NUMPAD1))
		return INT2FIX(1);
	if (isPress(VK_NUMPAD3))
		return INT2FIX(3);
	if (isPress(VK_NUMPAD7))
		return INT2FIX(7);
	if (isPress(VK_NUMPAD9))
		return INT2FIX(9);
	
	return get_dir4();
}

/*
**	call-seq:
**		SINRGE2::Input.on_focus?	-> bool
**
*/
VALUE MRbInput::on_focus()
{
	return C2RbBool(OnFocus());
}

/*
**	call-seq:
**		SINRGE2::Input.mouse_wheel	-> fixnum
**
*/
VALUE MRbInput::mouse_wheel()
{
	return INT2FIX(MouseWheel());
}

/*
**	call-seq:
**		SINRGE2::Input.dblclk?(vk)	-> bool
**
*/
VALUE MRbInput::mouse_dblclk(int argc, VALUE key)
{
	SafeFixnumValue(key);
	return C2RbBool(MouseDblClk(FIX2INT(key)) && in_screen);
}

/*
**	call-seq:
**		SINRGE2::Input.mouse_pos	-> array
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

VALUE MRbInput::get_character()
{
	bool capital = (GetKeyState(VK_CAPITAL) & 0x01) == 1 || isPress(VK_LSHIFT) || isPress(VK_RSHIFT);

	//if (isTrigger(VK_SPACE))		return rb_str_new2(" ");

	if (isTrigger(VK_A))			return rb_str_new2(capital ? "A" : "a");
	if (isTrigger(VK_B))			return rb_str_new2(capital ? "B" : "b");
	if (isTrigger(VK_C))			return rb_str_new2(capital ? "C" : "c");
	if (isTrigger(VK_D))			return rb_str_new2(capital ? "D" : "d");
	if (isTrigger(VK_E))			return rb_str_new2(capital ? "E" : "e");
	if (isTrigger(VK_F))			return rb_str_new2(capital ? "F" : "f");
	if (isTrigger(VK_G))			return rb_str_new2(capital ? "G" : "g");
	if (isTrigger(VK_H))			return rb_str_new2(capital ? "H" : "h");
	if (isTrigger(VK_I))			return rb_str_new2(capital ? "I" : "i");
	if (isTrigger(VK_J))			return rb_str_new2(capital ? "J" : "j");
	if (isTrigger(VK_K))			return rb_str_new2(capital ? "K" : "k");
	if (isTrigger(VK_L))			return rb_str_new2(capital ? "L" : "l");
	if (isTrigger(VK_M))			return rb_str_new2(capital ? "M" : "m");
	if (isTrigger(VK_N))			return rb_str_new2(capital ? "N" : "n");
	if (isTrigger(VK_O))			return rb_str_new2(capital ? "O" : "o");
	if (isTrigger(VK_P))			return rb_str_new2(capital ? "P" : "p");
	if (isTrigger(VK_Q))			return rb_str_new2(capital ? "Q" : "q");
	if (isTrigger(VK_R))			return rb_str_new2(capital ? "R" : "r");
	if (isTrigger(VK_S))			return rb_str_new2(capital ? "S" : "s");
	if (isTrigger(VK_T))			return rb_str_new2(capital ? "T" : "t");
	if (isTrigger(VK_U))			return rb_str_new2(capital ? "U" : "u");
	if (isTrigger(VK_V))			return rb_str_new2(capital ? "V" : "v");
	if (isTrigger(VK_W))			return rb_str_new2(capital ? "W" : "w");
	if (isTrigger(VK_X))			return rb_str_new2(capital ? "X" : "x");
	if (isTrigger(VK_Y))			return rb_str_new2(capital ? "Y" : "y");
	if (isTrigger(VK_Z))			return rb_str_new2(capital ? "Z" : "z");
	
	if (isTrigger(VK_1))			return rb_str_new2(capital ? "!" : "1");
	if (isTrigger(VK_2))			return rb_str_new2(capital ? "@" : "2");
	if (isTrigger(VK_3))			return rb_str_new2(capital ? "#" : "3");
	if (isTrigger(VK_4))			return rb_str_new2(capital ? "$" : "4");
	if (isTrigger(VK_5))			return rb_str_new2(capital ? "%%": "5");
	if (isTrigger(VK_6))			return rb_str_new2(capital ? "^" : "6");
	if (isTrigger(VK_7))			return rb_str_new2(capital ? "&" : "7");
	if (isTrigger(VK_8))			return rb_str_new2(capital ? "*" : "8");
	if (isTrigger(VK_9))			return rb_str_new2(capital ? "(" : "9");
	if (isTrigger(VK_0))			return rb_str_new2(capital ? ")" : "0");
	
	if (isTrigger(VK_OEM_1))		return rb_str_new2(capital ? ":" : ";");
	if (isTrigger(VK_OEM_PLUS))		return rb_str_new2(capital ? "+" : "=");
	if (isTrigger(VK_OEM_COMMA))	return rb_str_new2(capital ? "<" : ",");
	if (isTrigger(VK_OEM_MINUS))	return rb_str_new2(capital ? "_" : "-");
	if (isTrigger(VK_OEM_PERIOD))	return rb_str_new2(capital ? ">" : ".");
	if (isTrigger(VK_OEM_2))		return rb_str_new2(capital ? "?" : "/");
	if (isTrigger(VK_OEM_3))		return rb_str_new2(capital ? "~" : "`");
	if (isTrigger(VK_OEM_4))		return rb_str_new2(capital ? "{" : "[");
	if (isTrigger(VK_OEM_5))		return rb_str_new2(capital ? "|" : "\\");
	if (isTrigger(VK_OEM_6))		return rb_str_new2(capital ? "}" : "]");
	if (isTrigger(VK_OEM_7))		return rb_str_new2(capital ? "\"" : "'");

	return Qnil;
}

void MRbInput::InitLibrary()
{
	mInput = rb_define_module_under(rb_mSin, "Input");

	rb_define_module_function(mInput, "on_focus?", RbFunc(on_focus), 0);
	rb_define_module_function(mInput, "click?", RbFunc(is_click), 1);
	rb_define_module_function(mInput, "dblclk?", RbFunc(mouse_dblclk), 1);
	rb_define_module_function(mInput, "mouse_wheel", RbFunc(mouse_wheel), 0);
	rb_define_module_function(mInput, "mouse_over?", RbFunc(mouse_over), 0);
	rb_define_module_function(mInput, "mouse_pos", RbFunc(get_mouse_pos), 0);
	rb_define_module_function(mInput, "update", RbFunc(update_input), 0);
	rb_define_module_function(mInput, "press?", RbFunc(is_press), 1);
	rb_define_module_function(mInput, "trigger?", RbFunc(is_trigger), 1);
	rb_define_module_function(mInput, "repeat?", RbFunc(is_repeat), 1);
	rb_define_module_function(mInput, "dir4", RbFunc(get_dir4), 0);
	rb_define_module_function(mInput, "dir8", RbFunc(get_dir8), 0);

	rb_define_module_function(mInput, "character", RbFunc(get_character), 0);
}