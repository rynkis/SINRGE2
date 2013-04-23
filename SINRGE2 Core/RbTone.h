#ifndef __RB_TONE_H__
#define __RB_TONE_H__

#ifdef WIN32
#pragma once
#endif

#include "RbColor.h"

class RbTone : public RbColor
{
public:
	static void		InitLibrary();

protected:
	virtual VALUE	initialize(int argc, VALUE *argv, VALUE obj);	//	initialize
};

#endif