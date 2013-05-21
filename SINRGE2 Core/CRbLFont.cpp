/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class LFont
*/
#include "CRbLFont.h"
#include "CRbColor.h"
#include "CRbBitmap.h"
#include "sin_app.h"
#include "zlib.h"

VALUE rb_cLFont;

CRbLFont::CRbLFont()
	: m_filename(Qnil)
	, m_size(0)
	, m_length(0)
	, m_shadow(Qfalse)
	, m_color_ptr(0)
	, m_disposed(false)
	, m_font_data(0)
{
}

CRbLFont::~CRbLFont()
{
	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
}

void CRbLFont::InitLibrary()
{
	/**
	 *	@classname
	 *		Font
	 *
	 *	@desc
	 *		×ÖÌåµÄÀà¡£
	 */
	rb_cLFont = rb_define_class_under(rb_mSin, "LFont", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cLFont, ObjAllocate<CRbLFont>);
	rb_define_method(rb_cLFont, "initialize",			(RbFunc)dm_initialize, -1);
	
	// instance method
	rb_define_method(rb_cLFont, "dispose",				(RbFunc)dm_dispose,		0);
	rb_define_method(rb_cLFont, "disposed?",			(RbFunc)dm_is_disposed,	0);
	
	rb_define_method(rb_cLFont, "char_bitmap",			(RbFunc)dm_get_char_bmp,1);
	rb_define_method(rb_cLFont, "char_width",			(RbFunc)dm_char_width,	1);
	//rb_define_method(rb_cLFont, "draw_text",			(RbFunc)dm_draw_text,	-1);
	

	// object attribute
	rb_define_method(rb_cLFont, "size",					(RbFunc)dm_get_size,	0);
	rb_define_method(rb_cLFont, "color",				(RbFunc)dm_get_color,	0);
	rb_define_method(rb_cLFont, "color=",				(RbFunc)dm_set_color,	1);
	/*rb_define_method(rb_cLFont, "shadow",				(RbFunc)dm_get_shadow,	0);
	rb_define_method(rb_cLFont, "shadow=",				(RbFunc)dm_set_shadow,	1);*/

	// supplement
 	rb_define_method(rb_cLFont, "to_s",					(RbFunc)dm_to_string,	0);
}

void CRbLFont::mark()
{
	if (m_color_ptr)	m_color_ptr->MarkObject();

	if (RTEST(m_filename))
		rb_gc_mark(m_filename);
}

VALUE CRbLFont::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE arg01, arg02, arg03;
	rb_scan_args(argc, argv, "21", &arg01, &arg02, &arg03);

	SafeStringValue(arg01);
	SafeFixnumValue(arg02);

	m_filename = arg01;
	m_size = FIX2INT(arg02);
	
	if (m_size != 12 && m_size != 14 && m_size != 16)
		rb_raise(rb_eArgError, "bad value for size: %d.", m_size);

	if (m_size == 14) m_length = 16 * 16 / 8;
	else m_length = m_size * m_size / 8;
	
	DWORD size = MAX_SIZE * m_length;
	m_font_data = (BYTE *)malloc(size);
	//memset(m_font_data, 0, size);
	if (RTEST(arg03))
	{
		memcpy(m_font_data, RSTRING_PTR(m_filename), size);
		m_filename = Qnil;
	}
	else
	{
		char * filename = Kconv::UTF8ToAnsi(RSTRING_PTR(m_filename));
		
		FILE * fp = 0;
		fopen_s(&fp, filename, "rb");
		if (fp == NULL) rb_raise(rb_eSinError, "Failed to load lattice font.");
		long fsize = _filelength(_fileno(fp));
		void * temp_data = malloc(fsize);
		fread(temp_data, fsize, 1, fp);
		fclose(fp);
		if (uncompress(m_font_data, &size, (BYTE *)temp_data, fsize) != Z_OK)
			rb_raise(rb_eSinError, "Failed to uncompress lattice font data.");
		free(temp_data);
		temp_data = NULL;
	}

	VALUE __argv[3]	= { INT2FIX(255), INT2FIX(255), INT2FIX(255) };
	VALUE color		= rb_class_new_instance(3, __argv, rb_cColor);
	m_color_ptr		= GetObjectPtr<CRbColor>(color);
	
	return obj;
}

void CRbLFont::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed lattice font");
}

VALUE CRbLFont::dispose()
{
	if (m_disposed)
		return Qnil;

	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
	m_disposed = true;
	return Qnil;
}

VALUE CRbLFont::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE CRbLFont::get_char_bmp(VALUE str)
{
	check_raise();

	SafeStringValue(str);
	DWORD char_index = Kconv::UTF8ToUnicode(RSTRING_PTR(str))[0];
	VALUE bitmap;
	if (m_size == 14)
	{
		VALUE __argv[2]	= { INT2FIX(16), INT2FIX(16) };
		bitmap = rb_class_new_instance(2, __argv, rb_cBitmap);
	}
	else
	{
		VALUE __argv[2]	= { INT2FIX(m_size), INT2FIX(m_size) };
		bitmap = rb_class_new_instance(2, __argv, rb_cBitmap);
	}
	CRbBitmap * bmp_p =  GetObjectPtr<CRbBitmap>(bitmap);
	HGE * hge = GetAppPtr()->GetHgePtr();

	DWORD * data = hge->Texture_Lock(bmp_p->GetBitmapPtr()->quad.tex, false);
	if (!data) goto __failed_return;

	BYTE fc = 0;

	DWORD offset = 0;
	for (int i = 0; i < m_length; ++i)
	{
		fc = m_font_data[char_index * m_length + i];
		for (int l = 0; l < 8; ++l)
		{
			if (fc & (1 << l))
				data[offset] = m_color_ptr->GetColor();
			else
				data[offset] = 0;

			offset++;
		}
	}

	hge->Texture_Unlock(bmp_p->GetBitmapPtr()->quad.tex);

	bmp_p = NULL;
	return bitmap;

__failed_return:
	if (bmp_p)
		bmp_p = NULL;
	return Qnil;
}

VALUE CRbLFont::char_width(VALUE str)
{
	SafeStringValue(str);
	DWORD char_index = Kconv::UTF8ToUnicode(RSTRING_PTR(str))[0];
	if (char_index < 128)
		return INT2FIX(m_size / 2);
	else
		return INT2FIX(m_size);
	/*wchar_t * text = Kconv::UTF8ToUnicode(RSTRING_PTR(str));
	DWORD len = wcslen(text);
	long width = 0;
	for (DWORD l = 0; l < len; ++l)
	{
		if (text[len] < 128)
			width += m_size / 2;
		else
			width += m_size;
	}
	return LONG2FIX(width);*/
}

VALUE CRbLFont::get_size()
{
	check_raise();

	return INT2FIX(m_size);
}

VALUE CRbLFont::get_color()
{
	check_raise();

	return ReturnObject(m_color_ptr);
}

VALUE CRbLFont::set_color(VALUE color)
{
	check_raise();

	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<CRbColor>(color);
	return color;
}

//VALUE CRbLFont::get_shadow()
//{
//	check_raise();
//
//	return m_shadow;
//}
//
//VALUE CRbLFont::set_shadow(VALUE shadow)
//{
//	check_raise();
//
//	m_shadow = Ruby2RbBool(shadow);
//	return shadow;
//}


imp_method(CRbLFont, dispose)
imp_method(CRbLFont, is_disposed)
imp_method(CRbLFont, get_size)
imp_method01(CRbLFont, get_char_bmp)
imp_method01(CRbLFont, char_width)
//imp_method_vargs(CRbLFont, draw_text)

imp_attr_accessor(CRbLFont, color)
//imp_attr_accessor(CRbLFont, shadow)
