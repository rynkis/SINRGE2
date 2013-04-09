#include "sin_image.h"
#include "nge_common.h"

using namespace Sin;

//HGE *hge = 0;

image_p ImageLoad(const wchar_t* filename)
{
	hgeQuad quad;
	//hge=hgeCreate(HGE_VERSION);
	quad.tex = GetHgePtr()->Texture_Load(filename);
	if(!quad.tex) return 0;
	quad.blend = BLEND_DEFAULT;
	for (int i = 0; i < 4; i++)
	{
		quad.v[i].z = 0.5f;
		quad.v[i].col = 0xffffffff;
	}
	quad.v[0].tx = 0; quad.v[0].ty = 0;
	quad.v[1].tx = 1; quad.v[1].ty = 0;
	quad.v[2].tx = 1; quad.v[2].ty = 1;
	quad.v[3].tx = 0; quad.v[3].ty = 1;
	
	image_p pimage = (image_p)malloc(sizeof(image_t));
	memset(pimage, 0, sizeof(image_t));
	pimage->quad = quad;
	pimage->w = GetHgePtr()->Texture_GetWidth(quad.tex, true);
	pimage->h = GetHgePtr()->Texture_GetHeight(quad.tex, true);
	pimage->texw = GetHgePtr()->Texture_GetWidth(quad.tex);
	pimage->texh = GetHgePtr()->Texture_GetHeight(quad.tex);
	pimage->rcentrex = pimage->w*1.0f/2;
	pimage->rcentrey = pimage->h*1.0f/2;

	return pimage;
}

image_p ImageLoadBuf(const char* mbuf, int bsize)
{
#pragma message("		Unfinished Function: " __FUNCTION__)
	return 0;
}

image_p ImageCreate(int width, int height)
{
	hgeQuad quad;
	//hge=hgeCreate(HGE_VERSION);
	quad.tex = GetHgePtr()->Texture_Create(width, height);
	if(!quad.tex) return 0;
	quad.blend = BLEND_DEFAULT;
	for (int i = 0; i < 4; i++)
	{
		quad.v[i].z = 0.5f;
		quad.v[i].col = 0xffffffff;
	}
	quad.v[0].tx = 0; quad.v[0].ty = 0;
	quad.v[1].tx = 1; quad.v[1].ty = 0;
	quad.v[2].tx = 1; quad.v[2].ty = 1;
	quad.v[3].tx = 0; quad.v[3].ty = 1;
	
	image_p pimage = (image_p)malloc(sizeof(image_t));
	memset(pimage, 0, sizeof(image_t));
	pimage->quad = quad;
	pimage->w = GetHgePtr()->Texture_GetWidth(quad.tex, true);
	pimage->h = GetHgePtr()->Texture_GetHeight(quad.tex, true);
	pimage->texw = GetHgePtr()->Texture_GetWidth(quad.tex);
	pimage->texh = GetHgePtr()->Texture_GetHeight(quad.tex);
	pimage->rcentrex = pimage->w*1.0f/2;
	pimage->rcentrey = pimage->h*1.0f/2;

	return pimage;
}

int ImageSave(image_p pimage,const wchar_t* filename, int alpha, int rle)
{
#pragma message("		Unfinished Function: " __FUNCTION__)
	return 0;
}

void ImageFree(image_p pimage)
{
	if(pimage){
		GetHgePtr()->Texture_Free(pimage->quad.tex);
		SAFE_FREE(pimage);
	}
}

image_p ImageClone(image_p pimage)
{
	image_p clone = (image_p)malloc(sizeof(image_t));
	memcpy(clone, pimage, sizeof(image_t));
	clone->quad.tex = GetHgePtr()->Texture_Create(pimage->w, pimage->h);
	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, true);
	DWORD* pDesTexData = GetHgePtr()->Texture_Lock(clone->quad.tex, false);
	memcpy(pDesTexData, pSrcTexData, pimage->w * pimage->h *sizeof(DWORD));
	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
	GetHgePtr()->Texture_Unlock(clone->quad.tex);
	return clone;
}

void ImageClear(image_p pimage)
{
	DWORD* pTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, false);
	if (!pTexData)
		return;

	for (int y = 0; y < pimage->h; ++y)
		memset(&pTexData[pimage->texw * y], 0, sizeof(DWORD) * pimage->w);

	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
}

void ImageToImageEx(const image_p src, const image_p des, int sx, int sy, int sw, int sh, int dx, int dy)
{
	if (sx >= src->w)
		return;

	if (sy >= src->h)
		return;

	if (sx < 0)	{ sw += sx; sx = 0; }
	if (sw <= 0)	return ;
	if (sy < 0)	{ sh += sy; sy = 0; }
	if (sh <= 0)	return ;

	if (src->w - sx < sw)	sw = src->w - sx;
	if (src->h - sy < sh)	sh = src->h - sy;

	if (dx < 0)	{ sw += dx; sx -= dx; dx = 0; }
	if (dy < 0)	{ sh += dy; sx -= dy; dy = 0; }
	if (des->w - dx < sw)	sw = des->w - dx;
	if (des->h - dy < sh)	sh = des->h - dy;
	if (sw <= 0)	return;
	if (sh <= 0)	return;

	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(src->quad.tex,true);
	DWORD* pDstTexData = GetHgePtr()->Texture_Lock(des->quad.tex,false);
	
	if (!pSrcTexData || !pDstTexData)
		return;
	
	DWORD color1, color2;
	for (s32 lx = sx; lx < sx + sw; ++lx)
	{
		for (s32 ly = sy; ly < sy + sh; ++ly)
		{
			color1 = pSrcTexData[src->texw * ly + lx];
			//	Ìø¹ýÍ¸Ã÷ÏñËØ
			if (!GET_RGBA_A(color1))
				continue;

			color2 = pDstTexData[des->texw * (ly - sy + dy) + lx - sx + dx];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[des->texw * (ly - sy + dy) + lx - sx + dx] = color2;
		}
	}

	GetHgePtr()->Texture_Unlock(src->quad.tex);
	GetHgePtr()->Texture_Unlock(des->quad.tex);
}

void ImageToImage(const image_p src, const image_p des, int dx, int dy)
{
	int SrcW = src->w;
	int SrcH = src->h;
	
	if (SrcW <= 0)	return ;
	if (SrcH <= 0)	return ;

	if (src->w < SrcW)	SrcW = src->w;
	if (src->h < SrcH)	SrcH = src->h;

	if (dx < 0)	{ SrcW += dx; dx = 0; }
	if (dy < 0)	{ SrcH += dy; dy = 0; }
	if (des->w - dx < SrcW)	SrcW = des->w - dx;
	if (des->h - dy < SrcH)	SrcH = des->h - dy;
	if (SrcW <= 0)	return;
	if (SrcH <= 0)	return;

	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(src->quad.tex, true);
	DWORD* pDstTexData = GetHgePtr()->Texture_Lock(des->quad.tex, false);

	if (!pSrcTexData || !pDstTexData)
		return;

	DWORD color1, color2;
	for (s32 lx = 0; lx < SrcW; ++lx)
	{
		for (s32 ly = 0; ly < SrcH; ++ly)
		{
			color1 = pSrcTexData[src->texw * ly + lx];
			//	Ìø¹ýÍ¸Ã÷ÏñËØ
			if (!GET_RGBA_A(color1))
				continue;

			color2 = pDstTexData[des->texw * (ly + dy) + lx + dx];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[des->texw * (ly + dy) + lx + dx] = color2;
		}
	}

	GetHgePtr()->Texture_Unlock(src->quad.tex);
	GetHgePtr()->Texture_Unlock(des->quad.tex);
}

int ImageFlipH(image_p pimage)
{
	s32 width = pimage->w;
	s32 height = pimage->h;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, false);
	if (!pTexData)
		return 1;

	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	for (s32 lx = 0; lx < width; ++lx)
	{
		for (s32 ly = 0; ly < height; ++ly)
			pTexData[pimage->texw * ly + lx] = pTempData[pimage->texw * (height - ly - 1) + lx];
	}
	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
	free(pTempData);
	return 0;
}

int ImageFlipV(image_p pimage)
{
	s32 width = pimage->w;
	s32 height = pimage->h;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, false);
	if (!pTexData)
		return 1;

	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	for (s32 lx = 0; lx < width; ++lx)
	{
		for (s32 ly = 0; ly < height; ++ly)
			pTexData[pimage->texw * ly + lx] = pTempData[pimage->texw * ly + (width - lx - 1)];
	}
	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
	free(pTempData);
	return 0;
}

void ImageFillRect(image_p pimage, int x, int y, int width, int height, u32 color)
{
	//	ÐÞÕý¾ØÐÎÇøÓò
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (pimage->texw - x < width)	{ width = pimage->texw - x; }
	if (pimage->texh - y < height)	{ height = pimage->texh - y; }

	if (width <= 0 || height <= 0)
		return;

	//	Ìî³ä
	DWORD* pTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, false);
	if (!pTexData)
		return;
	
	DWORD color2;
	for (s32 lx = x; lx < x + width; ++lx)
	{
		for (s32 ly = y; ly < y + height; ++ly)
		{
			//	Ìø¹ýÍ¸Ã÷ÏñËØ
			if (!GET_RGBA_A(color))
				continue;

			color2 = pTexData[pimage->texw * ly + lx];
			BLEND_ARGB_8888(color, color2);
			pTexData[pimage->texw * ly + lx] = color2;//color;
		}
	}

	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
}

void ImageHueChange(image_p pimage, int iHue)
{
	iHue %= 360;
	if (iHue < 0)
		iHue += 360;

	if (iHue == 0)
		return;

	float h, s, l;
	BYTE a, r, g, b;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(pimage->quad.tex, false);

	for (s32 x = 0; x < pimage->w; ++x)
	{
		for (s32 y = 0; y < pimage->h; ++y)
		{
			//color = pTexData[pimage->texw * y + x];
			GET_ARGB_8888(pTexData[pimage->texw * y + x], a, r, g, b);
			ColorSpaceRGB2HSV(r, g, b, h, s, l);
			h += iHue;
			ColorSpaceHSV2RGB(h, s, l, r, g, b);
			pTexData[pimage->texw * y + x] = MAKE_ARGB_8888(a, r, g, b);//color;
		}
	}

	GetHgePtr()->Texture_Unlock(pimage->quad.tex);
	return;
}

void ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V)
{
	int max = R;
	if (G > max) max = G;
	if (B > max) max = B;

	int min = R;
	if (G < min) min = G;
	if (B < min) min = B;

	float diff = (float)(max - min);

	V = (float)max;
	S = diff / max;

	if (max == min)
	{
		H = 0;
	}
	else
	{
		if (R == max)		H = (G - B) / diff;
		else if(G == max)	H = 2 + (B - R) / diff;
		else				H = 4 + (R - G) / diff;

		H *= 60;
		if(H < 0) H += 360;
	}
}

void ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B)
{
	if (S == 0)
	{
		R = G = B = (BYTE)V;
	}
	else
	{
		if (H < 0)		H += 360;
		if (H >= 360)	H -= 360;
		H /= 60;
		int i = (int)H;
		float f = H - i;
		float a = V * (1 - S);
		float b = V * (1 - S * f);
		float c = V * (1 - S * (1 - f));
		switch(i)
		{
		case 0:
			R = (BYTE)V; G = (BYTE)c; B = (BYTE)a; break;
		case 1:
			R = (BYTE)b; G = (BYTE)V; B = (BYTE)a; break;
		case 2:
			R = (BYTE)a; G = (BYTE)V; B = (BYTE)c; break;
		case 3:
			R = (BYTE)a; G = (BYTE)b; B = (BYTE)V; break;
		case 4:
			R = (BYTE)c; G = (BYTE)a; B = (BYTE)V; break;
		case 5:
			R = (BYTE)V; G = (BYTE)a; B = (BYTE)b; break;
		}
	}
}
