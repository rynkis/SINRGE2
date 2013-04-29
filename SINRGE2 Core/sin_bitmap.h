/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Bitmap Header
*/
#ifndef __SIN_BITMAP_H__
#define __SIN_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "hge.h"

typedef struct tagBitmap
{
	hgeQuad		quad;
	int			width;	/**< Í¼Æ¬ÌùÍ¼¿í */
	int			height;	/**< Í¼Æ¬ÌùÍ¼¸ß */
	int			owidth;    /**< Í¼Æ¬Ô­¿í */
	int			oheight;   /**< Í¼Æ¬Ô­¸ß */
	//float		rcentrex; /**< Í¼ÏñÖÐÐÄx */
	//float		rcentrey; /**< Í¼ÏñÖÐÐÄy */
} bitmap_t, *bitmap_p;

#endif //__SIN_BITMAP_H__