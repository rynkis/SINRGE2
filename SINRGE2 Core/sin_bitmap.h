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
	//int			texw;     /**< Í¼Æ¬ÌùÍ¼¿í */
	//int			texh;     /**< Í¼Æ¬ÌùÍ¼¸ß */
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