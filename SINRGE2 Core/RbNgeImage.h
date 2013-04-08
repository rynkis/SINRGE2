#ifndef __CLASS_RGE_BITMAP_H__
#define __CLASS_RGE_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"
#include "nge_image.h"
#include "nge_graphics.h"
//#include "RgeTextureEx.h"

using namespace Sin;

class RbRect;
class RbFont;

class RbNgeImage : public RbClassBase
{
public:
	RbNgeImage();
	virtual ~RbNgeImage();

public:
	static void				InitLibrary();
	//static bool				AdjustTexturesToneSingle(const SRgeTexture* pTex, DWORD dwTone);
	//static bool				AdjustTexturesToneDouble(const SRgeTexture* pSrcTex, const SRgeTexture* pDstTex, DWORD dwTone);
	//static void				ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V);
	//static void				ColorSpaceHSV2RGB(float H, float S, float V, int &R, int &G, int &B);
	//static void				ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B);
	//static bool				CreateTextureInMemory(void* pData, u32 uDataLength, DWORD dwColorKey, SRgeTexture* pTex);

public:
	uint32_t				GetRows()					const { return m_image->w; }
	uint32_t				GetCols()					const { return m_image->h; }
	uint32_t				GetImgWidth()				const { return m_image->texw; }
	uint32_t				GetImgHeight()				const { return m_image->texh; }
	const image_p			GetNgeImage()				const { return m_image; }

	int						GetModifyCount()			const { return m_modify_count; }

	bool					IsDisposed()				const { return m_disposed; }

protected:
	virtual void			mark();
	virtual VALUE			initialize(int argc, VALUE *argv, VALUE obj);

protected:
	void					check_raise();

protected:
	bool					m_disposed;
	image_p					m_image;
	int						m_modify_count;

	VALUE					m_filename;		// mark

	RbRect*					m_rect_ptr;
	RbFont*					m_font_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)
	dm_method01(save_to_file)
	dm_method01(hue_change)
	dm_method01(brightness_change)
	dm_method_vargs(tone_change)

	dm_method_vargs(blt)
	dm_method_vargs(stretch_blt)
	dm_method_vargs(fill_rect)
	dm_method(clear)
	dm_method02(get_pixel)
	dm_method03(set_pixel)
	dm_method_vargs(draw_text)
	dm_method01(text_size)

	dm_method_vargs(gradient_fill_rect)
	dm_method_vargs(clear_rect)
	dm_method(blur)
	dm_method02(radial_blur)

	attr_reader(rect)
	attr_reader(width)
	attr_reader(height)
	attr_reader(filename)
	attr_accessor(font)
};

#endif	//	__CLASS_RGE_BITMAP_H__