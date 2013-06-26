/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Bitmap
*/
#include "sin_color.h"
#include "RbExport.h"
#include "CRbBitmap.h"
#include "CRbColor.h"
#include "CRbRect.h"
#include "CRbFont.h"
#include "CRbTone.h"
#include "sin_app.h"
#include <math.h>


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

CRbBitmap::CRbBitmap()
	: m_disposed(true)
	, m_modify_count(0)
	, m_filename(Qnil)
	, m_rect_ptr(0)
	//, m_font_ptr(0)
{
}

CRbBitmap::~CRbBitmap()
{
	if (m_image.quad.tex)
	{
		GetAppPtr()->GetHgePtr()->Texture_Free(m_image.quad.tex);
		m_image.quad.tex = 0;
	}
}

void CRbBitmap::InitLibrary()
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
	rb_define_alloc_func(rb_cBitmap, ObjAllocate<CRbBitmap>);
	rb_define_method(rb_cBitmap, "initialize",			(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cBitmap, "dispose",				(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cBitmap, "disposed?",			(RbFunc)dm_is_disposed,			0);
	rb_define_method(rb_cBitmap, "save_to_file",		(RbFunc)dm_save_to_file,		-1);

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
	rb_define_method(rb_cBitmap, "text_size",			(RbFunc)dm_text_size,			2);

	rb_define_method(rb_cBitmap, "gradient_fill_rect",	(RbFunc)dm_gradient_fill_rect,	-1);
	rb_define_method(rb_cBitmap, "clear_rect",			(RbFunc)dm_clear_rect,			-1);
	rb_define_method(rb_cBitmap, "blur",				(RbFunc)dm_blur ,				0);
	rb_define_method(rb_cBitmap, "radial_blur",			(RbFunc)dm_radial_blur,			2);
	
	rb_define_method(rb_cBitmap, "flip_h",				(RbFunc)dm_flip_h ,				0);
	rb_define_method(rb_cBitmap, "flip_v",				(RbFunc)dm_flip_v ,				0);
	rb_define_method(rb_cBitmap, "to_str",				(RbFunc)dm_to_str ,				0);

	// object attribute
	rb_define_method(rb_cBitmap, "width",				(RbFunc)dm_get_width,			0);
	rb_define_method(rb_cBitmap, "height",				(RbFunc)dm_get_height,			0);
	rb_define_method(rb_cBitmap, "rect",				(RbFunc)dm_get_rect,			0);
	rb_define_method(rb_cBitmap, "filename",			(RbFunc)dm_get_filename,		0);

	/*rb_define_method(rb_cBitmap, "font",				(RbFunc)dm_get_font,			0);
	rb_define_method(rb_cBitmap, "font=",				(RbFunc)dm_set_font,			1);*/

	// supplement
 	rb_define_method(rb_cBitmap, "to_s",				(RbFunc)dm_to_string,			0);
	rb_define_method(rb_cBitmap, "clone",				(RbFunc)dm_clone,				0);
}

void CRbBitmap::mark()
{
	if (m_rect_ptr)	m_rect_ptr->MarkObject();
	//if (m_font_ptr)	m_font_ptr->MarkObject();

	if (RTEST(m_filename))
		rb_gc_mark(m_filename);
}

VALUE CRbBitmap::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE arg01, arg02;

	u32 dwColorValue = 0;
	HGE * hge = GetAppPtr()->GetHgePtr();
	hgeQuad quad;

	if (rb_scan_args(argc, argv, "11", &arg01, &arg02) == 1)
	{
		SafeStringValue(arg01);
		goto __bitmap_load;
	}
	else
	{
		if (TYPE(arg01) == T_STRING)
		{
			if (FIXNUM_P(arg02))
				goto __bitmap_load_from_buffer;

			SafeColorValue(arg02);
			{
				CRbColor * ccol;
				ccol = GetObjectPtr<CRbColor>(arg02);
				dwColorValue = ccol->GetColor();
			}
			goto __bitmap_load;
		}
		else goto __bitmap_create;
	}

__bitmap_load_from_buffer:
	{
		wchar_t * data = (wchar_t *)RSTRING_PTR(arg01);
		int size = FIX2INT(arg02);
		quad.tex = hge->Texture_Load(data, size);
		data = NULL;
		if(!quad.tex)
			rb_raise(rb_eSinError, "Failed to create bitmap from buffer.");
	}
	goto __finish;

__bitmap_load:
	{
		int suffix_idx = -1;
		static char * szSuffixA[] = { ".png",  ".jpg",  ".bmp",  ".tga",  ".dds",  ".dib"};
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

		/*if (w > GetAppPtr()->GetMaxTexW() || h > GetAppPtr()->GetMaxTexH())
			rb_raise(rb_eSinError, "Too large width or height, Graphics card won't support.\n %d x %d", w, h);*/

		quad.tex = hge->Texture_Create(w, h);
		if(!quad.tex)
			rb_raise(rb_eSinError, "Failed to create bitmap: %d x %d .", w, h);
	}
	goto __finish;

__finish:
	{
		QUAD_INIT(quad);
		
		m_image.quad = quad;
		m_image.width = hge->Texture_GetWidth(quad.tex);
		m_image.height = hge->Texture_GetHeight(quad.tex);
		m_image.owidth = hge->Texture_GetWidth(quad.tex, true);
		m_image.oheight = hge->Texture_GetHeight(quad.tex, true);

		//	create rect
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(m_image.width), INT2FIX(m_image.height)};
		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<CRbRect>(rect);
		// create a font object for the bitmap-obj
		/*VALUE font = rb_class_new_instance(0, 0, rb_cFont);
		m_font_ptr = GetObjectPtr<CRbFont>(font);*/
		m_disposed = false;
	}
	return obj;
}

VALUE CRbBitmap::clone()
{
	check_raise();

	VALUE __argv[] = { INT2FIX(m_image.width), INT2FIX(m_image.height) };
	VALUE bitmap = rb_class_new_instance(2, __argv, obj_class());
	CRbBitmap * bmp_ptr = GetObjectPtr<CRbBitmap>(bitmap);

	HGE * hge = GetAppPtr()->GetHgePtr();
	//memcpy(bmp_ptr->GetBitmapPtr(), &m_image, sizeof(image_t));
	u32 * pSrcTexData = hge->Texture_Lock(m_image.quad.tex, true);
	u32 * pDesTexData = hge->Texture_Lock(bmp_ptr->m_image.quad.tex, false);
	if (!pSrcTexData || !pDesTexData)
		rb_raise(rb_eSinError, "Failed to clone bitmap.");

	memcpy(pDesTexData, pSrcTexData, m_image.width * m_image.height * sizeof(u32));
	hge->Texture_Unlock(m_image.quad.tex);
	hge->Texture_Unlock(bmp_ptr->m_image.quad.tex);

	bmp_ptr->m_filename = m_filename;
	//bmp_ptr->m_font_ptr = m_font_ptr;
	bmp_ptr->m_modify_count = m_modify_count;
	bmp_ptr->m_rect_ptr = m_rect_ptr;
	
	bmp_ptr = NULL;
	return bitmap;
}

HTEXTURE CRbBitmap::LoadTexture(const wchar_t * filename, u32 colorKey, int &suffix_idx)
{
	HGE* hge = GetAppPtr()->GetHgePtr();
	HTEXTURE tex = hge->Texture_Load(filename, 0, false, colorKey);
	if (!tex)
	{
		void * data;
		u32 size;

		static wchar_t * szSuffix[]	= {L".png", L".jpg", L".bmp", L".tga", L".dds", L".dib"};
		static int		uSuffixCnt	= SinArrayCount(szSuffix);
		
		if (data = hge->Resource_Load_Without_Suffix(filename, &size, szSuffix, uSuffixCnt, &suffix_idx))
		{
			tex = hge->Texture_Load((const wchar_t *)data, size, false, colorKey);
			
			if(!tex)
				rb_raise(rb_eSinError, "Failed to load bitmap `%s'.", Kconv::UnicodeToAnsi(filename));

			// free
			hge->Resource_Free(data);
			return tex;
		}
		else
			rb_raise(rb_eSinError, "Failed to load bitmap `%s'.", Kconv::UnicodeToAnsi(filename));
	}
	return tex;
}

void CRbBitmap::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed bitmap");
}

bool CRbBitmap::AdjustTexturesTone(const image_p pimage, u32 dwTone)
{
	if (!dwTone)
		return true;
	
	BYTE a1, r1, g1, b1, a2, r2, g2, b2;
	GET_ARGB_8888(dwTone, a1, r1, g1, b1);

	u32 * pSrcTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(pimage->quad.tex, false);
	if (!pSrcTexData)
		return false;
	int tmp_i;
	for (s32 x = 0; x < pimage->width; ++x)
	{
		for (s32 y = 0; y < pimage->height; ++y)
		{
			tmp_i = pimage->width * y + x;
			GET_ARGB_8888(pSrcTexData[tmp_i], a2, r2, g2, b2);
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
			pSrcTexData[tmp_i] = MAKE_ARGB_8888(a2, r2, g2, b2);
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(pimage->quad.tex);

	return true;
}

bool CRbBitmap::AdjustTexturesToneDouble(const image_p pSrcBmp, const HTEXTURE pDstTex, u32 dwTone)
{
	u32 * pSrcTexData = 0;
	u32 * pDstTexData = 0;

	if (!pSrcBmp->quad.tex)
		return false;

	if (!pDstTex)
		return false;
	
	HGE * hge = GetAppPtr()->GetHgePtr();
	
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

void CRbBitmap::ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V)
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

void CRbBitmap::ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B)
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

bool CRbBitmap::GetTextRect(HFONT hFont, const wchar_t * pStr, s32 &cx, s32 &cy, HDC hDC)
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

image_p CRbBitmap::CloneImage(image_p pimage)
{
	HGE * hge = GetAppPtr()->GetHgePtr();
	image_p clone = (image_p)malloc(sizeof(image_t));
	memcpy(clone, pimage, sizeof(image_t));
	clone->quad.tex = hge->Texture_Create(pimage->width, pimage->height);
	u32 * pSrcTexData = hge->Texture_Lock(pimage->quad.tex, true);
	u32 * pDesTexData = hge->Texture_Lock(clone->quad.tex, false);
	memcpy(pDesTexData, pSrcTexData, pimage->width * pimage->height * sizeof(u32));
	hge->Texture_Unlock(pimage->quad.tex);
	hge->Texture_Unlock(clone->quad.tex);
	return clone;
}

HTEXTURE CRbBitmap::CutTexture(int x, int y, int width, int height, image_p pimage)
{
	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (pimage->width - x < width)	{ width = pimage->width - x; }
	if (pimage->height - y < height)	{ height = pimage->height - y; }

	if (width <= 0 || height <= 0)
		return 0;
	
	HGE * hge = GetAppPtr()->GetHgePtr();

	HTEXTURE cut = hge->Texture_Create(width, height);
	u32 * pSrcTexData = hge->Texture_Lock(pimage->quad.tex, true);//, x, y, width, height);
	u32 * pDesTexData = hge->Texture_Lock(cut, false);
	//memcpy(pDesTexData, pSrcTexData, width * height *sizeof(u32));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pDesTexData + (width * ly), pSrcTexData + (pimage->width * (y + ly) + x), sizeof(u32) * width);
	hge->Texture_Unlock(pimage->quad.tex);
	hge->Texture_Unlock(cut);
	return cut;
}

void CRbBitmap::BilinearZoom(u32 * srcData, u32 * desData, int srcWidth, int srcHeight, int desWidth, int desHeight, int mathWidth, int mathHeight)
{
	long lx, ly, ty1, ty2, tx1, tx2, px1, px2, py1, py2, p11, p21, p12, p22;
	
	//u32 color11, color21, color12, color22;
	BYTE a, r, g, b;
	BYTE a11, r11, g11, b11;
	BYTE a21, r21, g21, b21;
	BYTE a12, r12, g12, b12;
	BYTE a22, r22, g22, b22;
	for (ly = 0; ly < desHeight; ly++)
	{
		// 获取y坐标
		ty1 = ly * srcHeight / mathHeight;
		ty2 = ty1 + 1;
		// 获取y权
		py2 = ly * srcHeight % mathHeight * 100 / mathHeight;
		py1 = 100 - py2;
		for (lx = 0; lx < desWidth; lx++)
		{
			tx1 = lx * srcWidth / mathWidth;
			tx2 = tx1 + 1;
			// 获取x权
			px2 = lx * srcWidth % mathWidth * 100 / mathWidth;
			px1 = 100 - px2;
			// 获取四点颜色对应的权
			p11 = px1 * py1;
			p21 = px2 * py1;
			p12 = px1 * py2;
			p22 = px2 * py2;
			// 获取颜色
			GET_ARGB_8888(srcData[tx1 + ty1 * srcWidth], a11, r11, g11, b11);
			GET_ARGB_8888(srcData[tx2 + ty1 * srcWidth], a21, r21, g21, b21);
			GET_ARGB_8888(srcData[tx1 + ty2 * srcWidth], a12, r12, g12, b12);
			GET_ARGB_8888(srcData[tx2 + ty2 * srcWidth], a22, r22, g22, b22);
			// 计算颜色
			a = (a11 * p11 + a21 * p21 + a12 * p12 + a22 * p22) / 10000;
			r = (r11 * p11 + r21 * p21 + r12 * p12 + r22 * p22) / 10000;
			g = (g11 * p11 + g21 * p21 + g12 * p12 + g22 * p22) / 10000;
			b = (b11 * p11 + b21 * p21 + b12 * p12 + b22 * p22) / 10000;
			desData[lx + ly * desWidth] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
}

bool CRbBitmap::ScreenToBitmap(image_p pimage)
{
	int width, height;
	HGE * hge = GetAppPtr()->GetHgePtr();
	u32 * pSrcData = hge->System_Snapshot(width, height);

	if (!pSrcData)
		return false;

	if (pimage->quad.tex)
		GetAppPtr()->GetHgePtr()->Texture_Free(pimage->quad.tex);

	pimage->quad.tex = hge->Texture_Create(GetAppPtr()->GetFrameWidth(), height);
	u32* pDesData = hge->Texture_Lock(pimage->quad.tex, false);
	//BYTE a, r, g, b;
	int tmp_i;
	for (int ly = 0; ly < height; ++ly)
	{
		tmp_i = ly * width;
		int i = GetAppPtr()->GetFrameWidth() * ly;
		for (int lx = 0; lx < GetAppPtr()->GetFrameWidth(); ++lx)
		{
			//GET_ARGB_8888(pSrcData[tmp_i + lx], a, r, g, b);
			pDesData[i + lx] = SETA(pSrcData[tmp_i + lx], 255);//MAKE_ARGB_8888(255, r, g, b);
		}
	}
		//memcpy(pDesData + GetAppPtr()->GetFrameWidth() * ly, pSrcData + width * ly, GetAppPtr()->GetFrameWidth() * sizeof(u32));
	hge->Texture_Unlock(pimage->quad.tex);
	free(pSrcData);

	pimage->width	= GetAppPtr()->GetFrameWidth();//hge->Texture_GetWidth(pimage->quad.tex, true);
	pimage->height	= height;//hge->Texture_GetHeight(pimage->quad.tex, true);

	return true;
}

VALUE CRbBitmap::dispose()
{
	if (m_disposed)
		return Qnil;

	if(m_image.quad.tex)
	{
		GetAppPtr()->GetHgePtr()->Texture_Free(m_image.quad.tex);
		m_image.quad.tex = 0;
	}

	m_disposed = true;

	return Qnil;
}

VALUE CRbBitmap::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE CRbBitmap::hue_change(VALUE hue)
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

	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	int tmp_i;
	for (s32 x = 0; x < m_image.width; ++x)
	{
		for (s32 y = 0; y < m_image.height; ++y)
		{
			tmp_i = m_image.width * y + x;
			GET_ARGB_8888(pTexData[tmp_i], a, r, g, b);
			/*r = (BYTE)SinBound(r*matrix[0]+g*matrix[1]+b*matrix[2], 0, 255);
			g = (BYTE)SinBound(r*matrix[3]+g*matrix[4]+b*matrix[5], 0, 255);
			b = (BYTE)SinBound(r*matrix[6]+g*matrix[7]+b*matrix[8], 0, 255);*/
			ColorSpaceRGB2HSV(r, g, b, h, s, l);
			h += iHue;
			ColorSpaceHSV2RGB(h, s, l, r, g, b);
			pTexData[tmp_i] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::brightness_change(VALUE brightness)
{
	check_raise();

	SafeFixnumValue(brightness);
	int iBrightness = FIX2INT(brightness);
	if(!iBrightness) return Qnil;
	iBrightness = SinBound(iBrightness, -255, 255);

	BYTE a, r, g, b;
	u32* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	int tmp_i1, tmp_i2;
	for (s32 y = 0; y < m_image.height; ++y)
	{
		tmp_i1 = m_image.width * y;
		for (s32 x = 0; x < m_image.width; ++x)
		{
			tmp_i2 = tmp_i1 + x;
			GET_ARGB_8888(pTexData[tmp_i2], a, r, g, b);
			r = sTable768_mid[r + iBrightness + 256];
			g = sTable768_mid[g + iBrightness + 256];
			b = sTable768_mid[b + iBrightness + 256];
			pTexData[tmp_i2] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::tone_change(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	int a, r, g, b;
	VALUE red, green, blue, gray;
	u32 tone = 0;

	if (argc == 1)
	{
		if (rb_obj_is_kind_of(argv[0], rb_cInteger))
			tone = NUM2ULONG(argv[0]);
		else
		{
			SafeToneValue(argv[0]);
			tone = GetObjectPtr<CRbTone>(argv[0])->GetColor();
		}
	}
	else
	{
		rb_scan_args(argc, argv, "4", &red, &green, &blue, &gray);
		for (int i = 0; i < argc; ++i)
			SafeFixnumValue(argv[i]);

		r = (NIL_P(red) ? 255 : FIX2INT(red));
		g = (NIL_P(green) ? 255 : FIX2INT(green));
		b = (NIL_P(blue) ? 255 : FIX2INT(blue));
		a = (NIL_P(gray) ? 255 : FIX2INT(gray));

		r = SinBound(r, 0, 255);
		g = SinBound(g, 0, 255);
		b = SinBound(b, 0, 255);
		a = SinBound(a, 0, 255);

		tone = MAKE_ARGB_8888(a, r, g, b);
	}
	AdjustTexturesTone(&m_image, tone);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::blt(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	VALUE x, y, src_bitmap, src_rect, opacity;
	image_p src;
	bool with_self = false;

	if (rb_obj_class((argv[2])) == rb_cBitmap)
	{
		rb_scan_args(argc, argv, "41", &x, &y, &src_bitmap, &src_rect, &opacity);

		SafeBitmapValue(src_bitmap);
		CRbBitmap * srcBmp = GetObjectPtr<CRbBitmap>(src_bitmap);
		src = srcBmp->GetBitmapPtr();
	}
	else
	{
		rb_scan_args(argc, argv, "31", &x, &y, &src_rect, &opacity);
		src = &m_image;
		with_self = true;
	}
	SafeFixnumValue(x);
	SafeFixnumValue(y);
	SafeRectValue(src_rect);

	image_p des = &m_image;
	CRbRect* srcRect = GetObjectPtr<CRbRect>(src_rect);
	int sx = srcRect->m_rect_data[0];
	int sy = srcRect->m_rect_data[1];
	int sw = srcRect->m_rect_data[2];
	int sh = srcRect->m_rect_data[3];
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

	u32 * pTempData;
	u32 * pSrcTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(src->quad.tex, true);
	if (with_self)
	{
		pTempData = (u32 *)malloc(src->width * src->height * sizeof(u32));
		memcpy(pTempData, pSrcTexData, src->width * src->height * sizeof(u32));
		GetAppPtr()->GetHgePtr()->Texture_Unlock(src->quad.tex);
	}
	else
	{
		pTempData = pSrcTexData;
	}
	u32* pDstTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(des->quad.tex,false);
	if (!pTempData || !pDstTexData)
		return Qfalse;
	u32 color1, color2;
	BYTE a, r, g, b;
	int tmp_i;
	for (int lx = sx; lx < sx + sw; ++lx)
	{
		for (int ly = sy; ly < sy + sh; ++ly)
		{
			tmp_i = des->width * (ly - sy + dy) + lx - sx + dx;
			color1 = pTempData[src->width * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	跳过透明像素
			if (!a) continue;
			a = a * op / 255;
			//	跳过透明像素
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[tmp_i];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[tmp_i] = color2;
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

VALUE CRbBitmap::stretch_blt(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	VALUE dest_rect, src_bitmap, src_rect, opacity;
	image_p src;
	rb_scan_args(argc, argv, "31", &dest_rect, &src_bitmap, &src_rect, &opacity);
	
	SafeRectValue(dest_rect);
	SafeBitmapValue(src_bitmap);
	SafeRectValue(src_rect);
	
	CRbRect * desRect = GetObjectPtr<CRbRect>(dest_rect);
	int dx = desRect->m_rect_data[0];
	int dy = desRect->m_rect_data[1];
	int dw = desRect->m_rect_data[2];
	int dh = desRect->m_rect_data[3];
	CRbBitmap * srcBmp = GetObjectPtr<CRbBitmap>(src_bitmap);
	src = srcBmp->GetBitmapPtr();
	CRbRect * srcRect = GetObjectPtr<CRbRect>(src_rect);
	int sx = srcRect->m_rect_data[0];
	int sy = srcRect->m_rect_data[1];
	int sw = srcRect->m_rect_data[2];
	int sh = srcRect->m_rect_data[3];
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
	if (m_image.width - dx <= 0) return Qfalse;
	if (m_image.height - dy <= 0) return Qfalse;
	
	HGE* hge = GetAppPtr()->GetHgePtr();

	HTEXTURE tmpTex;
	/*if (sx == 0 && sy == 0 && sw == src->width && sh == src->height)
		tmpTex = src->quad.tex;
	else*/
	tmpTex = CutTexture(sx, sy, sw, sh, src);

	if (!tmpTex) return Qfalse;

	u32 * pSrcTexData = hge->Texture_Lock(tmpTex, true);
	if (!pSrcTexData) return Qfalse;
	u32 * pTempData = (u32 *)malloc(dw * dh * sizeof(u32));
	int mathW = dw + (int)ceil(sqrt(sqrt((double)dw / (double)sw)));
	int mathH = dh + (int)ceil(sqrt(sqrt((double)dh / (double)sh)));
	BilinearZoom(pSrcTexData, pTempData, sw, sh, dw, dh, mathW, mathH);

	u32 * pDstTexData = hge->Texture_Lock(m_image.quad.tex, false);
	u32 color1, color2;
	BYTE a, r, g, b;

	int ndw = dw;
	if (dw + dx > m_image.width) dw = m_image.width - dx;
	if (dh + dy > m_image.height) dh = m_image.height - dy;
	int tmp_i;
	for (s32 lx = 0; lx < dw; ++lx)
	{
		for (s32 ly = 0; ly < dh; ++ly)
		{
			tmp_i = m_image.width * (ly + dy) + lx + dx;
			color1 = pTempData[ndw * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	跳过透明像素
			if (!a) continue;
			a = a * op / 255;
			//	跳过透明像素
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[tmp_i];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[tmp_i] = color2;
		}
	}
	hge->Texture_Unlock(m_image.quad.tex);
	hge->Texture_Unlock(tmpTex);
	//if (sx != 0 || sy != 0 || sw != src->width || sh != src->height)
	hge->Texture_Free(tmpTex);
	tmpTex = 0;
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::fill_rect(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	int x, y, width, height;
	u32 color;

	if (argc == 5)
	{
		for (int i = 0; i < 4; ++i)
			SafeFixnumValue(argv[i]);
		
		x = FIX2INT(argv[0]);
		y = FIX2INT(argv[1]);
		width = FIX2INT(argv[2]);
		height = FIX2INT(argv[3]);

		if (rb_obj_is_kind_of(argv[4], rb_cInteger))
			color = NUM2ULONG(argv[4]);
		else
		{
			SafeColorValue(argv[4]);
			color = GetObjectPtr<CRbColor>(argv[4])->GetColor();
		}
	}
	else if (argc == 2)
	{
		SafeRectValue(argv[0]);

		CRbRect * rect = GetObjectPtr<CRbRect>(argv[0]);
		x = rect->m_rect_data[0];
		y = rect->m_rect_data[1];
		width = rect->m_rect_data[2];
		height = rect->m_rect_data[3];

		if (rb_obj_is_kind_of(argv[1], rb_cInteger))
			color = NUM2ULONG(argv[1]);
		else
		{
			SafeColorValue(argv[1]);
			color = GetObjectPtr<CRbColor>(argv[1])->GetColor();
		}
	}
	else
		rb_raise(rb_eArgError, "wrong number of arguments (%d for 2 or 5)", argc);

	//	跳过透明像素
	/*if (!GET_ARGB_A(color))
		return Qnil;*/

	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_image.width - x < width)		{ width = m_image.width - x; }
	if (m_image.height - y < height)	{ height = m_image.height - y; }

	if (width <= 0 || height <= 0)
		return Qfalse;

	//	填充
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	
	int tmp_i;
	for (s32 ly = y; ly < y + height; ++ly)
	{
		tmp_i = m_image.width * ly;
		for (s32 lx = x; lx < x + width; ++lx)
			pTexData[tmp_i + lx] = color;
	}
	for (s32 ly = y + 1; ly < y + height; ++ly)
		memcpy(pTexData + (m_image.width * ly) + x, pTexData + (m_image.width * y + x), sizeof(u32) * width);
	//u32 color2;
	//for (s32 lx = x; lx < x + width; ++lx)
	//{
	//	for (s32 ly = y; ly < y + height; ++ly)
	//	{
	//		color2 = pTexData[m_image.width * ly + lx];
	//		BLEND_ARGB_8888(color, color2);
	//		pTexData[m_image.width * ly + lx] = color2;
	//	}
	//}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::clear()
{
	check_raise();

	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	/*for (int y = 0; y < m_image.height; ++y)
		memset(&pTexData[m_image.width * y], 0, sizeof(u32) * m_image.width);*/
	ZeroMemory(pTexData, sizeof(u32) * m_image.width * m_image.height);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::get_pixel(VALUE x, VALUE y)
{
	check_raise();
	
	SafeFixnumValue(x);
	SafeFixnumValue(y);
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);
	if (dx < 0 || dy < 0 || dx >= m_image.width || dy >= m_image.height)
		return Qnil;

	int a, r, g, b;
	
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qnil;
	GET_ARGB_8888(pTexData[m_image.width * dy + dx], a, r, g, b);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);

	VALUE __argv[] = {INT2FIX(r), INT2FIX(g), INT2FIX(b), INT2FIX(a)};
	VALUE pixel = rb_class_new_instance(4, __argv, rb_cColor);
	return pixel;
}

VALUE CRbBitmap::set_pixel(VALUE x, VALUE y, VALUE color)
{
	check_raise();

	SafeFixnumValue(x);
	SafeFixnumValue(y);
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);
	if (dx < 0 || dy < 0 || dx >= m_image.width || dy >= m_image.height)
		return Qnil;
	
	u32 col;
	if (rb_obj_is_kind_of(color, rb_cInteger))
		col = NUM2ULONG(color);
	else
	{
		SafeColorValue(color);
		col = GetObjectPtr<CRbTone>(color)->GetColor();
	}
	//	跳过透明像素
	/*if (!GET_ARGB_A(dwColor))
		return Qfalse;*/

	//u32 color2;
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	//color2 = pTexData[m_image.width * dy + dx];
	//BLEND_ARGB_8888(dwColor, color2);
	pTexData[m_image.width * dy + dx] = col;//color2;
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::draw_text(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	VALUE font, rect, vstr, ha, va, vx, vy, vw, vh;
	int ix, iy, width, height;
	wchar_t * pStr;
	int halign, valign;

	if (FIXNUM_P(argv[1]))
	{
		rb_scan_args(argc, argv, "62", &font, &vx, &vy, &vw, &vh, &vstr, &ha, &va);
		SafeFixnumValue(vx);
		SafeFixnumValue(vy);
		SafeFixnumValue(vw);
		SafeFixnumValue(vh);
		ix = FIX2INT(vx);
		iy = FIX2INT(vy);
		width = FIX2INT(vw);
		height = FIX2INT(vh);
	}
	else
	{
		rb_scan_args(argc, argv, "32", &font, &rect, &vstr, &ha, &va);
		SafeRectValue(rect);
		CRbRect * prect = GetObjectPtr<CRbRect>(rect);
		ix = prect->m_rect_data[0];
		iy = prect->m_rect_data[1];
		width = prect->m_rect_data[2];
		height = prect->m_rect_data[3];
		prect = NULL;
	}
	
	SafeFontValue(font);
	CRbFont * pfont = GetObjectPtr<CRbFont>(font);

	VALUE vStr = NIL_P(vstr) ? rb_str_new2("nil") : rb_obj_as_string(vstr);
	pStr = Kconv::UTF8ToUnicode(RSTRING_PTR(vStr));
	halign = NIL_P(ha) ? 0 : FIX2INT(ha);
	valign = NIL_P(va) ? 1 : FIX2INT(va);

	if (width <= 0 || height <= 0) return Qfalse;

	int iCharCount = wcslen(pStr);
	if (!iCharCount) return Qfalse;

	u32 draw_color = pfont->GetColorPtr()->GetColor();
	BYTE da = GET_ARGB_A(draw_color);
	u32 drgb = draw_color & 0x00FFFFFF;
	if (!da) return Qfalse;

	bool shadow = pfont->IsShadow();
	u32 color1, color2 = 0;
	HGE * hge = GetAppPtr()->GetHgePtr();

	//	开始正式描绘文字
	HDC	hScreenDC = NULL;
	/*do
	{*/
	s32 cx, cy;
	if (pfont->IsLattice())
	{
		cx = pfont->GetTextWidth(pStr);
		cy = FIX2INT(pfont->m_size);
		if (!cx || !cy) return Qfalse;
	}
	else
	{
		hScreenDC = GetDC(NULL);
		if (!GetTextRect(pfont->GetHFont(), pStr, cx, cy, hScreenDC))
		{
			ReleaseDC(NULL, hScreenDC);
			return Qfalse;
		}
	}
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
		case 0:offset_y = 0;				break;
		case 1:offset_y = (height - cy) / 2;break;
		case 2:offset_y = (height - cy);	break;
	}

	//	锁定纹理
	u32 * pTexData = hge->Texture_Lock(m_image.quad.tex, false);
	//	锁定失败：退出
	if (!pTexData)
	{
		if (hScreenDC) ReleaseDC(NULL, hScreenDC);
		return Qfalse;
	}
	int iCharOffset = 0;
	//	临时变量
	int tmp_x, tmp_y;
	if (pfont->IsLattice())
	{
		if (offset_x < 0) offset_x = 0;
		if (offset_y < 0) offset_y = 0;
		u8 fc = 0;
		u32 offset = 0;
		int fsize = FIX2INT(pfont->m_size);
		int fwidth = fsize == 14 ? 16 : fsize;
		//	循环逐字描绘
		for (int i = 0; i < iCharCount; ++i)
		{
			u32 char_index = pStr[i];
			s32 o1 = ix + offset_x + iCharOffset;
			tmp_x = 0, tmp_y = 0;
			for (u32 ii = 0; ii < pfont->m_count; ++ii)
			{
				fc = pfont->m_font_data[char_index * pfont->m_count + ii];
				for (int l = 0; l < 8; ++l)
				{
					if (tmp_x >= fwidth)
					{
						++tmp_y;
						tmp_x = 0;
					}
					s32 o2 = iy + offset_y + tmp_y;
					if (o1 + tmp_x < 0 || o1 + tmp_x >= m_image.width || o2 < 0 || o2 >= m_image.height)
						continue;
					offset = o2 * m_image.width + o1 + tmp_x;
					if (fc & (1 << l))
					{
						color2 = pTexData[offset];
						BLEND_ARGB_8888(draw_color, color2);
						pTexData[offset] = color2;
					}
					++tmp_x;
				}
			}
			iCharOffset += char_index < 128 ? fsize / 2 : fsize;
		}
	}
	else
	{
		GLYPHMETRICS	gm;
		TEXTMETRIC		tm;

		static MAT2		mmat2 = {0, 1, 0, 0, 0, 0, 0, 1};

		static BYTE		alphatable65[] = // 65.times{|i| 255 * i / (65 - 1)
										{0,3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63,67,71,75,79,83,
										87,91,95,99,103,107,111,115,119,123,127,131,135,139,143,147,151,
										155,159,163,167,171,175,179,183,187,191,195,199,203,207,211,215,
										219,223,227,231,235,239,243,247,251,255};

		static BYTE		buffer[1024];

		HFONT hOldFont = (HFONT)SelectObject(hScreenDC, pfont->GetHFont());
		//	循环逐字描绘
		for (int i = 0; i < iCharCount; ++i)
		{
			u32 dwBufferSize = GetGlyphOutline(hScreenDC, pStr[i], GGO_GRAY8_BITMAP, &gm, 0, NULL, &mmat2);

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
					int h1 = (tmp_y + 1) * m_image.width;
					int h2 = tmp_y * m_image.width;
					for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
					{
						//	过滤掉越界的像素
						tmp_x = x + gm.gmptGlyphOrigin.x + iCharOffset + ix + offset_x;
						if (tmp_x < 0 || tmp_x >= m_image.width || tmp_y < 0 || tmp_y >= m_image.height)
							continue;

						//	过滤全透明区域
						BYTE tmp_gray = alphatable65[buffer[y * stride + x]];
						if (!tmp_gray) continue;

						tmp_gray = (BYTE)((float)tmp_gray * da / 255);
						if (!tmp_gray) continue;

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
		SelectObject(hScreenDC, hOldFont);
	}
	// 取消锁定
	hge->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;
	//} while (0);
	pfont = NULL;
	return Qnil;
}

VALUE CRbBitmap::text_size(VALUE font, VALUE str)
{
	SafeFontValue(font);
	CRbFont * pfont = GetObjectPtr<CRbFont>(font);

	VALUE vStr = NIL_P(str) ? rb_str_new2("nil") : rb_obj_as_string(str);
	wchar_t * pStr = Kconv::UTF8ToUnicode(RSTRING_PTR(vStr));
	
	s32 cx = 0, cy = 0;
	VALUE rect;

	if (pfont->IsLattice())
	{
		cx = pfont->GetTextWidth(pStr);
		cy = FIX2INT(pfont->m_size);
	}
	else
	{
		if (GetTextRect(pfont->GetHFont(), pStr, cx, cy, NULL))
			if (pfont->IsShadow())
				cy += 1;
	}
	VALUE __argv[] = {RUBY_0, RUBY_0, LONG2FIX(cx), LONG2FIX(cy)};
	rect = rb_class_new_instance(4, __argv, rb_cRect);
	pfont = NULL;
	return rect;
}

VALUE CRbBitmap::gradient_fill_rect(int argc, VALUE * argv, VALUE obj)
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

		CRbRect * rect = GetObjectPtr<CRbRect>(argv[0]);
		x = rect->m_rect_data[0];
		y = rect->m_rect_data[1];
		width = rect->m_rect_data[2];
		height = rect->m_rect_data[3];
	}
	
	if (m_image.width - x <= 0 || m_image.height - y <= 0)
		return Qfalse;

	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qfalse;

	u32 color1, color2;
	if (rb_obj_is_kind_of(vcolor1, rb_cInteger))
		color1 = NUM2ULONG(vcolor1);
	else
	{
		SafeColorValue(vcolor1);
		color1 = GetObjectPtr<CRbColor>(vcolor1)->GetColor();
	}
	if (rb_obj_is_kind_of(vcolor2, rb_cInteger))
		color2 = NUM2ULONG(vcolor2);
	else
	{
		SafeColorValue(vcolor2);
		color2 = GetObjectPtr<CRbColor>(vcolor2)->GetColor();
	}

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

	u32* pline = (u32 *)malloc(value1 * sizeof(u32));
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
	if (m_image.width - x < width)	{ width = m_image.width - x; }
	if (m_image.height - y < height)	{ height = m_image.height - y; }

	if (!vertical)
	{
		for (s32 ly = y; ly < y + height; ++ly)
			memcpy(pTexData + (m_image.width * ly), pline, sizeof(u32) * width);
		//for (s32 lx = x; lx < x + width; ++lx)
		//{
		//	color1 = pline[lx - x];
		//	//	跳过透明像素
		//	if (!GET_ARGB_A(color1))
		//		continue;
		//	for (s32 ly = y; ly < y + height; ++ly)
		//	{
		//		color2 = pTexData[m_image.width * ly + lx];
		//		BLEND_ARGB_8888(color1, color2);
		//		pTexData[m_image.width * ly + lx] = color2;
		//	}
		//}
	}
	else
	{
		int tmp_i;
		for (s32 ly = y; ly < y + height; ++ly)
		{
			color1 = pline[ly - y];
			tmp_i = m_image.width * ly;
			for (s32 lx = x; lx < x + width; ++lx)
				pTexData[tmp_i + lx] = color1;
			//	跳过透明像素
			/*if (!GET_ARGB_A(color1))
				continue;
			for (s32 lx = x; lx < x + width; ++lx)
			{
				color2 = pTexData[m_image.width * ly + lx];
				BLEND_ARGB_8888(color1, color2);
				pTexData[m_image.width * ly + lx] = color2;
			}*/
		}
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	free(pline);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::clear_rect(int argc, VALUE * argv, VALUE obj)
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
		CRbRect * rect = GetObjectPtr<CRbRect>(argv[0]);
		x = rect->m_rect_data[0];
		y = rect->m_rect_data[1];
		width = rect->m_rect_data[2];
		height = rect->m_rect_data[3];
	}

	//	修正矩形区域
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_image.width - x < width)	{ width = m_image.width - x; }
	if (m_image.height - y < height)	{ height = m_image.height - y; }

	if (width <= 0 || height <= 0)
		return Qfalse;

	u32* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qfalse;

	for (int ly = y; ly < y + height; ++ly)
		memset(&pTexData[m_image.width * ly + x], 0, sizeof(u32) * width);

	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE CRbBitmap::blur()
{
	check_raise();
	
	HGE * hge = GetAppPtr()->GetHgePtr();
	int mathW, mathH;

	// 让高斯模糊见鬼去吧，我赌五毛 RGSS 的 blur 就是这么实现的 = =b
	u32 * pTexData = hge->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qnil;
	double rate = 1.8;
	int tmpWidth = (int)(m_image.width / rate), tmpHegiht = (int)(m_image.height / rate);
	u32 * pTmpData = (u32 *)malloc(tmpWidth * tmpHegiht * sizeof(u32));

	mathW = tmpWidth + ceil(sqrt(sqrt((double)tmpWidth / (double)m_image.width)));
	mathH = tmpHegiht + ceil(sqrt(sqrt((double)tmpHegiht / (double)m_image.height)));
	BilinearZoom(pTexData, pTmpData, m_image.width, m_image.height, tmpWidth, tmpHegiht, mathW, mathH);

	mathW = m_image.width + ceil(sqrt(sqrt((double)m_image.width / (double)tmpWidth)));
	mathH = m_image.height + ceil(sqrt(sqrt((double)m_image.height / (double)tmpHegiht)));
	BilinearZoom(pTmpData, pTexData, tmpWidth, tmpHegiht, m_image.width, m_image.height, mathW, mathH);

	free(pTmpData);
	hge->Texture_Unlock(m_image.quad.tex);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE CRbBitmap::flip_h()
{
	check_raise();
	
	s32 width = m_image.width;
	s32 height = m_image.height;
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qfalse;
	u32 * pTempData = (u32 *)malloc(width * height * sizeof(u32));
	memcpy(pTempData, pTexData, width * height * sizeof(u32));
	int tmp_i;
	for (s32 ly = 0; ly < height; ++ly)
	{
		tmp_i = m_image.width * ly;
		for (s32 lx = 0; lx < width; ++lx)
			pTexData[tmp_i + lx] = pTempData[tmp_i + (width - lx - 1)];
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE CRbBitmap::flip_v()
{
	check_raise();

	s32 width = m_image.width;
	s32 height = m_image.height;
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	if (!pTexData) return Qfalse;
	u32 * pTempData = (u32 *)malloc(width * height * sizeof(u32));
	memcpy(pTempData, pTexData, width * height * sizeof(u32));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pTexData + (m_image.width * ly), pTempData + (m_image.width * (height - ly - 1)), sizeof(u32) * width);
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	free(pTempData);
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::radial_blur(VALUE angle, VALUE division)
{
	check_raise();
	
#pragma message("		Unfinished Function " __FUNCTION__)
	/*SafeFixnumValue(angle);
	SafeFixnumValue(division);
	
	int agl = FIX2INT(angle);
	agl = SinBound(agl, 0, 360);
	int div = FIX2INT(division);
	div = SinBound(div, 2, 100);
	
	float aglRate = (float)agl / div;
	float alRate = 255.0f / div;
	
	HGE* hge = GetAppPtr()->GetHgePtr();
	
	float alpha = 0, rot = 0;*/

	
	/*s32 width = m_image.width;
	s32 height = m_image.height;
	u32* pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, false);
	u32* pTempData = (u32*)malloc(width * height * sizeof(u32));

	
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);
	free(pTempData);*/
	//	增加 修改计数值
	++m_modify_count;
	return Qnil;
}

VALUE CRbBitmap::get_rect()
{
	check_raise();
	return ReturnObject(m_rect_ptr);
}

VALUE CRbBitmap::get_width()
{
	check_raise();
	return INT2FIX(m_image.width);
}

VALUE CRbBitmap::get_height()
{
	check_raise();
	return INT2FIX(m_image.height);
}

VALUE CRbBitmap::get_filename()
{
	check_raise();
	return m_filename;
}

//VALUE CRbBitmap::get_font()
//{
//	check_raise();
//	return ReturnObject(m_font_ptr);
//}
//
//VALUE CRbBitmap::set_font(VALUE font)
//{
//	check_raise();
//	SafeFontValue(font);
//	m_font_ptr = GetObjectPtr<CRbFont>(font);
//	return font;
//}

VALUE CRbBitmap::save_to_file(int argc, VALUE * argv, VALUE obj)
{
	check_raise();
	VALUE filepath, fileformat;
	if (rb_scan_args(argc, argv, "11", &filepath, &fileformat) == 1)
		fileformat = INT2FIX(3);
	else
		SafeFixnumValue(fileformat);

	SafeStringValue(filepath);
	if (GetAppPtr()->GetHgePtr()->Texture_Save2File(m_image.quad.tex,
		m_image.width, m_image.height, Kconv::UTF8ToUnicode(RSTRING_PTR(filepath))))
		return Qtrue;
	
	return Qfalse;
}

VALUE CRbBitmap::to_str()
{
	check_raise();

	static const char * characters = "MNHQ$OC?7>!;-:. ";//"M80V1;:*-. ";
	int count = strlen(characters);
	int span = 0xFF / count;
	long strLen = m_image.width * m_image.height + m_image.height;
	char * charmap = (char *)malloc(strLen * sizeof(char));
	
	BYTE a, r, g, b;
	long lx, ly, lineno, offset, cidx;
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(m_image.quad.tex, true);
	for (ly = 0; ly < m_image.height; ++ly)
	{
		lineno = m_image.width * ly;
		for (lx = 0; lx < m_image.width; ++lx)
		{
			offset = lineno + lx;
			GET_ARGB_8888(pTexData[offset], a, r, g, b);
			cidx = (r + g + b) / 3 / span;
			cidx = SinBound(cidx, 0, count - 1);
			charmap[offset + ly] = characters[cidx];
		}
		charmap[offset + ly + 1] = '\n';
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(m_image.quad.tex);

	VALUE vstr = rb_str_new(charmap, strLen);
	free(charmap);
	return vstr;
}

imp_method(CRbBitmap, dispose)
imp_method(CRbBitmap, is_disposed)
imp_method_vargs(CRbBitmap, save_to_file)
imp_method01(CRbBitmap, hue_change)
imp_method01(CRbBitmap, brightness_change)
imp_method_vargs(CRbBitmap, tone_change)

imp_method_vargs(CRbBitmap, blt)
imp_method_vargs(CRbBitmap, stretch_blt)
imp_method_vargs(CRbBitmap, fill_rect)
imp_method(CRbBitmap, clear)
imp_method02(CRbBitmap, get_pixel)
imp_method03(CRbBitmap, set_pixel)
imp_method_vargs(CRbBitmap, draw_text)
imp_method02(CRbBitmap, text_size)

imp_method_vargs(CRbBitmap, gradient_fill_rect)
imp_method_vargs(CRbBitmap, clear_rect)
imp_method(CRbBitmap, blur)
imp_method02(CRbBitmap, radial_blur)

imp_method(CRbBitmap, flip_h)
imp_method(CRbBitmap, flip_v)
imp_method(CRbBitmap, to_str)

imp_attr_reader(CRbBitmap, rect)
imp_attr_reader(CRbBitmap, width)
imp_attr_reader(CRbBitmap, height)
imp_attr_reader(CRbBitmap, filename)
//imp_attr_accessor(CRbBitmap, font)