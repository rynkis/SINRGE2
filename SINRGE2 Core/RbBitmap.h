#ifndef __CLASS_RGE_BITMAP_H__
#define __CLASS_RGE_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"
#include "RbTextureEx.h"

class RbRect;
class RbFont;

class RbBitmap : public RbClassBase
{
public:
	RbBitmap();
	virtual ~RbBitmap();

public:
	static void				InitLibrary();
	static bool				AdjustTexturesToneSingle(const SRgeTexture* pTex, DWORD dwTone);
	static bool				AdjustTexturesToneDouble(const SRgeTexture* pSrcTex, const SRgeTexture* pDstTex, DWORD dwTone);
	static void				ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V);
	//static void				ColorSpaceHSV2RGB(float H, float S, float V, int &R, int &G, int &B);
	static void				ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B);
	static bool				CreateTextureInMemory(void* pData, u32 uDataLength, DWORD dwColorKey, SRgeTexture* pTex);

public:
	u32						GetSubTextureAttrCount()	const { return m_tex.GetSubTextureCount(); }
	u32						GetRows()					const { return m_tex.rows; }
	u32						GetCols()					const { return m_tex.cols; }
	u32						GetBmpWidth()				const { return m_tex.width; }
	u32						GetBmpHeight()				const { return m_tex.height; }
	const SRgeTexture*		GetSRgeTexture()			const { return &m_tex; }
	const SRgeTextureAttr*	GetSubTextures()			const { return m_tex.sub_textures; }

	int						GetModifyCount()			const { return m_modify_count; }

	bool					IsDisposed()				const { return m_disposed; }
	bool					IsSingle()					const { return m_tex.IsSingle(); }
	bool					IsMulti()					const { return m_tex.IsMulti(); }

protected:
	virtual void			mark();
	virtual VALUE			initialize(int argc, VALUE *argv, VALUE obj);

protected:
	void					check_raise();

protected:
	bool					m_disposed;
	SRgeTexture				m_tex;
	int						m_modify_count;

	VALUE					m_filename;		// mark

	CRgeRect*				m_rect_ptr;
	CRgeFont*				m_font_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)
	dm_method_vargs(save_to_file)
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