#ifndef __SIN_IMAGE_H__
#define __SIN_IMAGE_H__
#include "SINRGE2.h"

typedef struct tagImage{
	hgeQuad		quad;
	int			w;        /**< Í¼Æ¬Ô­¿í */
	int			h;        /**< Í¼Æ¬Ô­¸ß */
	int			texw;     /**< Í¼Æ¬ÌùÍ¼¿í */
	int			texh;     /**< Í¼Æ¬ÌùÍ¼¸ß */
	float		rcentrex; /**< Í¼ÏñÖÐÐÄx */
	float		rcentrey; /**< Í¼ÏñÖÐÐÄy */
}image_t, *image_p;


image_p ImageLoad(const wchar_t* filename);

image_p ImageLoadBuf(const char* mbuf, int bsize);

image_p ImageCreate(int width, int height);

int ImageSave(image_p pimage, const wchar_t* filename, int alpha, int rle);

void ImageFree(image_p pimage);

image_p ImageClone(image_p pimage);

void ImageClear(image_p pimage);

void ImageToImage(const image_p src, const image_p des, int dx, int dy);

void ImageToImageEx(const image_p src, const image_p des, int sx, int sy, int sw, int sh, int dx, int dy);

int ImageFlipH(image_p pimage);

int ImageFlipV(image_p pimage);

void ImageFillRect(image_p pimage, int x, int y, int width, int height, u32 color);

void ImageHueChange(image_p pimage, int iHue);

void ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V);

void ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B);

#endif //__SIN_IMAGE_H__