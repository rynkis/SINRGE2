/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Moudle SINRGE2
*/
#ifndef __MODULE_RB_SIN_CORE_H__
#define __MODULE_RB_SIN_CORE_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"

struct MRbSinCore
{
	static void	InitLibrary();

	static void Freeze();

	static void Transition(int duration, const wchar_t *filename, float vague);

	static VALUE init();
	static VALUE quit();
	static VALUE stop();
	
	static VALUE update();
	static VALUE wait(int argc, VALUE duration);
	static VALUE fadeout(int argc, VALUE duration);
	static VALUE fadein(int argc, VALUE duration);
	static VALUE freeze();
	static VALUE transition(int argc, VALUE *argv);
	static VALUE snap_to_bitmap();
	//static VALUE frame_reset();
	static VALUE get_width();
	static VALUE get_height();
	static VALUE resize_screen(int argc, VALUE width, VALUE height);
	static VALUE get_frame_rate();
	static VALUE set_frame_rate(int argc, VALUE framecount);
	static VALUE get_framecount();
	static VALUE set_framecount(int argc, VALUE framecount);
	static VALUE get_brightness();
	static VALUE set_brightness(int argc, VALUE brightness);

	static VALUE get_hwnd();
	static VALUE get_title();
	static VALUE set_title(int argc, VALUE title);
	
	static VALUE set_start_width(int argc, VALUE width);
	static VALUE set_start_height(int argc, VALUE height);
	static VALUE get_fs_start();
	static VALUE set_fs_start(int argc, VALUE fs_start);
	static VALUE get_forbid_fs();
	static VALUE set_forbid_fs(int argc, VALUE forbid_fs);
	static VALUE get_forbid_switch();
	static VALUE set_forbid_switch(int argc, VALUE forbid_switch);


	static VALUE get_real_fps();
	static VALUE peek_message();
};

#endif // __MODULE_RB_SIN_CORE_H__