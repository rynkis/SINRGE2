/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Moudle Audio
*/
#ifndef _MODULE_RB_AUDIO_H_
#define _MODULE_RB_AUDIO_H_

#include "RbClassBase.h"

struct MRbAudio
{
	static void	InitLibrary();
	
	static VALUE bgm_play(int argc, VALUE * argv);
	static VALUE bgm_stop();
	static VALUE bgm_fade(int argc, VALUE time);
	
	static VALUE bgs_play(int argc, VALUE * argv);
	static VALUE bgs_stop();
	static VALUE bgs_fade(int argc, VALUE time);
	
	static VALUE me_play(int argc, VALUE * argv);
	static VALUE me_stop();
	static VALUE me_fade(int argc, VALUE time);

	static VALUE se_play(int argc, VALUE * argv);
	static VALUE se_stop();
};

#endif //_MODULE_RB_AUDIO_H_