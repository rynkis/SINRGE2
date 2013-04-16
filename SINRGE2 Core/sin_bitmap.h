#ifndef __SIN_BITMAP_H__
#define __SIN_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "hge.h"
//#include "SINRGE2.h"

typedef struct tagBitmap{
	hgeQuad		quad;
	int			width;   /**< Í¼Æ¬ÌùÍ¼¿í */
	int			height;  /**< Í¼Æ¬ÌùÍ¼¸ß */
	int			owidth;    /**< Í¼Æ¬Ô­¿í */
	int			oheight;   /**< Í¼Æ¬Ô­¸ß */
	//float		rcentrex; /**< Í¼ÏñÖÐÐÄx */
	//float		rcentrey; /**< Í¼ÏñÖÐÐÄy */
}bitmap_t, *bitmap_p;

//typedef struct pix24_t{
//        BYTE g;
//        BYTE b;
//        BYTE r;
//} *m_bits24;
//
//typedef struct pix32_t{
//        BYTE g;
//        BYTE b;
//        BYTE r;
//} *m_bits32;

#endif //__SIN_BITMAP_H__