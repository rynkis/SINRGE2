#include "RbBitmap.h"
#include "RbColor.h"
#include "RbRect.h"
#include "RbFont.h"
#include "RbTone.h"

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
	m_tex.Dispose();
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
	rb_cBitmap = rb_define_class_under(rb_mRGE2Core, "Bitmap", rb_cObject);
	
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

/**
 *	@call
 *		Bitmap.new(filename[, colorkey])	-> bitmap 对象。
 *		Bitmap.new(width, height)			-> bitmap 对象。
 *
 *	@desc
 *		读取 filename 指定的图像文件生成 Bitmap 对象，可以指定透明颜色。或直接生成指定尺寸的位图对象。
 *
 *	@excp
 *		如果指定的文件加载失败会抛出RGEError异常。
 */
VALUE RbBitmap::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE arg01, arg02;

	DWORD dwColorValue = 0;

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
				CRgeColor* ccol;
				ccol = GetObjectPtr<CRgeColor>(arg02);
				dwColorValue = ccol->GetColor().value;
			}
			goto __bitmap_load;
		}
		else goto __bitmap_create;
	}

__bitmap_load:
	{
		void *data;
		DWORD size;

		int suffix_idx;

		static char* szSuffix[] = {".png", ".jpg", ".bmp", ".tga", ".dds", ".dib"};
		static u32	 uSuffixCnt = RgeArrayCount(szSuffix);

		if (data = GetResManager()->Resource_Load_Without_Suffix(RSTRING_PTR(arg01), &size, szSuffix, uSuffixCnt, &suffix_idx))
		{
			if (CreateTextureInMemory(data, size, dwColorValue, &m_tex))
			{
				VALUE tmp_filename = rb_str_dup(arg01);
				if (suffix_idx != -1) tmp_filename = rb_str_plus(tmp_filename, rb_str_new2(szSuffix[suffix_idx]));
				m_filename = rb_str_freeze(tmp_filename);
			}

			// free
			GetResManager()->Resource_Free(data);
		}

		if (!m_tex.sub_textures)
			rb_raise(rb_eRGEBaseError, RgeUTF8("读取图片 `%s' 失败"), RSTRING_PTR(arg01));
	}
	goto __finish;

__bitmap_create:
	{
		SafeFixnumValue(arg01);
		SafeFixnumValue(arg02);

		u32 w = FIX2INT(arg01);
		u32 h = FIX2INT(arg02);

		if (!m_tex.Create(w, h))
		{
			rb_raise(rb_eRGEBaseError, RgeUTF8("创建位图 `%d x %d' 失败!"), w, h);
		}
	}

__finish:
	{
		//	create rect
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(m_tex.width), INT2FIX(m_tex.height)};

		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<CRgeRect>(rect);

		// create a font object for the bitmap-obj
		VALUE font = rb_class_new_instance(0, 0, rb_cFont);
		m_font_ptr = GetObjectPtr<CRgeFont>(font);

		m_disposed = false;
	}
	return obj;
}

void RbBitmap::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eRGEDisposedObjectError, "disposed bitmap");
}

bool RbBitmap::AdjustTexturesToneSingle(const SRgeTexture* pTex, DWORD dwTone)
{
	DWORD *pSrcTexData = 0;

	if (!pTex || !pTex->sub_textures)
		return false;

	if (!dwTone)
		return true;

	HGE* hge = GetRgeSysPtr()->GetHgePtr();

	URgeARGB	tone(dwTone);
	URgeARGB*	color;

	int gray;
	int index;

	for (u32 i = 0; i < pTex->rows; ++i) 
	{
		for (u32 j = 0; j < pTex->cols; ++j) 
		{
			index = i * pTex->cols + j;

			pSrcTexData = hge->Texture_Lock(pTex->sub_textures[index].tex, false);

			if (!pSrcTexData)
				goto failed_return;

			u32 _x = pTex->sub_textures[0].width * j;
			u32 _y = pTex->sub_textures[0].height * i;

			int w = RgeMin(pTex->width - _x,	pTex->sub_textures[index].width);
			int h = RgeMin(pTex->height - _y,	pTex->sub_textures[index].height);

			int mem_w = pTex->sub_textures[0].width;

			for (int x = 0; x < w; ++x)
			{
				for (int y = 0; y < h; ++y)
				{
					color = (URgeARGB*)&pSrcTexData[mem_w * y + x];

					if (tone.a == 0)
					{
						color->r = sTable768_low[color->r + tone.r];
						color->g = sTable768_low[color->g + tone.g];
						color->b = sTable768_low[color->b + tone.b];
					}
					else
					{
						gray = (color->r * 38 + color->g * 75 + color->b * 15) >> 7;

						color->r = sTable768_low[tone.r + color->r + (gray - color->r) * tone.a / 256];
						color->g = sTable768_low[tone.g + color->g + (gray - color->g) * tone.a / 256];
						color->b = sTable768_low[tone.b + color->b + (gray - color->b) * tone.a / 256];
					}
				}
			}

			hge->Texture_Unlock(pTex->sub_textures[index].tex);
		}
	}

	return true;

failed_return:
	if (pSrcTexData) hge->Texture_Unlock(pTex->sub_textures[index].tex);

	return false;
}

bool RbBitmap::AdjustTexturesToneDouble(const SRgeTexture* pSrcTex, const SRgeTexture* pDstTex, DWORD dwTone)
{
	DWORD* pSrcTexData = 0;
	DWORD* pDstTexData = 0;

	if (!pSrcTex || !pSrcTex->sub_textures)
		return false;

	if (!pDstTex || !pDstTex->sub_textures)
		return false;

	if (pSrcTex->width != pDstTex->width ||
		pSrcTex->height != pDstTex->height ||
		pSrcTex->rows != pDstTex->rows ||
		pSrcTex->cols != pDstTex->cols)
		return false;

	if (!dwTone)
		return true;

	HGE* hge = GetRgeSysPtr()->GetHgePtr();

	URgeARGB tone(dwTone);
	URgeARGB *src_color, *dst_color;

	int gray;
	int index;

	for (u32 i = 0; i < pSrcTex->rows; ++i) 
	{
		for (u32 j = 0; j < pSrcTex->cols; ++j) 
		{
			index = i * pSrcTex->cols + j;

			pSrcTexData = hge->Texture_Lock(pSrcTex->sub_textures[index].tex, pDstTex != 0);

			if (!pSrcTexData)
				goto failed_return;

			pDstTexData = hge->Texture_Lock(pDstTex->sub_textures[index].tex, false);

			if (!pDstTexData)
				goto failed_return;

			u32 _x = pSrcTex->sub_textures[0].width * j;
			u32 _y = pSrcTex->sub_textures[0].height * i;

			int w = RgeMin(pSrcTex->width - _x,		pSrcTex->sub_textures[index].width);
			int h = RgeMin(pSrcTex->height - _y,	pSrcTex->sub_textures[index].height);

			int src_mem_w = pSrcTex->sub_textures[0].width;
			int dst_mem_w = pDstTex->sub_textures[0].width;

			for (int x = 0; x < w; ++x)
			{
				for (int y = 0; y < h; ++y)
				{
					src_color = (URgeARGB*)&pSrcTexData[src_mem_w * y + x];
					dst_color = (URgeARGB*)&pDstTexData[dst_mem_w * y + x];

					if (tone.a == 0)
					{
						dst_color->r = sTable768_low[src_color->r + tone.r];
						dst_color->g = sTable768_low[src_color->g + tone.g];
						dst_color->b = sTable768_low[src_color->b + tone.b];
					}
					else
					{
						gray = (src_color->r * 38 + src_color->g * 75 + src_color->b * 15) >> 7;

						dst_color->r = sTable768_low[tone.r + src_color->r + (gray - src_color->r) * tone.a / 256];
						dst_color->g = sTable768_low[tone.g + src_color->g + (gray - src_color->g) * tone.a / 256];
						dst_color->b = sTable768_low[tone.b + src_color->b + (gray - src_color->b) * tone.a / 256];
					}

					dst_color->a = src_color->a;
					//color.value = pSrcTexData[mem_w * y + x];

					//if (tone.a == 0)
					//{
					//	color.r = sTable768_low[color.r + tone.r];
					//	color.g = sTable768_low[color.g + tone.g];
					//	color.b = sTable768_low[color.b + tone.b];
					//}
					//else
					//{
					//	gray = (color.r * 38 + color.g * 75 + color.b * 15) >> 7;

					//	color.r = sTable768_low[tone.r + color.r + (gray - color.r) * tone.a / 256];
					//	color.g = sTable768_low[tone.g + color.g + (gray - color.g) * tone.a / 256];
					//	color.b = sTable768_low[tone.b + color.b + (gray - color.b) * tone.a / 256];
					//}
					////if (tone.a == 0)
					////{
					////	color.r = RgeBound(color.r + tone.r, 0, 255);
					////	color.g = RgeBound(color.g + tone.g, 0, 255);
					////	color.b = RgeBound(color.b + tone.b, 0, 255);
					////}
					////else
					////{
					////	gray = (color.r * 38 + color.g * 75 + color.b * 15) >> 7;

					////	color.r = RgeBound(tone.r + color.r + (gray - color.r) * tone.a / 256, 0, 255);
					////	color.g = RgeBound(tone.g + color.g + (gray - color.g) * tone.a / 256, 0, 255);
					////	color.b = RgeBound(tone.b + color.b + (gray - color.b) * tone.a / 256, 0, 255);
					////}

					//pDstTexData[mem_w * y + x] = color.value;
				}
			}

			hge->Texture_Unlock(pSrcTex->sub_textures[index].tex);
			hge->Texture_Unlock(pDstTex->sub_textures[index].tex);
		}
	}

	return true;

failed_return:
	if (pSrcTexData) hge->Texture_Unlock(pSrcTex->sub_textures[index].tex);
	if (pDstTexData) hge->Texture_Unlock(pDstTex->sub_textures[index].tex);

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
//void RbBitmap::ColorSpaceHSV2RGB(float H, float S, float V, int &R, int &G, int &B)
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

bool RbBitmap::CreateTextureInMemory(void* pData, u32 uDataLength, DWORD dwColorKey, SRgeTexture* pTex)
{
	u32	maxTexW, maxTexH;

	HTEXTURE tmpTex = NULL;
	LPDIRECT3DSURFACE8 imgSurface = NULL;

	D3DXIMAGE_INFO	ii;

	__try
	{
		if (FAILED(D3DXGetImageInfoFromFileInMemory(pData, uDataLength, &ii)))
			goto failed_return;

		maxTexW = GetRgeSysPtr()->GetMaxTexW();
		maxTexH = GetRgeSysPtr()->GetMaxTexH();

		pTex->width		= ii.Width;
		pTex->height	= ii.Height;

		//	检查图片宽度是否超过显卡最大允许值，然后根据情况分别处理。
		if (pTex->width > maxTexW || pTex->height > maxTexH)
		{
			//	创建Surface来存储图像数据
			if (FAILED(GetRgeSysPtr()->GetD3DDevicePtr()->CreateImageSurface(pTex->width, pTex->height, D3DFMT_A8R8G8B8, &imgSurface)))
				goto failed_return;

			//	从内存加载图像数据到Surface
			if (FAILED(D3DXLoadSurfaceFromFileInMemory(imgSurface,		// LPDIRECT3DSURFACE8 pDestSurface,
													NULL,				// CONST PALETTEENTRY* pDestPalette,
													NULL,				// CONST RECT* pDestRect,
													pData,				// LPCVOID pSrcData,
													uDataLength,		// UINT SrcData,
													NULL,				// CONST RECT* pSrcRect,
													D3DX_FILTER_NONE,	// DWORD Filter,
													dwColorKey,			// D3DCOLOR ColorKey,
													&ii)))

				goto failed_return;

			//	锁定Surface
			D3DLOCKED_RECT lockRect;
			if (FAILED(imgSurface->LockRect(&lockRect, NULL, D3DLOCK_READONLY)))
				goto failed_return;

			//	创建子Texture数组
			pTex->rows = pTex->cols = 1;

			if (maxTexW < pTex->width)
				pTex->cols = int((pTex->width + maxTexW - 1) / maxTexW);
			
			if (maxTexH < pTex->height)
				pTex->rows = int((pTex->height + maxTexH - 1) / maxTexH);

			pTex->sub_textures = new SRgeTextureAttr[pTex->rows * pTex->cols];
			memset(pTex->sub_textures, 0, sizeof(SRgeTextureAttr) * (pTex->rows * pTex->cols));

			//	循环创建子Texture对象
			u32 pixelsX = 0, pixelsY = 0;
			
			for (u32 i = 0; i < pTex->rows; ++i) 
			{
				for (u32 j = 0; j < pTex->cols; ++j) 
				{
					
					// Create the sub-texture at position (i, j) in the matrix.
					// Try to create it so it will be able to store whatever's left in
					// the image:
					tmpTex = GetRgeSysPtr()->GetHgePtr()->Texture_Create(maxTexW, maxTexH);
					if (!tmpTex)
						goto failed_return;
					
					// Lock the texture so I can fill it with pixel data:
					DWORD * tmpTexData = GetRgeSysPtr()->GetHgePtr()->Texture_Lock(tmpTex, false);
					
					for (u32 x = 0; x < maxTexW; ++x) 
					{
						for (u32 y = 0; y < maxTexH; ++y) 
						{
							// Look up the pixel's components:
							// Out of image bounds?
							if (pixelsY + y >= pTex->height || pixelsX + x >= pTex->width)
								tmpTexData[y * maxTexW + x] = 0; // Use a transparent black -- doesn't matter
							else
							{
								// Row offset:
								DWORD * row_data = (DWORD*)((unsigned char *)lockRect.pBits + lockRect.Pitch * (pixelsY + y));
								
								tmpTexData[y * maxTexW + x] = row_data[pixelsX + x];
							}
						}
					}
					
					// This sub-texture is done.
					GetRgeSysPtr()->GetHgePtr()->Texture_Unlock(tmpTex);
					
					// Store it:
					pTex->sub_textures[i * pTex->cols + j].tex	= tmpTex;
					pTex->sub_textures[i * pTex->cols + j].width  = maxTexW;
					pTex->sub_textures[i * pTex->cols + j].height = maxTexH;
					
					// Done with this texture:
					tmpTex = NULL;
					
					// Advance the amount of pixels read.
					pixelsX += maxTexW;
					
					// This row finished?
					if (pixelsX >= pTex->width) 
					{
						// Go to the beginning of the next row:
						pixelsX = 0;
						pixelsY += maxTexH;
					}
				}
			}
			
			// Done!
			
			// Unlock the surface now:
			if (FAILED(imgSurface->UnlockRect()))
				goto failed_return;
		}
		else
		{
			pTex->rows = pTex->cols = 1;
			pTex->sub_textures = new SRgeTextureAttr[1];
			pTex->sub_textures[0].tex = GetRgeSysPtr()->GetHgePtr()->Texture_Load((const wchar_t*)pData, uDataLength, false, dwColorKey);
			pTex->sub_textures[0].width = GetRgeSysPtr()->GetHgePtr()->Texture_GetWidth(pTex->sub_textures[0].tex);
			pTex->sub_textures[0].height = GetRgeSysPtr()->GetHgePtr()->Texture_GetHeight(pTex->sub_textures[0].tex);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		goto failed_return;
	}	
	if (imgSurface) 
	{
		imgSurface->Release();
		imgSurface = NULL;
	}
	return true;

failed_return:

	//if (pRefTextures)
	//{
	//	u32 uNum = uRefRows * uRefCols;

	//	for (u32 uIdx = 0; uIdx < uNum; ++uIdx)
	//	{
	//		if (pRefTextures[uIdx].tex)
	//		{
	//			GetRgeSysPtr()->GetHgePtr()->Texture_Free(pRefTextures[uIdx].tex);
	//		}
	//	}

	//	delete [] pRefTextures;
	//}

	if (tmpTex) 
	{
		GetRgeSysPtr()->GetHgePtr()->Texture_Free(tmpTex);
		tmpTex = NULL;
	}
	
	if (imgSurface) 
	{
		imgSurface->Release();
		imgSurface = NULL;
	}

//	*tex = 0;
	return false;
}

VALUE RbBitmap::dispose()
{
	if (m_disposed)
		return Qnil;

	m_tex.Dispose();

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

	//int r, g, b, a;
	float h, s, l;

	//DWORD	color;
	URgeARGB color(0);

	int index;

	HGE* hge = GetRgeSysPtr()->GetHgePtr();

	for (u32 i = 0; i < m_tex.rows; ++i) 
	{
		for (u32 j = 0; j < m_tex.cols; ++j) 
		{
			index = i * m_tex.cols + j;

			DWORD* pTexData = hge->Texture_Lock(m_tex.sub_textures[index].tex, false);


			u32 _x = m_tex.sub_textures[0].width * j;
			u32 _y = m_tex.sub_textures[0].height * i;

			int wi = RgeMin(m_tex.width - _x,	m_tex.sub_textures[index].width);
			int hi = RgeMin(m_tex.height - _y,	m_tex.sub_textures[index].height);

			int mem_w = m_tex.sub_textures[0].width;

			for (int x = 0; x < wi; ++x)
			{
				for (int y = 0; y < hi; ++y)
				{
					color.value = pTexData[mem_w * y + x];

					ColorSpaceRGB2HSV(color.r, color.g, color.b, h, s, l);
					h += iHue;
					ColorSpaceHSV2RGB(h, s, l, color.r, color.g, color.b);

					pTexData[mem_w * y + x] = color.value;
				}
			}

			hge->Texture_Unlock(m_tex.sub_textures[index].tex);
		}
	}

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

	iBrightness = RgeBound(iBrightness, -255, 255);

	//URgeARGB color(0);
	URgeARGB* color;

	int index;

	HGE* hge = GetRgeSysPtr()->GetHgePtr();

	for (u32 i = 0; i < m_tex.rows; ++i) 
	{
		for (u32 j = 0; j < m_tex.cols; ++j) 
		{
			index = i * m_tex.cols + j;

			DWORD* pTexData = hge->Texture_Lock(m_tex.sub_textures[index].tex, false);


			u32 _x = m_tex.sub_textures[0].width * j;
			u32 _y = m_tex.sub_textures[0].height * i;

			int wi = RgeMin(m_tex.width - _x,	m_tex.sub_textures[index].width);
			int hi = RgeMin(m_tex.height - _y,	m_tex.sub_textures[index].height);

			int mem_w = m_tex.sub_textures[0].width;

			for (int x = 0; x < wi; ++x)
			{
				for (int y = 0; y < hi; ++y)
				{
					color = (URgeARGB*)&pTexData[mem_w * y + x];

					color->r = sTable768_mid[color->r + iBrightness + 256];
					color->g = sTable768_mid[color->g + iBrightness + 256];
					color->b = sTable768_mid[color->b + iBrightness + 256];

					//color.value = pTexData[mem_w * y + x];

					//color.r = sTable768_mid[color.r + iBrightness + 256];
					//color.g = sTable768_mid[color.g + iBrightness + 256];
					//color.b = sTable768_mid[color.b + iBrightness + 256];
					////color.r = RgeBound(color.r + iBrightness, 0, 255);
					////color.g = RgeBound(color.g + iBrightness, 0, 255);
					////color.b = RgeBound(color.b + iBrightness, 0, 255);

					//pTexData[mem_w * y + x] = color.value;
				}
			}

			hge->Texture_Unlock(m_tex.sub_textures[index].tex);
		}
	}

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

/**
 *	@call
 *		tone_change([red[, green[, blue[, gray]]]])					-> nil
 *		tone_change(tone)											-> nil
 *
 *	@desc
 *		更改位图的色调。red|green|blue|gray 省略后的默认值都为255。
 *
 *	@excp
 *		如果对已经释放的对象进行调用会抛出DisposedObjectError异常。
 */
VALUE RbBitmap::tone_change(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	int a, r, g, b;
	VALUE argv01, green, blue, gray;
	URgeARGB tone(0);

	if (rb_scan_args(argc, argv, "04", &argv01, &green, &blue, &gray) == 1)
	{
		if (FIXNUM_P(argv01))
		{
			r = FIX2INT(argv01);
			r = RgeBound(r, 0, 255);
			tone.a = tone.g = tone.b = 255;
			tone.r = r;
		}
		else
		{
			SafeToneValue(argv01);
			tone.value = GetObjectPtr<CRgeTone>(argv01)->GetColor().value;
		}
	}
	else
	{
		for (int i = 0; i < argc; ++i)
		{
			SafeFixnumValue(argv[i]);
		}

		r = (NIL_P(argv01) ? 255 : FIX2INT(argv01));
		g = (NIL_P(green) ? 255 : FIX2INT(green));
		b = (NIL_P(blue) ? 255 : FIX2INT(blue));
		a = (NIL_P(gray) ? 255 : FIX2INT(gray));

		r = RgeBound(r, 0, 255);
		g = RgeBound(g, 0, 255);
		b = RgeBound(b, 0, 255);
		a = RgeBound(a, 0, 255);

		tone.a = a;
		tone.r = r;
		tone.b = b;
		tone.g = g;
	}

	AdjustTexturesToneSingle(&m_tex, tone.value);

	//	增加 修改计数值
	++m_modify_count;

	return Qnil;
}

VALUE RbBitmap::blt(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::stretch_blt(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::fill_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::clear()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::get_pixel(VALUE x, VALUE y)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::set_pixel(VALUE x, VALUE y, VALUE color)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::draw_text(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::text_size(VALUE str)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::gradient_fill_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::clear_rect(int argc, VALUE *argv, VALUE obj)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::blur()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE RbBitmap::radial_blur(VALUE angle, VALUE division)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

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

	return INT2FIX(GetBmpWidth());
}

VALUE RbBitmap::get_height()
{
	check_raise();

	return INT2FIX(GetBmpHeight());
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
	m_font_ptr = GetObjectPtr<CRgeFont>(font);
	return Qnil;
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

	if (FAILED(GetRgeSysPtr()->GetD3DDevicePtr()->CreateImageSurface(m_tex.width, m_tex.height, D3DFMT_A8R8G8B8, &pDst)))
		return Qfalse;

	POINT	pt = {0, };
	RECT	rt = {0, };

	for (u32 i = 0; i < m_tex.rows; ++i) 
	{
		for (u32 j = 0; j < m_tex.cols; ++j) 
		{
			int index = i * m_tex.cols + j;

			pt.x = m_tex.sub_textures[0].width * j;
			pt.y = m_tex.sub_textures[0].height * i;

			rt.right	= RgeMin(m_tex.width - pt.x,	m_tex.sub_textures[index].width);
			rt.bottom	= RgeMin(m_tex.height - pt.y,	m_tex.sub_textures[index].height);

			if (FAILED(((LPDIRECT3DTEXTURE8)m_tex.sub_textures[index].tex)->GetSurfaceLevel(0, &pSrc)))
				goto failed_return;

			if (FAILED(GetRgeSysPtr()->GetD3DDevicePtr()->CopyRects(pSrc, &rt, 1, pDst, &pt)))
				goto failed_return;

			pSrc->Release();
			pSrc = NULL;
		}
	}

	//	This function supports the following file formats: .bmp and .dds.
	if (SUCCEEDED(D3DXSaveSurfaceToFileW(RgeUTF8ToUnicode(RSTRING_PTR(filepath)), 
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

imp_attr_reader(RbBitmap, rect)
imp_attr_reader(RbBitmap, width)
imp_attr_reader(RbBitmap, height)
imp_attr_reader(RbBitmap, filename)
imp_attr_accessor(RbBitmap, font)