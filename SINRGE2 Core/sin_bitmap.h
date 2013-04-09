#ifndef __SIN_BITMAP_H__
#define __SIN_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "SINRGE2.h"

typedef struct tagBitmap{
	hgeQuad		quad;
	int			width;    /**< Í¼Æ¬Ô­¿í */
	int			height;   /**< Í¼Æ¬Ô­¸ß */
	int			texw;     /**< Í¼Æ¬ÌùÍ¼¿í */
	int			texh;     /**< Í¼Æ¬ÌùÍ¼¸ß */
	float		rcentrex; /**< Í¼ÏñÖÐÐÄx */
	float		rcentrey; /**< Í¼ÏñÖÐÐÄy */
}bitmap_t, *bitmap_p;

#endif //__SIN_BITMAP_H__