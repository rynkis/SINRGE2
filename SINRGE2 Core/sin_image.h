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
	WORD bfType;
	BYTE bfSize8;
	BYTE bfSize16;
	BYTE bfSize24;
	BYTE bfSize32;
	WORD bfReserved1;

	WORD bfReserved2;
	BYTE bfOffBits8;
	BYTE bfOffBits16;
	BYTE bfOffBits24;
	BYTE bfOffBits32;
	BYTE biSize8;
	BYTE biSize16;

	BYTE biSize24;
	BYTE biSize32;
	BYTE biWidth8;
	BYTE biWidth16;
	BYTE biWidth24;
	BYTE biWidth32;
	BYTE biHeight8;
	BYTE biHeight16;

	BYTE biHeight24;
	BYTE biHeight32;
	WORD biPlanes;
	WORD biBitCount;
	BYTE biCompression8;
	BYTE biCompression16;

	BYTE biCompression24;
	BYTE biCompression32;
	BYTE biSizeImage8;
	BYTE biSizeImage16;
	BYTE biSizeImage24;
	BYTE biSizeImage32;
	BYTE biXPelsPerMeter8;
	BYTE biXPelsPerMeter16;

	BYTE biXPelsPerMeter24;
	BYTE biXPelsPerMeter32;
	BYTE biYPelsPerMeter8;
	BYTE biYPelsPerMeter16;
	BYTE biYPelsPerMeter24;
	BYTE biYPelsPerMeter32;
	BYTE biClrUsed8;
	BYTE biClrUsed16;

	BYTE biClrUsed24;
	BYTE biClrUsed32;
	BYTE biClrImportant8;
	BYTE biClrImportant16;
	BYTE biClrImportant24;
	BYTE biClrImportant32;
} BMPHEADER_T;

#endif //__SIN_IMAGE_H__