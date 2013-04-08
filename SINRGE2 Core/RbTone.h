#ifndef __C_RGE_TONE_H__
#define __C_RGE_TONE_H__

#ifdef WIN32
#pragma once
#endif

#include "RbColor.h"

using namespace Sin;

class RbTone : public RbColor
{
public:
	static void		InitLibrary();
};

#endif