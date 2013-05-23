/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Tone
*/
#ifndef __C_RB_TONE_H__
#define __C_RB_TONE_H__

#ifdef WIN32
#pragma once
#endif

#include "CRbColor.h"

class CRbTone : public CRbColor
{
public:
	static void		InitLibrary();

protected:
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize
};

#endif