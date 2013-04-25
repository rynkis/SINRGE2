/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Bitmap
*/
#include "sin_color.h"
#include "RbExport.h"
#include "RbBitmap.h"
#include "RbColor.h"
#include "RbRect.h"
#include "RbFont.h"
#include "RbTone.h"
#include "sin_app.h"
#include <math.h>
#include <d3dx8tex.h>


VALUE rb_cBitmap;

namespace
{
	const BYTE sTable768_low[] =
	{
		  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
		 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
		 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
		128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
		160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
		192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
		224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,

		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
	};

	const BYTE sTable768_mid[] = 
	{
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

		  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
		 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
		 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
		128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
		160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
		192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
		224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
	};
}

RbBitmap::RbBitmap()
	: m_disposed(true)
	, m_modify_count(0)
	, m_filename(Qnil)
	, m_rect_ptr(0)
	, m_font_ptr(0)
{
}

RbBitmap::~RbBitmap()
{
	if (m_bmp.quad.tex)
	{
		GetAppPtr()->GetHgePtr()->Texture_Free(m_bmp.quad.tex);
		m_bmp.quad.tex = 0;
	}
}

void RbBitmap::InitLibrary()
{
	/**
	 *	@classname
	 *		Bitmap
	 *
	 *	@desc
	 *		位图的类。所谓位图即表示图像其本身。
	 */
	rb_cBitmap = rb_define_class_under(rb_mSin, "Bitmap", rb_cObject);
	
	// special method
	rb_define_alloc_func(rb_cBitmap, ObjAllocate<RbBitmap>);
	rb_define_method(rb_cBitmap, "initialize",			(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cBitmap, "dispose",				(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cBitmap, "disposed?",			(RbFunc)dm_is_disposed,			0);
	rb_define_method(rb_cBitmap, "save_to_file",		(RbFunc)dm_save_to_file,		-1);	//	snap_to_bitmap

	rb_define_method(rb_cBitmap, "hue_change",			(RbFunc)dm_hue_change,			1);
	rb_define_method(rb_cBitmap, "brightness_change",	(RbFunc)dm_brightness_change,	1);
	rb_define_method(rb_cBitmap, "tone_change",			(RbFunc)dm_tone_change,			-1);

	rb_define_method(rb_cBitmap, "blt",					(RbFunc)dm_blt,					-1);
	rb_define_method(rb_cBitmap, "stretch_blt",			(RbFunc)dm_stretch_blt,			-1);
	rb_define_method(rb_cBitmap, "fill_rect",			(RbFunc)dm_fill_rect,			-1);
	rb_define_method(rb_cBitmap, "clear",				(RbFunc)dm_clear,				0);
	rb_define_method(rb_cBitmap, "get_pixel",			(RbFunc)dm_get_pixel,			2);
	rb_define_method(rb_cBitmap, "set_pixel",			(RbFunc)dm_set_pixel,			3);

	rb_define_method(rb_cBitmap, "draw_text",			(RbFunc)dm_draw_text,			-1);
	rb_define_method(rb_cBitmap, "text_size",			(RbFunc)dm_text_size,			1);

	rb_define_method(rb_cBitmap, "gradient_fill_rect",	(RbFunc)dm_gradient_fill_rect,	-1);
	rb_define_method(rb_cBitmap, "clear_rect",			(RbFunc)dm_clear_rect,			-1);
	rb_define_method(rb_cBitmap, "blur",				(RbFunc)dm_blur ,				0);
	rb_define_method(rb_cBitmap, "radial_blur",			(RbFunc)dm_radial_blur,			2);
	
	rb_define_method(rb_cBitmap, "flip_h",				(RbFunc)dm_flip_h ,				0);
	rb_define_method(rb_cBitmap, "flip_v",				(RbFunc)dm_flip_v ,				0);

	// object attribute
	rb_define_method(rb_cBitmap, "width",				(RbFunc)dm_get_width,			0);
	rb_define_method(rb_cBitmap, "height",				(RbFunc)dm_get_height,			0);
	rb_define_method(rb_cBitmap, "rect",				(RbFunc)dm_get_rect,			0);
	rb_define_method(rb_cBitmap, "filename",			(RbFunc)dm_get_filename,		0);

	rb_define_method(rb_cBitmap, "font",				(RbFunc)dm_get_font,			0);
	rb_define_method(rb_cBitmap, "font=",				(RbFunc)dm_set_font,			1);

	// supplement
 	rb_define_method(rb_cBitmap, "to_s",				(RbFunc)dm_to_string,			0);
}

void RbBitmap::mark()
{
	if (m_rect_ptr)	m_rect_ptr->MarkObject();
	if (m_font_ptr)	m_font_ptr->MarkObject();

	rb_gc_mark(m_filename);
}

VALUE RbBitmap::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE arg01, arg02;

	DWORD dwColorValue = 0;
	HGE* hge = GetAppPtr()->GetHgePtr();
	hgeQuad quad;
	//wchar_t* filenameW;

	if (rb_scan_args(argc, argv, "11", &arg01, &arg02) == 1)
	{
		SafeStringValue(arg01);
		goto __bitmap_load;
	}
	else
	{
		if (TYPE(arg01) == T_STRING)
		{
			SafeColorValue(arg02);
			{
				RbColor* ccol;
				ccol = GetObjectPtr<RbColor>(arg02);
				dwColorValue = ccol->GetColor();
			}
			goto __bitmap_load;
		}
		else goto __bitmap_create;
	}

__bitmap_load:
	{
		int suffix_idx = -1;
		static char* szSuffixA[] = { ".png",  ".jpg",  ".bmp",  ".tga",  ".dds",  ".dib"};
		wchar_t filename[MAX_PATH];
		wcscpy_s(filename, Kconv::UTF8ToUnicode(RSTRING_PTR(arg01)));
		quad.tex = LoadTexture(filename, dwColorValue, suffix_idx);

		VALUE tmp_filename = rb_str_dup(arg01);
		if (suffix_idx != -1) tmp_filename = rb_str_plus(tmp_filename, rb_str_new2(szSuffixA[suffix_idx]));
		m_filename = rb_str_freeze(tmp_filename);
	}
	goto __finish;

__bitmap_create:
	{
		SafeFixnumValue(arg01);
		SafeFixnumValue(arg02);

		s32 w = FIX2INT(arg01);
		s32 h = FIX2INT(arg02);

		if (w > GetAppPtr()->GetMaxTexW() || h > GetAppPtr()->GetMaxTexH())
			rb_raise(rb_eSinError, "Too large width or height, Graphics card won't support.\n %d x %d", w, h);

		quad.tex = hge->Texture_Create(w, h);
		if(!quad.tex)
			rb_raise(rb_eSinError, "Failed to create bitmap: %d x %d .", w, h);
	}
	goto __finish;

__finish:
	{
		quad.blend = BLEND_DEFAULT;
		quad.blend_color = 0x00000000;
		for (int i = 0; i < 4; i++)
		{
			quad.v[i].z = 0.5f;
			quad.v[i].col = 0xffffffff;
		}
		quad.v[0].tx = 0; quad.v[0].ty = 0;
		quad.v[1].tx = 1; quad.v[1].ty = 0;
		quad.v[2].tx = 1; quad.v[2].ty = 1;
		quad.v[3].tx = 0; quad.v[3].ty = 1;
		
		m_bmp.quad = quad;
		m_bmp.width = hge->Texture_GetWidth(quad.tex);
		m_bmp.height = hge->Texture_GetHeight(quad.tex);
		m_bmp.owidth = hge->Texture_GetWidth(quad.tex, true);
		m_bmp.oheight = hge->Texture_GetHeight(quad.tex, true);

		//	create rect
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(m_bmp.width), INT2FIX(m_bmp.height)};
		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<RbRect>(rect);
		// create a font object for the bitmap-obj
		VALUE font = rb_class_new_instance(0, 0, rb_cFont);
		m_font_ptr = GetObjectPtr<RbFont>(font);
		m_disposed = false;
	}
	return obj;
}

HTEXTURE RbBitmap::LoadTexture(const wchar_t* filename, DWORD colorKey, int &suffix_idx)
{
	HGE* hge = GetAppPtr()->GetHgePtr();
	HTEXTURE tex = hge->Texture_Load(filename, 0, false, colorKey);
	if (!tex)
	{
		void *data;
		DWORD size;

		static wchar_t* szSuffix[]	= {L".png", L".jpg", L".bmp", L".tga", L".dds", L".dib"};
		static int		uSuffixCnt	= SinArrayCount(szSuffix);
		
		if (data = hge->Resource_Load_Without_Suffix(filename, &size, szSuffix, uSuffixCnt, &suffix_idx))
		{
			tex = hge->Texture_Load((const wchar_t*)data, size, false, colorKey);
			
			if(!tex)
				rb_raise(rb_eSinError, "Failed to load bitmap `%s'.", Kconv::UnicodeToUTF8(filename));

			// free
			hge->Resource_Free(data);
			return tex;
		}
		else
			rb_raise(rb_eSinError, "Failed to load bitmap `%s'.", Kconv::UnicodeToUTF8(filename));
	}
	return tex;
}

void RbBitmap::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed bitmap");
}

bool RbBitmap::AdjustTexturesTone(const bitmap_p pBmp, DWORD dwTone)
{
	if (!dwTone)
		return true;
	
	BYTE a1, r1, g1, b1, a2, r2, g2, b2;
	GET_ARGB_8888(dwTone, a1, r1, g1, b1);

	//int gray;

	DWORD* pSrcTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(pBmp->quad.tex, false);
	if (!pSrcTexData)
		return false;

	//int v;
	for (s32 x = 0; x < pBmp->width; ++x)
	{
		for (s32 y = 0; y < pBmp->height; ++y)
		{
			int v = pBmp->width * y + x;
			GET_ARGB_8888(pSrcTexData[v], a2, r2, g2, b2);
			if (a1 == 0)
			{
				r2 = sTable768_low[r2 + r1];
				g2 = sTable768_low[g2 + g1];
				b2 = sTable768_low[b2 + b1];
			}
			else
			{
				int gray = (r2 * 38 + g2 * 75 + b2 * 15) >> 7;

				r2 = sTable768_low[r1 + r2 + (gray - r2) * a1 / 256];
				g2 = sTable768_low[g1 + g2 + (gray - g2) * a1 / 256];
				b2 = sTable768_low[b1 + b2 + (gray - b2) * a1 / 256];
			}
			pSrcTexData[v] = MAKE_ARGB_8888(a2, r2, g2, b2);
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(pBmp->quad.tex);

	return true;
}

bool RbBitmap::AdjustTexturesToneDouble(const bitmap_p pSrcBmp, const HTEXTURE pDstTex, DWORD dwTone)
{
	DWORD* pSrcTexData = 0;
	DWORD* pDstTexData = 0;

	if (!pSrcBmp->quad.tex)
		return false;

	if (!pDstTex)
		return false;
	
	HGE* hge = GetAppPtr()->GetHgePtr();
	
	/*int dw = hge->Texture_GetWidth(pDstTex, true);
	int dh = hge->Texture_GetHeight(pDstTex, true);*/
	int dtw = hge->Texture_GetWidth(pDstTex);
	int dth = hge->Texture_GetHeight(pDstTex);

	if (pSrcBmp->width != dtw ||
		pSrcBmp->height != dth/* ||
		pSrcBmp->owidth != dw ||
		pSrcBmp->oheight != dh*/)
		return false;

	if (!dwTone)
		return true;

	BYTE a1, r1, g1, b1, a2, r2, g2, b2;
	GET_ARGB_8888(dwTone, a1, r1, g1, b1);

	//int gray;

	pSrcTexData = hge->Texture_Lock(pSrcBmp->quad.tex, true);

	if (!pSrcTexData)
		goto failed_return;

	pDstTexData = hge->Texture_Lock(pDstTex, false);

	if (!pDstTexData)
		goto failed_return;

	for (int y = 0; y < pSrcBmp->height; ++y)
	{
		int th = pSrcBmp->width * y;
		for (int x = 0; x < pSrcBmp->width; ++x)
		{
			int tv = th + x;
			GET_ARGB_8888(pSrcTexData[tv], a2, r2, g2, b2);
			if (a1 == 0)
			{
				r2 = sTable768_low[r2 + r1];
				g2 = sTable768_low[g2 + g1];
				b2 = sTable768_low[b2 + b1];
			}
			else
			{
				int gray = (r2 * 38 + g2 * 75 + b2 * 15) >> 7;

				r2 = sTable768_low[r1 + r2 + (gray - r2) * a1 / 256];
				g2 = sTable768_low[g1 + g2 + (gray - g2) * a1 / 256];
				b2 = sTable768_low[b1 + b2 + (gray - b2) * a1 / 256];
			}
			pDstTexData[tv] = MAKE_ARGB_8888(a2, r2, g2, b2);
		}
	}

	hge->Texture_Unlock(pSrcBmp->quad.tex);
	hge->Texture_Unlock(pDstTex);

	return true;

failed_return:
	if (pSrcTexData) hge->Texture_Unlock(pSrcBmp->quad.tex);
	if (pDstTexData) hge->Texture_Unlock(pDstTex);

	return false;
}

void RbBitmap::ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V)
{
	int max = R;
	if (G > max) max = G;
	if (B > max) max = B;

	int min = R;
	if (G < min) min = G;
	if (B < min) min = B;

	float diff = (float)(max - min);

	V = max;
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

void RbBitmap::ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B)
{
	if (S == 0)
	{
		R = G = B = V;
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
			R = V; G = c; B = a; break;
		case 1:
			R = b; G = V; B = a; break;
		case 2:
			R = a; G = V; B = c; break;
		case 3:
			R = a; G = b; B = V; break;
		case 4:
			R = c; G = a; B = V; break;
		case 5:
			R = V; G = a; B = b; break;
		}
	}
}

bool RbBitmap::GetTextRect(HFONT hFont, const wchar_t* pStr, s32 &cx, s32 &cy, HDC hDC)
{
	HDC hScreenDC;

	if (hDC)	
		hScreenDC = hDC;
	else	
		hScreenDC = GetDC(NULL);

	HGDIOBJ Save = SelectObject(hScreenDC, (HGDIOBJ)hFont);

	SIZE size;

	BOOL bRet = GetTextExtentPoint32(hScreenDC, pStr, wcslen(pStr), &size);

	if (bRet)
	{
		cx = size.cx;
		cy = size.cy;
	}

	SelectObject(hScreenDC, Save);

	if (hDC == NULL)
		ReleaseDC(NULL, hScreenDC);

	return (bRet != 0);
}

bitmap_p RbBitmap::CloneBitmap(bitmap_p pBmp)
{
	HGE* hge = GetAppPtr()->GetHgePtr();
	bitmap_p clone = (bitmap_p)malloc(sizeof(bitmap_t));
	memcpy(clone, pBmp, sizeof(bitmap_t));
	clone->quad.tex = hge->Texture_Create(pBmp->width, pBmp->height);
	DWORD* pSrcTexData = hge->Texture_Lock(pBmp->quad.tex, true);
	DWORD* pDesTexData = hge->Texture_Lock(clone->quad.tex, false);
	memcpy(pDesTexData, pSrcTexData, pBmp->width * pBmp->height *sizeof(DWORD));
	hge->Texture_Unlock(pBmp->quad.tex);
	hge->Texture_Unlock(clone->quad.tex);
	return clone;
}

HTEXTURE RbBitmap::CutTexture(int x, int y, int width, int height, bitmap_p pBmp)
{
	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (pBmp->width - x < width)	{ width = pBmp->width - x; }
	if (pBmp->height - y < height)	{ height = pBmp->height - y; }

	if (width <= 0 || height <= 0)
		return 0;
	
	HGE* hge = GetAppPtr()->GetHgePtr();

	HTEXTURE cut = hge->Texture_Create(width, height);
	DWORD* pSrcTexData = hge->Texture_Lock(pBmp->quad.tex, true);//, x, y, width, height);
	DWORD* pDesTexData = hge->Texture_Lock(cut, false);
	//memcpy(pDesTexData, pSrcTexData, width * height *sizeof(DWORD));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pDesTexData + (width * ly), pSrcTexData + (pBmp->width * (y + ly) + x), sizeof(DWORD) * width);
	hge->Texture_Unlock(pBmp->quad.tex);
	hge->Texture_Unlock(cut);
	return cut;
}

void RbBitmap::BilinearZoom(DWORD *OldBitmap, DWORD *NewBitmap, int OldWidth, int OldHeight, int NewWidth, int NewHeight, int MathWidth, int MathHeight)
{
	long x, y, ty1, ty2, tx1, tx2, px1, px2, py1, py2, p11, p21, p12, p22;
	
	DWORD color11, color21, color12, color22;
	BYTE a, r, g, b;
	for (y = 0; y < NewHeight; y++)
	{
		// 获取y坐标
		ty1 = y * OldHeight / MathHeight;
		ty2 = ty1 + 1;
		// 获取y权
		py2 = y * OldHeight % MathHeight * 100 / MathHeight;
		py1 = 100 - py2;
		for (x = 0; x < NewWidth; x++)
		{
			tx1 = x * OldWidth / MathWidth;
			tx2 = tx1 + 1;
			// 获取x权
			px2 = x * OldWidth % MathWidth * 100 / MathWidth;
			px1 = 100 - px2;
			// 获取四点颜色对应的权
			p11 = px1 * py1;
			p21 = px2 * py1;
			p12 = px1 * py2;
			p22 = px2 * py2;
			// 获取颜色
			color11 = OldBitmap[tx1 + ty1 * OldWidth];
			color21 = OldBitmap[tx2 + ty1 * OldWidth];
			color12 = OldBitmap[tx1 + ty2 * OldWidth];
			color22 = OldBitmap[tx2 + ty2 * OldWidth];
			// 计算颜色
			a = (GET_ARGB_A(color11) * p11 + GET_ARGB_A(color21) * p21 + GET_ARGB_A(color12) * p12 + GET_ARGB_A(color22) * p22) / 10000;
			r = (GET_ARGB_R(color11) * p11 + GET_ARGB_R(color21) * p21 + GET_ARGB_R(color12) * p12 + GET_ARGB_R(color22) * p22) / 10000;
			g = (GET_ARGB_G(color11) * p11 + GET_ARGB_G(color21) * p21 + GET_ARGB_G(color12) * p12 + GET_ARGB_G(color22) * p22) / 10000;
			b = (GET_ARGB_B(color11) * p11 + GET_ARGB_B(color21) * p21 + GET_ARGB_B(color12) * p12 + GET_ARGB_B(color22) * p22) / 10000;
			NewBitmap[x + y * NewWidth] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
}

bool RbBitmap::ScreenToBitmap(bitmap_p pBmp)
{
	int width, height;
	HGE* hge = GetAppPtr()->GetHgePtr();
	DWORD* pSrcData = hge->System_Snapshot(width, height);

	if (!pSrcData)
		return false;

	if (pBmp->quad.tex)
		GetAppPtr()->GetHgePtr()->Texture_Free(pBmp->quad.tex);

	pBmp->quad.tex = hge->Texture_Create(GetAppPtr()->GetFrameWidth(), height);
	DWORD* pDesData = hge->Texture_Lock(pBmp->quad.tex, false);
	
	BYTE a, r, g, b;
	for (int ly = 0; ly < height; ++ly)
	{
		int v = ly * width;
		int i = GetAppPtr()->GetFrameWidth() * ly;
		for (int lx = 0; lx < GetAppPtr()->GetFrameWidth(); ++lx)
		{
			GET_ARGB_8888(pSrcData[v + lx], a, r, g, b);
			pDesData[i + lx] = MAKE_ARGB_8888(255, r, g, b);
		}
	}
		//memcpy(pDesData + GetAppPtr()->GetFrameWidth() * ly, pSrcData + width * ly, GetAppPtr()->GetFrameWidth() * sizeof(DWORD));
	hge->Texture_Unlock(pBmp->quad.tex);
	free(pSrcData);

	pBmp->width		= GetAppPtr()->GetFrameWidth();//hge->Texture_GetWidth(pBmp->quad.tex, true);
	pBmp->height	= height;//hge->Texture_GetHeight(pBmp->quad.tex, true);

	return true;
}

VALUE RbBitmap::dispose()
{
	if (m_disposed)
		return Qnil;

	if(m_bmp.quad.tex)
	{
		GetAppPtr()->GetHgePtr()->Texture_Free(m_bmp.quad.tex);
		m_bmp.quad.tex = 0;
	}

	m_disposed = true;

	return Qnil;
}

VALUE RbBitmap::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE RbBitmap::hue_change(VALUE hue)
{
	check_raise();

	SafeFixnumValue(hue);

	int iHue = FIX2INT(hue);

	if (!iHue)
		return Qnil;

	iHue %= 360;
	if (iHue < 0)	iHue += 360;

	/*double rad = iHue * M_PI / 180;
	double cos = cosf(rad);
	double sin = sinf(rad);
	double temp = (1.0 - cos)/3.0;
	float ta = (float)(temp + cos);
	float tb = (float)(temp - sqrt(0.33333) * sin);
	float tc = (float)(temp + sqrt(0.33333) * sin);

	float matrix[] = {
				ta, tb, tc,
				tc, ta, tb,
				tb, tc, ta
		};*/

	float h, s, l;
	BYTE a, r, g, b;

	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);

	//int v;
	for (s32 x = 0; x < m_bmp.width; ++x)
	{
		for (s32 y = 0; y < m_bmp.height; ++y)
		{
			int v = m_bmp.width * y + x;
			GET_ARGB_8888(pTexData[v], a, r, g, b);
			/*r = (BYTE)SinBound(r*matrix[0]+g*matrix[1]+b*matrix[2], 0, 255);
			g = (BYTE)SinBound(r*matrix[3]+g*matrix[4]+b*matrix[5], 0, 255);
			b = (BYTE)SinBound(r*matrix[6]+g*matrix[7]+b*matrix[8], 0, 255);*/
			ColorSpaceRGB2HSV(r, g, b, h, s, l);
			h += iHue;
			ColorSpaceHSV2RGB(h, s, l, r, g, b);
			pTexData[v] = MAKE_ARGB_8888(a, r, g, b);
		}
	}

	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::brightness_change(VALUE brightness)
{
	check_raise();

	SafeFixnumValue(brightness);

	int iBrightness = FIX2INT(brightness);

	if(!iBrightness) return Qnil;

	iBrightness = SinBound(iBrightness, -255, 255);

	BYTE a, r, g, b;

	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);

	for (s32 y = 0; y < m_bmp.height; ++y)
	{
		int th = m_bmp.width * y;
		for (s32 x = 0; x < m_bmp.width; ++x)
		{
			int tv = th + x;
			GET_ARGB_8888(pTexData[tv], a, r, g, b);
			r = sTable768_mid[r + iBrightness + 256];
			g = sTable768_mid[g + iBrightness + 256];
			b = sTable768_mid[b + iBrightness + 256];
			pTexData[tv] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
	
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::tone_change(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	int a, r, g, b;
	VALUE argv01, green, blue, gray;
	DWORD tone = 0;

	if (rb_scan_args(argc, argv, "04", &argv01, &green, &blue, &gray) == 1)
	{
		if (FIXNUM_P(argv01))
		{
			r = FIX2INT(argv01);
			r = SinBound(r, 0, 255);
			tone = MAKE_ARGB_8888(255, r, 255, 255);
		}
		else
		{
			SafeToneValue(argv01);
			tone = GetObjectPtr<RbTone>(argv01)->GetColor();
		}
	}
	else
	{
		for (int i = 0; i < argc; ++i)
			SafeFixnumValue(argv[i]);

		r = (NIL_P(argv01) ? 255 : FIX2INT(argv01));
		g = (NIL_P(green) ? 255 : FIX2INT(green));
		b = (NIL_P(blue) ? 255 : FIX2INT(blue));
		a = (NIL_P(gray) ? 255 : FIX2INT(gray));

		r = SinBound(r, 0, 255);
		g = SinBound(g, 0, 255);
		b = SinBound(b, 0, 255);
		a = SinBound(a, 0, 255);

		tone = MAKE_ARGB_8888(a, r, g, b);
	}

	AdjustTexturesTone(&m_bmp, tone);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::blt(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE x, y, src_bitmap, src_rect, opacity;
	bitmap_p src;
	bool with_self = false;

	if (rb_obj_class((argv[2])) == rb_cBitmap)
	{
		rb_scan_args(argc, argv, "41", &x, &y, &src_bitmap, &src_rect, &opacity);

		SafeBitmapValue(src_bitmap);
		RbBitmap* srcBmp = GetObjectPtr<RbBitmap>(src_bitmap);
		src = srcBmp->GetBitmapPtr();
	}
	else
	{
		rb_scan_args(argc, argv, "31", &x, &y, &src_rect, &opacity);
		src = &m_bmp;
		with_self = true;
	}
	SafeFixnumValue(x);
	SafeFixnumValue(y);
	SafeRectValue(src_rect);

	bitmap_p des = &m_bmp;
	RbRect* srcRect = GetObjectPtr<RbRect>(src_rect);
	int sx = srcRect->x;
	int sy = srcRect->y;
	int sw = srcRect->width;
	int sh = srcRect->height;
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);
	int op;
	if (NIL_P(opacity))
		op = 255;
	else
	{
		SafeFixnumValue(opacity);
		op = FIX2INT(opacity);
		op = SinBound(op, 0, 255);
	}
	if (sx >= src->width) return Qfalse;
	if (sy >= src->height) return Qfalse;

	if (sx < 0)	{ sw += sx; sx = 0; }
	if (sw <= 0)	return Qfalse;
	if (sy < 0)	{ sh += sy; sy = 0; }
	if (sh <= 0)	return Qfalse;

	if (src->width - sx < sw)	sw = src->width - sx;
	if (src->height - sy < sh)	sh = src->height - sy;

	if (dx < 0)	{ sw += dx; sx -= dx; dx = 0; }
	if (dy < 0)	{ sh += dy; sx -= dy; dy = 0; }
	if (des->width - dx < sw)	sw = des->width - dx;
	if (des->height - dy < sh)	sh = des->height - dy;
	if (sw <= 0)	return Qfalse;
	if (sh <= 0)	return Qfalse;

	DWORD* pTempData;
	DWORD* pSrcTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(src->quad.tex, true);
	if (with_self)
	{
		pTempData = (DWORD*)malloc(src->width * src->height * sizeof(DWORD));
		memcpy(pTempData, pSrcTexData, src->width * src->height * sizeof(DWORD));
		GetAppPtr()->GetHgePtr()->Texture_Unlock(src->quad.tex);
	}
	else
	{
		pTempData = pSrcTexData;
	}
	DWORD* pDstTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(des->quad.tex,false);
	if (!pTempData || !pDstTexData)
		return Qfalse;
	DWORD color1, color2;
	BYTE a, r, g, b;
	//int v;
	for (int lx = sx; lx < sx + sw; ++lx)
	{
		for (int ly = sy; ly < sy + sh; ++ly)
		{
			int v = des->width * (ly - sy + dy) + lx - sx + dx;
			color1 = pTempData[src->width * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	跳过透明像素
			if (!a) continue;
			a = a * op / 255;
			//	跳过透明像素
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[v];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[v] = color2;
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(des->quad.tex);
	if (with_self)
		free(pTempData);
	else
	{
		pTempData = NULL;
		GetAppPtr()->GetHgePtr()->Texture_Unlock(src->quad.tex);
	}
	
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::stretch_blt(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE dest_rect, src_bitmap, src_rect, opacity;
	bitmap_p src;
	rb_scan_args(argc, argv, "31", &dest_rect, &src_bitmap, &src_rect, &opacity);
	
	SafeRectValue(dest_rect);
	SafeBitmapValue(src_bitmap);
	SafeRectValue(src_rect);
	
	RbRect* desRect = GetObjectPtr<RbRect>(dest_rect);
	int dx = desRect->x;
	int dy = desRect->y;
	int dw = desRect->width;
	int dh = desRect->height;
	RbBitmap* srcBmp = GetObjectPtr<RbBitmap>(src_bitmap);
	src = srcBmp->GetBitmapPtr();
	RbRect* srcRect = GetObjectPtr<RbRect>(src_rect);
	int sx = srcRect->x;
	int sy = srcRect->y;
	int sw = srcRect->width;
	int sh = srcRect->height;
	int op;
	if (NIL_P(opacity))
		op = 255;
	else
	{
		SafeFixnumValue(opacity);
		op = FIX2INT(opacity);
		op = SinBound(op, 0, 255);
	}
	
	if (dx + dw <= 0) return Qfalse;
	if (dy + dh <= 0) return Qfalse;
	if (m_bmp.width - dx <= 0) return Qfalse;
	if (m_bmp.height - dy <= 0) return Qfalse;
	
	HGE* hge = GetAppPtr()->GetHgePtr();

	HTEXTURE tmpTex;
	/*if (sx == 0 && sy == 0 && sw == src->width && sh == src->height)
		tmpTex = src->quad.tex;
	else*/
	tmpTex = CutTexture(sx, sy, sw, sh, src);

	if (!tmpTex) return Qfalse;

	DWORD* pSrcTexData = hge->Texture_Lock(tmpTex, true);
	if (!pSrcTexData) return Qfalse;
	DWORD* pTempData = (DWORD*)malloc(dw * dh * sizeof(DWORD));
	int mathW = dw + ceil(sqrt(sqrt((double)(dw / sw))));
	int mathH = dh + ceil(sqrt(sqrt((double)(dh / sh))));
	BilinearZoom(pSrcTexData, pTempData, sw, sh, dw, dh, mathW, mathH);

	DWORD* pDstTexData = hge->Texture_Lock(m_bmp.quad.tex, false);
	DWORD color1, color2;
	BYTE a, r, g, b;

	int ndw = dw;
	if (dw + dx > m_bmp.width) dw = m_bmp.width - dx;
	if (dh + dy > m_bmp.height) dh = m_bmp.height - dy;

	for (s32 lx = 0; lx < dw; ++lx)
	{
		for (s32 ly = 0; ly < dh; ++ly)
		{
			int v = m_bmp.width * (ly + dy) + lx + dx;
			color1 = pTempData[ndw * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	跳过透明像素
			if (!a) continue;
			a = a * op / 255;
			//	跳过透明像素
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[v];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[v] = color2;
		}
	}
	hge->Texture_Unlock(m_bmp.quad.tex);
	hge->Texture_Unlock(tmpTex);
	//if (sx != 0 || sy != 0 || sw != src->width || sh != src->height)
	hge->Texture_Free(tmpTex);
	tmpTex = 0;
	free(pTempData);
	
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::fill_rect(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	int x, y, width, height;
	DWORD color;

	if (argc == 5)
	{
		for (int i = 0; i < 4; ++i)
			SafeFixnumValue(argv[i]);
		
		SafeColorValue(argv[4]);
		x = FIX2INT(argv[0]);
		y = FIX2INT(argv[1]);
		width = FIX2INT(argv[2]);
		height = FIX2INT(argv[3]);
		color = GetObjectPtr<RbTone>(argv[4])->GetColor();
	}
	else if (argc == 2)
	{
		SafeRectValue(argv[0]);
		SafeColorValue(argv[1]);

		RbRect* rect = GetObjectPtr<RbRect>(argv[0]);
		x = rect->x;
		y = rect->y;
		width = rect->width;
		height = rect->height;
		color = GetObjectPtr<RbTone>(argv[1])->GetColor();
	}
	else
		rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);

	//	跳过透明像素
	/*if (!GET_ARGB_A(color))
		return Qnil;*/

	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_bmp.width - x < width)		{ width = m_bmp.width - x; }
	if (m_bmp.height - y < height)	{ height = m_bmp.height - y; }

	if (width <= 0 || height <= 0)
		return Qfalse;

	//	填充
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	
	//int v;
	for (s32 ly = y; ly < y + height; ++ly)
	{
		int v = m_bmp.width * ly;
		for (s32 lx = x; lx < x + width; ++lx)
			pTexData[v + lx] = color;
	}

	for (s32 ly = y + 1; ly < y + height; ++ly)
		memcpy(pTexData + (m_bmp.width * ly) + x, pTexData + (m_bmp.width * y + x), sizeof(DWORD) * width);

	//DWORD color2;
	//for (s32 lx = x; lx < x + width; ++lx)
	//{
	//	for (s32 ly = y; ly < y + height; ++ly)
	//	{
	//		color2 = pTexData[m_bmp.width * ly + lx];
	//		BLEND_ARGB_8888(color, color2);
	//		pTexData[m_bmp.width * ly + lx] = color2;
	//	}
	//}

	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::clear()
{
	check_raise();

	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;

	/*for (int y = 0; y < m_bmp.height; ++y)
		memset(&pTexData[m_bmp.width * y], 0, sizeof(DWORD) * m_bmp.width);*/
	ZeroMemory(pTexData, sizeof(DWORD) * m_bmp.width * m_bmp.height);

	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::get_pixel(VALUE x, VALUE y)
{
	check_raise();
	
	SafeFixnumValue(x);
	SafeFixnumValue(y);
	
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);

	if (dx < 0 || dy < 0 || dx >= m_bmp.width || dy >= m_bmp.height)
		return Qnil;

	int a, r, g, b;
	
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData) return Qnil;
	GET_ARGB_8888(pTexData[m_bmp.width * dy + dx], a, r, g, b);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	VALUE __argv[] = {INT2FIX(r), INT2FIX(g), INT2FIX(b), INT2FIX(a)};
	VALUE pixel = rb_class_new_instance(4, __argv, rb_cColor);
	return pixel;
}

VALUE RbBitmap::set_pixel(VALUE x, VALUE y, VALUE color)
{
	check_raise();

	SafeFixnumValue(x);
	SafeFixnumValue(y);
	SafeColorValue(color);
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);
	
	if (dx < 0 || dy < 0 || dx >= m_bmp.width || dy >= m_bmp.height)
		return Qnil;

	DWORD dwColor = GetObjectPtr<RbTone>(color)->GetColor();
	//	跳过透明像素
	/*if (!GET_ARGB_A(dwColor))
		return Qfalse;*/

	//DWORD color2;
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	//color2 = pTexData[m_bmp.width * dy + dx];
	//BLEND_ARGB_8888(dwColor, color2);
	pTexData[m_bmp.width * dy + dx] = dwColor;//color2;
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::draw_text(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE vx, vy, vw, vh, vstr, ha, va, rect;
	int ix, iy, width, height;
	wchar_t* pStr;
	int halign, valign;

	if (FIXNUM_P(argv[0]))
	{
		rb_scan_args(argc, argv, "52", &vx, &vy, &vw, &vh, &vstr, &ha, &va);
		
		SafeFixnumValue(vx);
		SafeFixnumValue(vy);
		SafeFixnumValue(vw);
		SafeFixnumValue(vh);
		SafeStringValue(vstr);

		ix = FIX2INT(vx);
		iy = FIX2INT(vy);
		width = FIX2INT(vw);
		height = FIX2INT(vh);
	}
	else
	{
		rb_scan_args(argc, argv, "22", &rect, &vstr, &ha, &va);

		SafeRectValue(argv[0]);
		SafeStringValue(vstr);

		RbRect* rect = GetObjectPtr<RbRect>(argv[0]);
		ix = rect->x;
		iy = rect->y;
		width = rect->width;
		height = rect->height;
	}
	pStr  = Kconv::UTF8ToUnicode(RSTRING_PTR(vstr));
	halign = NIL_P(ha) ? 0 : FIX2INT(ha);
	valign = NIL_P(va) ? 1 : FIX2INT(va);

	if (width <= 0 || height <= 0)
		return Qfalse;

	int iCharCount = wcslen(pStr);
	if (!iCharCount)
		return Qfalse;

	DWORD draw_color = m_font_ptr->GetColorPtr()->GetColor();
	BYTE da = GET_ARGB_A(draw_color);//, dr, dg, db;
	DWORD drgb = draw_color & 0x00FFFFFF;
	//GET_ARGB_8888(draw_color, da, dr, dg, db);
	if (!da)
		return Qfalse;

	bool shadow = m_font_ptr->IsShadow();

	DWORD color1, color2 = 0;

	HGE* hge = GetAppPtr()->GetHgePtr();

	//	开始正式描绘文字
	HDC	hScreenDC = GetDC(NULL);

	do
	{
		s32 cx, cy;

		if (!GetTextRect(m_font_ptr->GetHFont(), pStr, cx, cy, hScreenDC))
			break;

		if (shadow) cy += 1;

		int offset_x = 0, offset_y = 0;

		switch (halign % 3)
		{
			case 0:offset_x = 0;				break;
			case 1:offset_x = (width - cx) / 2;	break;
			case 2:offset_x = (width - cx);		break;
		}

		switch (valign % 3)
		{
			case 0:offset_y = 0;					break;
			case 1:offset_y = (height - cy) / 2;	break;
			case 2:offset_y = (height - cy);		break;
		}

		int iCharOffset = 0;

		GLYPHMETRICS	gm;
		TEXTMETRIC		tm;

		static MAT2		mmat2 = {0, 1, 0, 0, 0, 0, 0, 1};

		static BYTE		alphatable65[] = // 65.times{|i| 255 * i / (65 - 1)
									{0,3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63,67,71,75,79,83,
									87,91,95,99,103,107,111,115,119,123,127,131,135,139,143,147,151,
									155,159,163,167,171,175,179,183,187,191,195,199,203,207,211,215,
									219,223,227,231,235,239,243,247,251,255};

		static BYTE		buffer[1024];

		//	锁定纹理
		DWORD *pTexData = hge->Texture_Lock(m_bmp.quad.tex, false);

		//	锁定失败：退出
		if (!pTexData)
			break;

		HFONT hOldFont = (HFONT)SelectObject(hScreenDC, m_font_ptr->GetHFont());

		//	临时变量
		int tmp_x, tmp_y;

		//	循环逐字描绘
		for (int i = 0; i < iCharCount; ++i)
		{
			DWORD dwBufferSize = GetGlyphOutline(hScreenDC, pStr[i], GGO_GRAY8_BITMAP, &gm, 0, NULL, &mmat2);

			if (dwBufferSize > SinArrayCount(buffer))
				rb_raise(rb_eSinError, "buffer size too small.");

			if (dwBufferSize > 0)
			{
				int nLentemp = GetGlyphOutline(hScreenDC, pStr[i],GGO_GRAY8_BITMAP,&gm,dwBufferSize,buffer,&mmat2);

				GetTextMetrics(hScreenDC, &tm);

				int stride = dwBufferSize / gm.gmBlackBoxY;

				//	逐像素描绘
				for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
				{
					tmp_y = y + tm.tmAscent - gm.gmptGlyphOrigin.y + iy + offset_y;
					int h1 = (tmp_y + 1) * m_bmp.width;
					int h2 = tmp_y * m_bmp.width;
					for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
					{
						//	过滤掉越界的像素
						tmp_x = x + gm.gmptGlyphOrigin.x + iCharOffset + ix + offset_x;
						if (tmp_x < 0 || tmp_x >= m_bmp.width || tmp_y < 0 || tmp_y >= m_bmp.height)
							continue;

						//	过滤全透明区域
						BYTE tmp_gray = alphatable65[buffer[y * stride + x]];
						if (!tmp_gray)
							continue;

						tmp_gray = (BYTE)((float)tmp_gray * da / 255);
						if (!tmp_gray)
							continue;

						if (shadow)
						{
							int v1 = h1 + tmp_x + 1;
							//	处理阴影颜色
							color1 = tmp_gray<<24;
							color2 = pTexData[v1];
							BLEND_ARGB_8888(color1, color2);
							pTexData[v1] = color2;
						}

						int v2 = h2 + tmp_x;
						//	处理像素颜色
						color1 = drgb + (tmp_gray<<24);
						color2 = pTexData[v2];
						BLEND_ARGB_8888(color1, color2);
						pTexData[v2] = color2;
					}
				}
			}

			iCharOffset += gm.gmCellIncX;
		}

		// 取消锁定
		hge->Texture_Unlock(m_bmp.quad.tex);
		
		SelectObject(hScreenDC, hOldFont);
		
		//	增加 修改计数值
		++m_modify_count;

	} while (0);
	
	//	释放DC
	ReleaseDC(NULL, hScreenDC);

	return Qnil;
}

VALUE RbBitmap::text_size(VALUE str)
{
	VALUE vStr = NIL_P(str) ? rb_str_new2("nil") : rb_obj_as_string(str);

	wchar_t* pStr = Kconv::UTF8ToUnicode(RSTRING_PTR(vStr));
	
	s32 cx, cy;
	VALUE rect;

	if (GetTextRect(m_font_ptr->GetHFont(), pStr, cx, cy, NULL))
	{
		if (m_font_ptr->IsShadow()) cy += 1;
		VALUE __argv[] = {RUBY_0, RUBY_0, LONG2FIX(cx), LONG2FIX(cy)};
		rect = rb_class_new_instance(4, __argv, rb_cRect);
	}
	else
	{
		VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0,};
		rect = rb_class_new_instance(4, __argv, rb_cRect);
	}
	return rect;
}

VALUE RbBitmap::gradient_fill_rect(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE vx, vy, vw, vh, vcolor1, vcolor2, vert, rect;
	int x, y, width, height;

	if (FIXNUM_P(argv[0]))
	{
		rb_scan_args(argc, argv, "61", &vx, &vy, &vw, &vh, &vcolor1, &vcolor2, &vert);
		
		SafeFixnumValue(vx);
		SafeFixnumValue(vy);
		SafeFixnumValue(vw);
		SafeFixnumValue(vh);

		x = FIX2INT(vx);
		y = FIX2INT(vy);
		width = FIX2INT(vw);
		height = FIX2INT(vh);
	}
	else
	{
		rb_scan_args(argc, argv, "31", &rect, &vcolor1, &vcolor2, &vert);

		SafeRectValue(argv[0]);

		RbRect* rect = GetObjectPtr<RbRect>(argv[0]);
		x = rect->x;
		y = rect->y;
		width = rect->width;
		height = rect->height;
	}
	
	if (m_bmp.width - x <= 0 || m_bmp.height - y <= 0)
		return Qfalse;

	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData) return Qfalse;

	SafeColorValue(vcolor1);
	SafeColorValue(vcolor2);
	DWORD color1 = GetObjectPtr<RbColor>(vcolor1)->GetColor();
	DWORD color2 = GetObjectPtr<RbColor>(vcolor2)->GetColor();
	
	bool vertical = RTEST(vert);
	float a1, r1, g1, b1, a2, r2, g2, b2, rateA, rateR, rateG, rateB;;
	GET_ARGB_8888(color1, a1, r1, g1, b1);
	GET_ARGB_8888(color2, a2, r2, g2, b2);

	int /*v1, v2, */value1, value2;
	if (!vertical) { value1 = width; value2 = height; }
	else { value1 = height; value2 = width; }

	rateA = (a2 - a1) / value1;
	rateR = (r2 - r1) / value1;
	rateG = (g2 - g1) / value1;
	rateB = (b2 - b1) / value1;

	DWORD* pline = (DWORD*)malloc(value1 * sizeof(DWORD));
	for (s32 ln = 0; ln < value1; ++ln)
	{
		a2 = a1 + ln * rateA;
		r2 = r1 + ln * rateR;
		g2 = g1 + ln * rateG;
		b2 = b1 + ln * rateB;
		pline[ln] = MAKE_ARGB_8888((BYTE)a2, (BYTE)r2, (BYTE)g2, (BYTE)b2);
	}
	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_bmp.width - x < width)	{ width = m_bmp.width - x; }
	if (m_bmp.height - y < height)	{ height = m_bmp.height - y; }

	if (!vertical)
	{
		for (s32 ly = y; ly < y + height; ++ly)
			memcpy(pTexData + (m_bmp.width * ly), pline, sizeof(DWORD) * width);
		//for (s32 lx = x; lx < x + width; ++lx)
		//{
		//	color1 = pline[lx - x];
		//	//	跳过透明像素
		//	if (!GET_ARGB_A(color1))
		//		continue;
		//	for (s32 ly = y; ly < y + height; ++ly)
		//	{
		//		color2 = pTexData[m_bmp.width * ly + lx];
		//		BLEND_ARGB_8888(color1, color2);
		//		pTexData[m_bmp.width * ly + lx] = color2;
		//	}
		//}
	}
	else
	{
		//int v;
		for (s32 ly = y; ly < y + height; ++ly)
		{
			color1 = pline[ly - y];
			int v = m_bmp.width * ly;
			for (s32 lx = x; lx < x + width; ++lx)
				pTexData[v + lx] = color1;
			//	跳过透明像素
			/*if (!GET_ARGB_A(color1))
				continue;
			for (s32 lx = x; lx < x + width; ++lx)
			{
				color2 = pTexData[m_bmp.width * ly + lx];
				BLEND_ARGB_8888(color1, color2);
				pTexData[m_bmp.width * ly + lx] = color2;
			}*/
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pline);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::clear_rect(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	int x, y, width, height;

	if (argc == 4)
	{
		for (int i = 0; i < 4; ++i)
			SafeFixnumValue(argv[i]);
		x = FIX2INT(argv[0]);
		y = FIX2INT(argv[1]);
		width = FIX2INT(argv[2]);
		height = FIX2INT(argv[3]);
	}
	else
	{
		SafeRectValue(argv[0]);
		RbRect* rect = GetObjectPtr<RbRect>(argv[0]);
		x = rect->x;
		y = rect->y;
		width = rect->width;
		height = rect->height;
	}

	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_bmp.width - x < width)	{ width = m_bmp.width - x; }
	if (m_bmp.height - y < height)	{ height = m_bmp.height - y; }

	if (width <= 0 || height <= 0)
		return Qfalse;

	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData) return Qfalse;

	for (int ly = y; ly < y + height; ++ly)
		memset(&pTexData[m_bmp.width * ly + x], 0, sizeof(DWORD) * width);

	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::blur()
{
	check_raise();
	
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	int radius = 5;
	double sigma = (double)radius / 3.0;
	long width = m_bmp.width, height = m_bmp.height;

    double *gaussMatrix, gaussSum = 0.0, _2sigma2 = 2 * sigma * sigma;
    s32 x, y, xx, yy, xxx, yyy;
    double *pdbl, a, r, g, b, d;
    DWORD *pout, *poutb;
    pout = poutb = (DWORD*)LocalAlloc(LMEM_FIXED, width * height * sizeof(DWORD));
    if (!pout) return Qfalse;
    gaussMatrix = pdbl = (double *)LocalAlloc(LMEM_FIXED, (radius * 2 + 1) * (radius * 2 + 1) * sizeof(double));
    if (!gaussMatrix)
	{
        LocalFree(pout);
        return Qfalse;
    }
    for (y = -radius; y <= radius; y++)
	{
        for (x = -radius; x <= radius; x++)
		{
            a = exp(-(double)(x * x + y * y) / _2sigma2); 
            *pdbl++ = a;
            gaussSum += a;
        }
    }
    pdbl = gaussMatrix;
    for (y = -radius; y <= radius; y++)
	{
        for (x = -radius; x <= radius; x++)
            *pdbl++ /= gaussSum;
    }
    for (y = 0; y < height; y++)
	{
        for (x = 0; x < width; x++)
		{
            a = r = g = b = 0.0;
            pdbl = gaussMatrix;
            for (yy = -radius; yy <= radius; yy++)
			{
                yyy = y + yy;
                if (yyy >= 0 && yyy < height)
				{
                    for (xx = -radius; xx <= radius; xx++)
					{
                        xxx = x + xx;
                        if (xxx >= 0 && xxx < width)
						{
                            d = *pdbl;
                            a += d * GET_ARGB_A(pTexData[xxx + yyy * width]);
                            r += d * GET_ARGB_R(pTexData[xxx + yyy * width]);
                            g += d * GET_ARGB_G(pTexData[xxx + yyy * width]);
                            b += d * GET_ARGB_B(pTexData[xxx + yyy * width]);
                        }
                        pdbl++;
                    }
                }
				else
				{
                    pdbl += (radius * 2 + 1);
                }
            }
			*pout++ = MAKE_ARGB_8888((BYTE)a, (BYTE)r, (BYTE)g, (BYTE)b);
        }
    }
    RtlMoveMemory(pTexData, poutb, width * height * sizeof(DWORD));
    LocalFree(gaussMatrix);
    LocalFree(poutb);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
    
	//	增加 修改计数值
	++m_modify_count;

	//int nRadius = 5;
	//// 定义变量
	//long diamet = (nRadius << 1) + 1;				// 采样区域直径,或者方阵的边长
	//double s = (double)nRadius / 3.0;				// 正态分布的标准偏差σ
	//double sigma2 = 2.0 * s * s;					// 2倍的σ平方,参考N维空间正态分布方程
	//double nuclear = 0.0;							// 高斯卷积核
	//double* matrix = new double[diamet * diamet];	// 高斯矩阵定义
	//DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false); // 像素内存块
	//long w = m_bmp.width, h = m_bmp.height;			// 像素矩阵的宽与高
	//// 计算高斯矩阵
	//int i = 0;
	//for(long y = -nRadius; y <= nRadius; ++y)
	//	for(long x = -nRadius; x <= nRadius; ++x)
	//	{
	//		matrix[i] = exp(-(double)(x * x + y * y) / sigma2);
	//		nuclear += matrix[i];
	//		++i;
	//	}
	//// 遍历并处理像素
	//for (long y_s = 0; y_s < h; ++y_s)
	//{
	//	for (long x_s = 0; x_s < w; ++x_s)
	//	{
	//		// 分析取样区域
	//		double r = 0.0, g = 0.0, b = 0.0;
	//		int i_m = 0;
	//		for (long m = -nRadius; m <= nRadius; ++m)
	//		{
	//			long y = y_s + m;
	//			if (y >= 0 && y < h)
	//				for (long n = -nRadius; n <= nRadius; ++n)
	//				{
	//					long x = x_s + n;
	//					if (x >= 0 && x < w)
	//					{
	//						double weight = matrix[i_m] / nuclear;
	//						long i = (h - y - 1) * w + x;
	//						r += weight * GET_ARGB_R(pTexData[i]);
	//						g += weight * GET_ARGB_G(pTexData[i]);
	//						b += weight * GET_ARGB_B(pTexData[i]);
	//					}
	//					++i_m;
	//				}
	//			else
	//				i_m += diamet;
	//		}
	//		// 保存处理结果
	//		long i_s = (h - y_s - 1) * w + x_s;
	//		pTexData[i_s] = MAKE_ARGB_8888(GET_ARGB_A(pTexData[i_s]), 
	//			(BYTE)(r > (BYTE)~0 ? (BYTE)~0 : r), 
	//			(BYTE)(g > (BYTE)~0 ? (BYTE)~0 : g), 
	//			(BYTE)(b > (BYTE)~0 ? (BYTE)~0 : b));
	//	}
	//}
	//// 清理内存
	//delete [] matrix;
	//// 解锁texture
	//GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	return Qnil;
}

VALUE RbBitmap::flip_h()
{
	check_raise();

	s32 width = m_bmp.width;
	s32 height = m_bmp.height;
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData) return Qfalse;
	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pTexData + (m_bmp.width * ly), pTempData + (m_bmp.width * (height - ly - 1)), sizeof(DWORD) * width);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::flip_v()
{
	check_raise();
	
	s32 width = m_bmp.width;
	s32 height = m_bmp.height;
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData) return Qfalse;
	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	//int v;
	for (s32 ly = 0; ly < height; ++ly)
	{
		int v = m_bmp.width * ly;
		for (s32 lx = 0; lx < width; ++lx)
			pTexData[v + lx] = pTempData[v + (width - lx - 1)];

	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::radial_blur(VALUE angle, VALUE division)
{
	check_raise();
	
	s32 width = m_bmp.width;
	s32 height = m_bmp.height;
	DWORD* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));

#pragma message("		Unfinished Function " __FUNCTION__)
	
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::get_rect()
{
	check_raise();

	return ReturnObject(m_rect_ptr);
}

VALUE RbBitmap::get_width()
{
	check_raise();

	return INT2FIX(GetWidth());
}

VALUE RbBitmap::get_height()
{
	check_raise();

	return INT2FIX(GetHeight());
}

VALUE RbBitmap::get_filename()
{
	check_raise();

	return m_filename;
}

VALUE RbBitmap::get_font()
{
	check_raise();

	return ReturnObject(m_font_ptr);
}

VALUE RbBitmap::set_font(VALUE font)
{
	check_raise();

	SafeFontValue(font);
	m_font_ptr = GetObjectPtr<RbFont>(font);
	return font;
}

VALUE RbBitmap::save_to_file(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	VALUE filepath, fileformat;

	if (rb_scan_args(argc, argv, "11", &filepath, &fileformat) == 1)
		fileformat = INT2FIX(3);
	else
		SafeFixnumValue(fileformat);

	SafeStringValue(filepath);

	LPDIRECT3DSURFACE8 pDst = NULL;
	LPDIRECT3DSURFACE8 pSrc = NULL;

	if (FAILED(GetAppPtr()->GetD3DDevicePtr()->CreateImageSurface(m_bmp.width, m_bmp.height, D3DFMT_A8R8G8B8, &pDst)))
		return Qfalse;

	POINT	pt = {0, };
	RECT	rt = {0, };

	pt.x = 0;
	pt.y = 0;
	rt.right = m_bmp.width;
	rt.bottom = m_bmp.height;

	if (FAILED(((LPDIRECT3DTEXTURE8)m_bmp.quad.tex)->GetSurfaceLevel(0, &pSrc)))
		goto failed_return;

	if (FAILED(GetAppPtr()->GetD3DDevicePtr()->CopyRects(pSrc, &rt, 1, pDst, &pt)))
		goto failed_return;

	pSrc->Release();
	pSrc = NULL;

	//	This function supports the following file formats: .bmp and .dds.
	if (SUCCEEDED(D3DXSaveSurfaceToFileW(Kconv::UTF8ToUnicode(RSTRING_PTR(filepath)), 
		D3DXIFF_BMP, pDst, NULL, NULL)))
	{
		pDst->Release();
		pDst = NULL;
		return Qtrue;
	}
	
failed_return:
	if (pDst)
	{
		pDst->Release();
		pDst = NULL;
	}
	if (pSrc)
	{
		pSrc->Release();
		pSrc = NULL;
	}
	return Qfalse;
}

imp_method(RbBitmap, dispose)
imp_method(RbBitmap, is_disposed)
imp_method_vargs(RbBitmap, save_to_file)
imp_method01(RbBitmap, hue_change)
imp_method01(RbBitmap, brightness_change)
imp_method_vargs(RbBitmap, tone_change)

imp_method_vargs(RbBitmap, blt)
imp_method_vargs(RbBitmap, stretch_blt)
imp_method_vargs(RbBitmap, fill_rect)
imp_method(RbBitmap, clear)
imp_method02(RbBitmap, get_pixel)
imp_method03(RbBitmap, set_pixel)
imp_method_vargs(RbBitmap, draw_text)
imp_method01(RbBitmap, text_size)

imp_method_vargs(RbBitmap, gradient_fill_rect)
imp_method_vargs(RbBitmap, clear_rect)
imp_method(RbBitmap, blur)
imp_method02(RbBitmap, radial_blur)

imp_method(RbBitmap, flip_h)
imp_method(RbBitmap, flip_v)

imp_attr_reader(RbBitmap, rect)
imp_attr_reader(RbBitmap, width)
imp_attr_reader(RbBitmap, height)
imp_attr_reader(RbBitmap, filename)
imp_attr_accessor(RbBitmap, font)