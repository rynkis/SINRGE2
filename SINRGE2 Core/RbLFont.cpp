/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class LFont
*/
#include "RbLFont.h"
#include "RbColor.h"
#include "RbBitmap.h"
#include "sin_app.h"

VALUE rb_cLFont;

RbLFont::RbLFont()
	: m_filename(Qnil)
	, m_size(0)
	, m_length(0)
	, m_shadow(Qfalse)
	, m_color_ptr(0)
	, m_closed(false)
	, m_font_data(0)
{
}

RbLFont::~RbLFont()
{
	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
}

void RbLFont::InitLibrary()
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
	rb_define_alloc_func(rb_cLFont, ObjAllocate<RbLFont>);
	rb_define_method(rb_cLFont, "initialize",			(RbFunc)dm_initialize, -1);
	
	// instance method
	rb_define_method(rb_cLFont, "close",				(RbFunc)dm_close,		0);
	rb_define_method(rb_cLFont, "closed?",				(RbFunc)dm_is_closed,	0);
	
	rb_define_method(rb_cLFont, "font_bitmap",			(RbFunc)dm_get_font_bmp,1);
	rb_define_method(rb_cLFont, "text_size",			(RbFunc)dm_text_size,	1);
	//rb_define_method(rb_cLFont, "draw_text",			(RbFunc)dm_draw_text,	-1);
	

	// object attribute
	rb_define_method(rb_cLFont, "size",					(RbFunc)dm_get_size,	0);
	rb_define_method(rb_cLFont, "color",				(RbFunc)dm_get_color,	0);
	rb_define_method(rb_cLFont, "color=",				(RbFunc)dm_set_color,	1);
	rb_define_method(rb_cLFont, "shadow",				(RbFunc)dm_get_shadow,	0);
	rb_define_method(rb_cLFont, "shadow=",				(RbFunc)dm_set_shadow,	1);

	// supplement
 	rb_define_method(rb_cLFont, "to_s",					(RbFunc)dm_to_string,	0);
}

void RbLFont::mark()
{
	if (m_color_ptr)	m_color_ptr->MarkObject();

	if (RTEST(m_filename))
		rb_gc_mark(m_filename);
}

VALUE RbLFont::initialize(int argc, VALUE * argv, VALUE obj)
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
		fread(m_font_data, size, 1, fp);
		fclose(fp);
	}

	VALUE __argv[3]	= { INT2FIX(255), INT2FIX(255), INT2FIX(255) };
	VALUE color		= rb_class_new_instance(3, __argv, rb_cColor);
	m_color_ptr		= GetObjectPtr<RbColor>(color);
	
	return obj;
}

void RbLFont::check_raise()
{
	if (m_closed)
		rb_raise(rb_eSinError, "closed lattice font");
}

VALUE RbLFont::close()
{
	if (m_closed)
		return Qnil;

	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
	m_closed = true;
	return Qnil;
}

VALUE RbLFont::is_closed()
{
	return C2RbBool(m_closed);
}

VALUE RbLFont::get_font_bmp(VALUE str)
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
	RbBitmap * bmp_p =  GetObjectPtr<RbBitmap>(bitmap);
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

VALUE RbLFont::text_size(VALUE str)
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

VALUE RbLFont::get_size()
{
	check_raise();

	return INT2FIX(m_size);
}

VALUE RbLFont::get_color()
{
	check_raise();

	return ReturnObject(m_color_ptr);
}

VALUE RbLFont::set_color(VALUE color)
{
	check_raise();

	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<RbColor>(color);
	return color;
}

VALUE RbLFont::get_shadow()
{
	check_raise();

	return m_shadow;
}

VALUE RbLFont::set_shadow(VALUE shadow)
{
	check_raise();

	m_shadow = Ruby2RbBool(shadow);
	return shadow;
}


imp_method(RbLFont, close)
imp_method(RbLFont, is_closed)
imp_method(RbLFont, get_size)
imp_method01(RbLFont, get_font_bmp)
imp_method01(RbLFont, text_size)
//imp_method_vargs(RbLFont, draw_text)

imp_attr_accessor(RbLFont, color)
imp_attr_accessor(RbLFont, shadow)
