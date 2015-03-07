/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Image Header
*/
#ifndef __SIN_IMAGE_H__
#define __SIN_IMAGE_H__

#ifdef WIN32
#pragma once
#endif

#include "hge.h"

typedef struct tagImage
{
	hgeQuad		quad;
	int			width;	/**< Í¼Æ¬ÌùÍ¼¿í */
	int			height;	/**< Í¼Æ¬ÌùÍ¼¸ß */
	int			owidth;    /**< Í¼Æ¬Ô­¿í */
	int			oheight;   /**< Í¼Æ¬Ô­¸ß */
	//float		rcentrex; /**< Í¼ÏñÖÐÐÄx */
	//float		rcentrey; /**< Í¼ÏñÖÐÐÄy */
} image_t, * image_p;

typedef struct {
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BMPFILEHEADER_T;

typedef struct{
	DWORD      biSize;
	LONG       biWidth;
	LONG       biHeight;
	WORD       biPlanes;
	WORD       biBitCount;
	DWORD      biCompression;
	DWORD      biSizeImage;
	LONG       biXPelsPerMeter;
	LONG       biYPelsPerMeter;
	DWORD      biClrUsed;
	DWORD      biClrImportant;
} BMPINFOHEADER_T;

typedef struct{
	BMPFILEHEADER_T bfh;
	BMPINFOHEADER_T bih;
} BMPHEADER_T;

#endif //__SIN_IMAGE_H__