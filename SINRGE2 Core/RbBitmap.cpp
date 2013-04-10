#include "RbExport.h"
#include "RbBitmap.h"
#include "RbColor.h"
#include "RbRect.h"
#include "RbFont.h"
#include "RbTone.h"
#include "SINRGE2.h"

#include <math.h>
#include <d3dx8tex.h>

using namespace Sin;

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
	if(m_bmp.quad.tex)
		GetHgePtr()->Texture_Free(m_bmp.quad.tex);
}

void RbBitmap::InitLibrary()
{
	/**
	 *	@classname
	 *		Bitmap
	 *
	 *	@desc
	 *		λͼ���ࡣ��νλͼ����ʾͼ���䱾����
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
	
	rb_define_method(rb_cBitmap, "render",				(RbFunc)dm_render ,				0);
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

/**
 *	@call
 *		Bitmap.new(filename[, colorkey])	-> bitmap ����
 *		Bitmap.new(width, height)			-> bitmap ����
 *
 *	@desc
 *		��ȡ filename ָ����ͼ���ļ����� Bitmap ���󣬿���ָ��͸����ɫ����ֱ������ָ���ߴ��λͼ����
 *
 *	@excp
 *		���ָ�����ļ�����ʧ�ܻ��׳�RGEError�쳣��
 */
VALUE RbBitmap::initialize(int argc, VALUE *argv, VALUE obj)
{
	VALUE arg01, arg02;

	DWORD dwColorValue = 0;
	HGE* hge = GetHgePtr();
	hgeQuad quad;
	wchar_t* filenameW;

	if (rb_scan_args(argc, argv, "11", &arg01, &arg02) == 1)
	{
		SafeStringValue(arg01);
		goto __try_bitmap_load;
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
			goto __try_bitmap_load;
		}
		else goto __bitmap_create;
	}

__try_bitmap_load:
	{
		filenameW = Kconv::UTF8ToUnicode(RSTRING_PTR(arg01));
		quad.tex = hge->Texture_Load(filenameW);
		if (!quad.tex)
			goto __bitmap_load;

	}
	goto __finish;

__bitmap_load:
	{
		void *data;
		DWORD size;

		int suffix_idx;
		
		static wchar_t* szSuffix[]	= {L".png", L".jpg", L".bmp", L".tga", L".dds", L".dib"};
		static char*	szSuffixA[] = { ".png",  ".jpg",  ".bmp",  ".tga",  ".dds",  ".dib"};
		static int		uSuffixCnt	= SinArrayCount(szSuffix);

		if (data = hge->Resource_Load_Without_Suffix(filenameW, &size, szSuffix, uSuffixCnt, &suffix_idx))
		{
			quad.tex = hge->Texture_Load((const wchar_t*)data, size, false, dwColorValue);

			if(!quad.tex)
				rb_raise(rb_eSINBaseError, "Failed to load bitmap `%s'.", RSTRING_PTR(arg01));
			
			VALUE tmp_filename = rb_str_dup(arg01);
			if (suffix_idx != -1) tmp_filename = rb_str_plus(tmp_filename, rb_str_new2(szSuffixA[suffix_idx]));
			m_filename = rb_str_freeze(tmp_filename);
			// free
			hge->Resource_Free(data);
		}
		else
			rb_raise(rb_eSINBaseError, "Failed to load bitmap `%s'.", RSTRING_PTR(arg01));

	}
	goto __finish;

__bitmap_create:
	{
		SafeFixnumValue(arg01);
		SafeFixnumValue(arg02);

		u32 w = FIX2INT(arg01);
		u32 h = FIX2INT(arg02);

		quad.tex = hge->Texture_Create(w, h);
		if(!quad.tex)
			rb_raise(rb_eSINBaseError, "Failed to create bitmap: `%d x %d'.", w, h);
	}

__finish:
	{
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
		
		m_bmp.quad		= quad;
		m_bmp.width		= hge->Texture_GetWidth(quad.tex, true);
		m_bmp.height	= hge->Texture_GetHeight(quad.tex, true);
		m_bmp.texw		= hge->Texture_GetWidth(quad.tex);
		m_bmp.texh		= hge->Texture_GetHeight(quad.tex);
		m_bmp.rcentrex = m_bmp.width * 1.0f / 2;
		m_bmp.rcentrey = m_bmp.height * 1.0f / 2;

		//	create rect
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(m_bmp.texw), INT2FIX(m_bmp.texh)};
		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<RbRect>(rect);
		// create a font object for the bitmap-obj
		VALUE font = rb_class_new_instance(0, 0, rb_cFont);
		m_font_ptr = GetObjectPtr<RbFont>(font);
		m_disposed = false;
	}
	return obj;
}

void RbBitmap::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSINDisposedObjectError, "disposed bitmap");
}

bool RbBitmap::AdjustTexturesTone(const bitmap_p pBmp, DWORD dwTone)
{
	if (!dwTone)
		return true;
	
	BYTE a1, r1, g1, b1, a2, r2, g2, b2;
	GET_ARGB_8888(dwTone, a1, r1, g1, b1);

	int gray;

	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(pBmp->quad.tex, false);
	if (!pSrcTexData)
		goto failed_return;

	for (s32 x = 0; x < pBmp->width; ++x)
	{
		for (s32 y = 0; y < pBmp->height; ++y)
		{
			GET_ARGB_8888(pSrcTexData[pBmp->texw * y + x], a2, r2, g2, b2);
			if (a1 == 0)
			{
				r2 = sTable768_low[r2 + r1];
				g2 = sTable768_low[g2 + g1];
				b2 = sTable768_low[b2 + b1];
			}
			else
			{
				gray = (r2 * 38 + g2 * 75 + b2 * 15) >> 7;

				r2 = sTable768_low[r1 + r2 + (gray - r2) * a1 / 256];
				g2 = sTable768_low[g1 + g2 + (gray - g2) * a1 / 256];
				b2 = sTable768_low[b1 + b2 + (gray - b2) * a1 / 256];
			}
			pSrcTexData[pBmp->texw * y + x] = MAKE_ARGB_8888(a2, r2, g2, b2);
		}
	}
	GetHgePtr()->Texture_Unlock(pBmp->quad.tex);

	return true;

failed_return:
	if (pSrcTexData) GetHgePtr()->Texture_Unlock(pBmp->quad.tex);

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
		//pSize = &size;
		/*pOutRect->width		= size.cx;
		pOutRect->height	= size.cy;*/
	}

	SelectObject(hScreenDC, Save);

	if (hDC == NULL)
		ReleaseDC(NULL, hScreenDC);

	return (bRet != 0);
}

bitmap_p RbBitmap::CloneBitmap(bitmap_p pBmp)
{
	bitmap_p clone = (bitmap_p)malloc(sizeof(bitmap_t));
	memcpy(clone, pBmp, sizeof(bitmap_t));
	clone->quad.tex = GetHgePtr()->Texture_Create(pBmp->width, pBmp->height);
	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(pBmp->quad.tex, true);
	DWORD* pDesTexData = GetHgePtr()->Texture_Lock(clone->quad.tex, false);
	memcpy(pDesTexData, pSrcTexData, pBmp->width * pBmp->height *sizeof(DWORD));
	GetHgePtr()->Texture_Unlock(pBmp->quad.tex);
	GetHgePtr()->Texture_Unlock(clone->quad.tex);
	return clone;
}

HTEXTURE RbBitmap::CutTexture(int x, int y, int width, int height, bitmap_p pBmp)
{
	//	������������
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (pBmp->texw - x < width)		{ width = pBmp->texw - x; }
	if (pBmp->texh - y < height)	{ height = pBmp->texh - y; }

	if (width <= 0 || height <= 0)
		return 0;

	HTEXTURE cut = GetHgePtr()->Texture_Create(width, height);
	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(pBmp->quad.tex, true);//, x, y, width, height);
	DWORD* pDesTexData = GetHgePtr()->Texture_Lock(cut, false);
	//memcpy(pDesTexData, pSrcTexData, width * height *sizeof(DWORD));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pDesTexData + (width * ly), pSrcTexData + (pBmp->texw * (y + ly) + x), sizeof(DWORD) * width);
	GetHgePtr()->Texture_Unlock(pBmp->quad.tex);
	GetHgePtr()->Texture_Unlock(cut);
	return cut;
}

void RbBitmap::BilinearZoom(DWORD *OldBitmap, DWORD *NewBitmap, int OldWidth, int OldHeight, int NewWidth, int NewHeight, int MathWidth, int MathHeight)
{
	long x, y, ty1, ty2, tx1, tx2, px1, px2, py1, py2, p11, p21, p12, p22;
	
	DWORD color11, color21, color12, color22;
	BYTE a, r, g, b;
	for (y = 0; y < NewHeight; y++)
	{
		// ��ȡy����
		ty1 = y * OldHeight / MathHeight;
		ty2 = ty1 + 1;
		// ��ȡyȨ
		py2 = y * OldHeight % MathHeight * 100 / MathHeight;
		py1 = 100 - py2;
		for (x = 0; x < NewWidth; x++)
		{
			tx1 = x * OldWidth / MathWidth;
			tx2 = tx1 + 1;
			// ��ȡxȨ
			px2 = x * OldWidth % MathWidth * 100 / MathWidth;
			px1 = 100 - px2;
			// ��ȡ�ĵ���ɫ��Ӧ��Ȩ
			p11 = px1 * py1;
			p21 = px2 * py1;
			p12 = px1 * py2;
			p22 = px2 * py2;
			// ��ȡ��ɫ
			color11 = OldBitmap[tx1 + ty1 * OldWidth];
			color21 = OldBitmap[tx2 + ty1 * OldWidth];
			color12 = OldBitmap[tx1 + ty2 * OldWidth];
			color22 = OldBitmap[tx2 + ty2 * OldWidth];
			// ������ɫ
			a = (GET_ARGB_A(color11) * p11 + GET_ARGB_A(color21) * p21 + GET_ARGB_A(color12) * p12 + GET_ARGB_A(color22) * p22) / 10000;
			r = (GET_ARGB_R(color11) * p11 + GET_ARGB_R(color21) * p21 + GET_ARGB_R(color12) * p12 + GET_ARGB_R(color22) * p22) / 10000;
			g = (GET_ARGB_G(color11) * p11 + GET_ARGB_G(color21) * p21 + GET_ARGB_G(color12) * p12 + GET_ARGB_G(color22) * p22) / 10000;
			b = (GET_ARGB_B(color11) * p11 + GET_ARGB_B(color21) * p21 + GET_ARGB_B(color12) * p12 + GET_ARGB_B(color22) * p22) / 10000;
			NewBitmap[x + y * NewWidth] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
}

VALUE RbBitmap::dispose()
{
	if (m_disposed)
		return Qnil;

	if(m_bmp.quad.tex)
		GetHgePtr()->Texture_Free(m_bmp.quad.tex);

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

	float h, s, l;
	BYTE a, r, g, b;

	//int index;

	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);

	for (s32 x = 0; x < m_bmp.width; ++x)
	{
		for (s32 y = 0; y < m_bmp.height; ++y)
		{
			GET_ARGB_8888(pTexData[m_bmp.texw * y + x], a, r, g, b);
			ColorSpaceRGB2HSV(r, g, b, h, s, l);
			h += iHue;
			ColorSpaceHSV2RGB(h, s, l, r, g, b);
			pTexData[m_bmp.texw * y + x] = MAKE_ARGB_8888(a, r, g, b);
		}
	}

	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	//	���� �޸ļ���ֵ
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

	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);

	for (s32 x = 0; x < m_bmp.width; ++x)
	{
		for (s32 y = 0; y < m_bmp.height; ++y)
		{
			GET_ARGB_8888(pTexData[m_bmp.texw * y + x], a, r, g, b);
			r = sTable768_mid[r + iBrightness + 256];
			g = sTable768_mid[g + iBrightness + 256];
			b = sTable768_mid[b + iBrightness + 256];
			pTexData[m_bmp.texw * y + x] = MAKE_ARGB_8888(a, r, g, b);
		}
	}
	
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	//	���� �޸ļ���ֵ
	++m_modify_count;

	return Qnil;
}

/**
 *	@call
 *		tone_change([red[, green[, blue[, gray]]]])					-> nil
 *		tone_change(tone)											-> nil
 *
 *	@desc
 *		����λͼ��ɫ����red|green|blue|gray ʡ�Ժ��Ĭ��ֵ��Ϊ255��
 *
 *	@excp
 *		������Ѿ��ͷŵĶ�����е��û��׳�DisposedObjectError�쳣��
 */
VALUE RbBitmap::tone_change(int argc, VALUE *argv, VALUE obj)
{
	check_raise();

	int a, r, g, b;
	VALUE argv01, green, blue, gray;
	//UColor tone(0);
	DWORD tone = 0;

	if (rb_scan_args(argc, argv, "04", &argv01, &green, &blue, &gray) == 1)
	{
		if (FIXNUM_P(argv01))
		{
			r = FIX2INT(argv01);
			r = SinBound(r, 0, 255);
			tone = MAKE_ARGB_8888(255, r, 255, 255);
			/*tone.a = tone.g = tone.b = 255;
			tone.r = r;*/
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
		{
			SafeFixnumValue(argv[i]);
		}

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

	//	���� �޸ļ���ֵ
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
	//SafeFixnumValue(opacity);
	bitmap_p des = &m_bmp;
	RbRect* srcRect = GetObjectPtr<RbRect>(src_rect);
	int sx = srcRect->x;
	int sy = srcRect->y;
	int sw = srcRect->width;
	int sh = srcRect->height;
	int dx = FIX2INT(x);
	int dy = FIX2INT(y);
	int op;// = (NIL_P(opacity) ? 255 : FIX2INT(opacity));
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
	DWORD* pSrcTexData = GetHgePtr()->Texture_Lock(src->quad.tex, true);
	if (with_self)
	{
		pTempData = (DWORD*)malloc(src->width * src->height * sizeof(DWORD));
		memcpy(pTempData, pSrcTexData, src->width * src->height * sizeof(DWORD));
		GetHgePtr()->Texture_Unlock(src->quad.tex);
	}
	else
	{
		pTempData = pSrcTexData;
	}
	DWORD* pDstTexData = GetHgePtr()->Texture_Lock(des->quad.tex,false);
	/*if (!pTempData || !pDstTexData)
		return Qfalse;*/
	DWORD color1, color2;
	BYTE a, r, g, b;
	for (s32 lx = sx; lx < sw; ++lx)
	{
		for (s32 ly = sy; ly < sh; ++ly)
		{
			color1 = pTempData[src->texw * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	����͸������
			if (!a) continue;
			a = a * op / 255;
			//	����͸������
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[des->texw * (ly - sy + dy) + lx - sx + dx];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[des->texw * (ly - sy + dy) + lx - sx + dx] = color2;
		}
	}
	GetHgePtr()->Texture_Unlock(des->quad.tex);
	if (with_self)
		free(pTempData);
	else
	{
		pTempData = NULL;
		GetHgePtr()->Texture_Unlock(src->quad.tex);
	}
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
	int op;// = (NIL_P(opacity) ? 255 : FIX2INT(opacity));
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
	if (m_bmp.texw - dx <= 0) return Qfalse;
	if (m_bmp.texh - dy <= 0) return Qfalse;
	
	HGE* hge = GetHgePtr();

	HTEXTURE tmpTex;
	/*if (sx == 0 && sy == 0 && sw == src->texw && sh == src->texh)
		tmpTex = src->quad.tex;
	else*/
	tmpTex = CutTexture(sx, sy, sw, sh, src);

	if (!tmpTex) return Qfalse;

	DWORD* pSrcTexData = hge->Texture_Lock(tmpTex, true);
	if (!pSrcTexData) return Qfalse;
	DWORD* pTempData = (DWORD*)malloc(dw * dh * sizeof(DWORD));
	int mathW = dw + sqrt(sqrt((double)(dw / sw)));
	int mathH = dh + sqrt(sqrt((double)(dh / sh)));
	BilinearZoom(pSrcTexData, pTempData, sw, sh, dw, dh, mathW, mathH);

	DWORD* pDstTexData = hge->Texture_Lock(m_bmp.quad.tex, false);
	DWORD color1, color2;
	BYTE a, r, g, b;

	int ndw = dw;
	if (dw + dx > m_bmp.texw) dw = m_bmp.texw - dx;
	if (dh + dy > m_bmp.texh) dh = m_bmp.texh - dy;

	for (s32 lx = 0; lx < dw; ++lx)
	{
		for (s32 ly = 0; ly < dh; ++ly)
		{
			color1 = pTempData[ndw * ly + lx];
			GET_ARGB_8888(color1, a, r, g, b)
			//	����͸������
			if (!a) continue;
			a = a * op / 255;
			//	����͸������
			if (!a) continue;
			color1 = MAKE_ARGB_8888(a, r, g, b);
			color2 = pDstTexData[m_bmp.texw * (ly + dy) + lx + dx];
			BLEND_ARGB_8888(color1, color2);
			pDstTexData[m_bmp.texw * (ly + dy) + lx + dx] = color2;
		}
	}
	hge->Texture_Unlock(m_bmp.quad.tex);
	hge->Texture_Unlock(tmpTex);
	//if (sx != 0 || sy != 0 || sw != src->texw || sh != src->texh)
	hge->Texture_Free(tmpTex);
	free(pTempData);
	
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

	//	������������
	if (x < 0)						{ width += x; x = 0; }
	if (y < 0)						{ height += y; y = 0; }
	if (m_bmp.texw - x < width)		{ width = m_bmp.texw - x; }
	if (m_bmp.texh - y < height)	{ height = m_bmp.texh - y; }

	if (width <= 0 || height <= 0)
		return Qfalse;

	//	���
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	
	DWORD color2;
	for (s32 lx = x; lx < x + width; ++lx)
	{
		for (s32 ly = y; ly < y + height; ++ly)
		{
			//	����͸������
			if (!GET_ARGB_A(color))
				continue;

			color2 = pTexData[m_bmp.texw * ly + lx];
			BLEND_ARGB_8888(color, color2);
			pTexData[m_bmp.texw * ly + lx] = color2;
		}
	}

	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	return Qnil;
}

VALUE RbBitmap::clear()
{
	check_raise();

	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;

	/*for (int y = 0; y < m_bmp.height; ++y)
		memset(&pTexData[m_bmp.texw * y], 0, sizeof(DWORD) * m_bmp.width);*/
	ZeroMemory(pTexData, sizeof(DWORD) * m_bmp.width * m_bmp.height);

	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
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
	
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qnil;
	GET_ARGB_8888(pTexData[m_bmp.texw * dy + dx], a, r, g, b);
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

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
	//	����͸������
	if (!GET_ARGB_A(dwColor))
		return Qfalse;

	DWORD color2;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;
	color2 = pTexData[m_bmp.texw * dy + dx];
	BLEND_ARGB_8888(dwColor, color2);
	pTexData[m_bmp.texw * dy + dx] = color2;
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
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
	if (!GET_ARGB_A(draw_color))
		return Qfalse;

	DWORD color1, color2 = 0;

	HGE* hge = GetHgePtr();

	//	��ʼ��ʽ�������
	HDC	hScreenDC = GetDC(NULL);

	do
	{
		s32 cx, cy;

		if (!GetTextRect(m_font_ptr->GetHFont(), pStr, cx, cy, hScreenDC))
			break;

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

		//	��������
		DWORD *pTexData = hge->Texture_Lock(m_bmp.quad.tex, false);

		//	����ʧ�ܣ��˳�
		if (!pTexData)
			break;

		HFONT hOldFont = (HFONT)SelectObject(hScreenDC, m_font_ptr->GetHFont());

		//	��ʱ����
		int tmp_x, tmp_y;

		//	ѭ���������
		for (int i = 0; i < iCharCount; ++i)
		{
			DWORD dwBufferSize = GetGlyphOutline(hScreenDC, pStr[i], GGO_GRAY8_BITMAP, &gm, 0, NULL, &mmat2);

			if (dwBufferSize > SinArrayCount(buffer))
				rb_raise(rb_eSINBaseError, "buffer size too small.");
			//FetAssert(dwBufferSize <= FetArrayCount(buffer));

			if (dwBufferSize > 0)
			{
				int nLentemp = GetGlyphOutline(hScreenDC, pStr[i],GGO_GRAY8_BITMAP,&gm,dwBufferSize,buffer,&mmat2);

				GetTextMetrics(hScreenDC, &tm);

				int stride = dwBufferSize / gm.gmBlackBoxY;

				//	���������
				for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
				{
					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						//	���˵�Խ�������
						tmp_x = x  + gm.gmptGlyphOrigin.x + iCharOffset + ix + offset_x;
						tmp_y = y  + tm.tmAscent - gm.gmptGlyphOrigin.y + iy + offset_y;
						if (tmp_x < 0 || tmp_x >= m_bmp.texw || tmp_y < 0 || tmp_y >= m_bmp.texh)
							continue;

						//	����ȫ͸������
						BYTE tmp_gray = alphatable65[buffer[y * stride + x]];
						if (!tmp_gray)
							continue;

						tmp_gray = (BYTE)((float)tmp_gray * GET_ARGB_A(draw_color) / 255);
						if (!tmp_gray)
							continue;

						//	����������ɫ
						color1 = MAKE_ARGB_8888(tmp_gray, GET_ARGB_R(draw_color), GET_ARGB_G(draw_color), GET_ARGB_B(draw_color));
						color2 = pTexData[tmp_y * m_bmp.texw + tmp_x];
						BLEND_ARGB_8888(color1, color2);
						pTexData[tmp_y * m_bmp.texw + tmp_x] = color2;
					}
				}
			}

			iCharOffset += gm.gmCellIncX;
		}

		// ȡ������
		hge->Texture_Unlock(m_bmp.quad.tex);
		
		SelectObject(hScreenDC, hOldFont);

		++m_modify_count;

	} while (0);
	
	//	�ͷ�DC
	ReleaseDC(NULL, hScreenDC);

	return Qnil;
}

VALUE RbBitmap::text_size(VALUE str)
{
	VALUE vStr = NIL_P(str) ? rb_str_new2("nil") : rb_obj_as_string(str);

	char* pStr = RSTRING_PTR(vStr);

	/*int len = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	wchar_t* pStrW = (wchar_t*)malloc(len);
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pStrW, len);*/
	
	wchar_t* pStrW = Kconv::UTF8ToUnicode(pStr);
	
	s32 cx, cy;
	VALUE rect;

	if (GetTextRect(m_font_ptr->GetHFont(), pStrW, cx, cy, NULL))
	{
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
#pragma message("		Unfinished Function " __FUNCTION__)

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

	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	if (!pTexData)
		return Qfalse;

	for (int ly = y; ly < y + height; ++ly)
		memset(&pTexData[m_bmp.texw * ly + x], 0, sizeof(DWORD) * width);

	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);

	return Qnil;
}

VALUE RbBitmap::blur()
{
	check_raise();
	
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	int radius = 5;
	double sigma = (double)radius / 3.0;
	long width = m_bmp.texw, height = m_bmp.texh;

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
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
    
	//int nRadius = 5;
	//// �������
	//long diamet = (nRadius << 1) + 1;				// ��������ֱ��,���߷���ı߳�
	//double s = (double)nRadius / 3.0;				// ��̬�ֲ��ı�׼ƫ���
	//double sigma2 = 2.0 * s * s;					// 2���Ħ�ƽ��,�ο�Nά�ռ���̬�ֲ�����
	//double nuclear = 0.0;							// ��˹������
	//double* matrix = new double[diamet * diamet];	// ��˹������
	//DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false); // �����ڴ��
	//long w = m_bmp.texw, h = m_bmp.texh;			// ���ؾ���Ŀ����
	//// �����˹����
	//int i = 0;
	//for(long y = -nRadius; y <= nRadius; ++y)
	//	for(long x = -nRadius; x <= nRadius; ++x)
	//	{
	//		matrix[i] = exp(-(double)(x * x + y * y) / sigma2);
	//		nuclear += matrix[i];
	//		++i;
	//	}
	//// ��������������
	//for (long y_s = 0; y_s < h; ++y_s)
	//{
	//	for (long x_s = 0; x_s < w; ++x_s)
	//	{
	//		// ����ȡ������
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
	//		// ���洦�����
	//		long i_s = (h - y_s - 1) * w + x_s;
	//		pTexData[i_s] = MAKE_ARGB_8888(GET_ARGB_A(pTexData[i_s]), 
	//			(BYTE)(r > (BYTE)~0 ? (BYTE)~0 : r), 
	//			(BYTE)(g > (BYTE)~0 ? (BYTE)~0 : g), 
	//			(BYTE)(b > (BYTE)~0 ? (BYTE)~0 : b));
	//	}
	//}
	//// �����ڴ�
	//delete [] matrix;
	//// ����texture
	//GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	return Qnil;
}

VALUE RbBitmap::render()
{
	check_raise();

	float tempx1, tempy1, tempx2, tempy2;
	bitmap_p texture = &m_bmp;

	tempx1 = 0;
	tempy1 = 0;
	tempx2 = 0 + texture->texw;
	tempy2 = 0 + texture->texh;

	texture->quad.v[0].x = tempx1; texture->quad.v[0].y = tempy1;
	texture->quad.v[1].x = tempx2; texture->quad.v[1].y = tempy1;
	texture->quad.v[2].x = tempx2; texture->quad.v[2].y = tempy2;
	texture->quad.v[3].x = tempx1; texture->quad.v[3].y = tempy2;

	GetHgePtr()->Gfx_RenderQuad(&texture->quad);

	return Qnil;
}

VALUE RbBitmap::flip_h()
{
	check_raise();

	s32 width = m_bmp.width;
	s32 height = m_bmp.height;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	/*if (!pTexData)
		return Qfalse;*/
	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	for (s32 ly = 0; ly < height; ++ly)
		memcpy(pTexData + (m_bmp.texw * ly), pTempData + (m_bmp.texw * (height - ly - 1)), sizeof(DWORD) * width);
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pTempData);
	return Qnil;
}

VALUE RbBitmap::flip_v()
{
	check_raise();
	
	s32 width = m_bmp.width;
	s32 height = m_bmp.height;
	DWORD* pTexData = GetHgePtr()->Texture_Lock(m_bmp.quad.tex, false);
	/*if (!pTexData)
		return Qfalse;*/
	DWORD* pTempData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(pTempData, pTexData, width * height * sizeof(DWORD));
	for (s32 lx = 0; lx < width; ++lx)
	{
		for (s32 ly = 0; ly < height; ++ly)
			pTexData[m_bmp.texw * ly + lx] = pTempData[m_bmp.texw * ly + (width - lx - 1)];
	}
	GetHgePtr()->Texture_Unlock(m_bmp.quad.tex);
	free(pTempData);
	return Qnil;
}

VALUE RbBitmap::radial_blur(VALUE angle, VALUE division)
{
#pragma message("		Unfinished Function " __FUNCTION__)

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

	return INT2FIX(GetMemWidth());
}

VALUE RbBitmap::get_height()
{
	check_raise();

	return INT2FIX(GetMemHeight());
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
	return Qnil;
}

VALUE RbBitmap::save_to_file(int argc, VALUE *argv, VALUE obj)
{
	check_raise();
#pragma message("		Unfinished Function " __FUNCTION__)

//	VALUE filepath, fileformat;
//
//	if (rb_scan_args(argc, argv, "11", &filepath, &fileformat) == 1)
//		fileformat = INT2FIX(3);
//	else
//		SafeFixnumValue(fileformat);
//
//	SafeStringValue(filepath);
//
//	LPDIRECT3DSURFACE8 pDst = NULL;
//	LPDIRECT3DSURFACE8 pSrc = NULL;
//
//	if (FAILED(GetD3DDevicePtr()->CreateImageSurface(m_bmp.texw, m_bmp.texh, D3DFMT_A8R8G8B8, &pDst)))
//		return Qfalse;
//
//	POINT	pt = {0, };
//	RECT	rt = {0, };
//
//	for (u32 i = 0; i < m_bmp.width; ++i) 
//	{
//		for (u32 j = 0; j < m_bmp.height; ++j) 
//		{
//			int index = i * m_bmp.height + j;
//
//			pt.x = m_tex.sub_textures[0].width * j;
//			pt.y = m_tex.sub_textures[0].height * i;
//
//			rt.right	= SinMin(m_bmp.texw - pt.x,	m_tex.sub_textures[index].width);
//			rt.bottom	= SinMin(m_bmp.texh - pt.y,	m_tex.sub_textures[index].height);
//
//			if (FAILED(((LPDIRECT3DTEXTURE8)m_tex.sub_textures[index].tex)->GetSurfaceLevel(0, &pSrc)))
//				goto failed_return;
//
//			if (FAILED(GetD3DDevicePtr()->CopyRects(pSrc, &rt, 1, pDst, &pt)))
//				goto failed_return;
//
//			pSrc->Release();
//			pSrc = NULL;
//		}
//	}
//
//	//	This function supports the following file formats: .bmp and .dds.
//	if (SUCCEEDED(D3DXSaveSurfaceToFileW(Kconv::UTF8ToUnicode(RSTRING_PTR(filepath)), 
//		D3DXIFF_BMP, pDst, NULL, NULL)))
//	{
//		pDst->Release();
//		pDst = NULL;
//		return Qtrue;
//	}
//	
//failed_return:
//	if (pDst)
//	{
//		pDst->Release();
//		pDst = NULL;
//	}
//	if (pSrc)
//	{
//		pSrc->Release();
//		pSrc = NULL;
//	}
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

imp_method(RbBitmap, render)
imp_method(RbBitmap, flip_h)
imp_method(RbBitmap, flip_v)

imp_attr_reader(RbBitmap, rect)
imp_attr_reader(RbBitmap, width)
imp_attr_reader(RbBitmap, height)
imp_attr_reader(RbBitmap, filename)
imp_attr_accessor(RbBitmap, font)