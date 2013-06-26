/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Font
*/
#include "CRbFont.h"
#include "CRbColor.h"
#include "zlib.h"

VALUE rb_cFont;

#define SafeFontSetSize(_size) do			\
{											\
	SafeFixnumValue(_size);					\
	if(FIX2INT(_size) < 6) rb_raise(rb_eArgError, "bad value for size, the min value is 6.");	\
	if(FIX2INT(_size) > 96) rb_raise(rb_eArgError, "bad value for size, the max value is 96.");	\
} while(0)

CRbFont::CRbFont()
	: m_hFont(0)
	, m_name(Qnil)
	, m_color_ptr(0)
	
	, m_count(0)
	, m_disposed(false)
	, m_font_data(0)
{
	memset(&m_lfw, 0, sizeof(LOGFONTW));

	m_lfw.lfCharSet			= DEFAULT_CHARSET;
	m_lfw.lfOutPrecision	= OUT_TT_PRECIS;
	m_lfw.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	m_lfw.lfEscapement		= 0;
	m_lfw.lfOrientation		= 0;
	m_lfw.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	m_lfw.lfQuality			= DEFAULT_QUALITY;//NONANTIALIASED_QUALITY;//DRAFT_QUALITY;//DEFAULT_QUALITY;
	m_lfw.lfWidth			= 0;
	m_lfw.lfHeight			= 20;			// 字号
	m_lfw.lfWeight			= FW_NORMAL;	// 粗体 FW_BOLD
	m_lfw.lfItalic			= 0;			// 斜体
}

CRbFont::~CRbFont()
{
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
}

void CRbFont::InitLibrary()
{
	/**
	 *	@classname
	 *		Font
	 *
	 *	@desc
	 *		字体的类。
	 */
	rb_cFont = rb_define_class_under(rb_mSin, "Font", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cFont, ObjAllocate<CRbFont>);
	rb_define_method(rb_cFont, "initialize",				(RbFunc)dm_initialize,			-1);
	
	// instance method
	rb_define_method(rb_cFont, "dispose",					(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cFont, "disposed?",					(RbFunc)dm_is_disposed,			0);

	// object attribute
	rb_define_method(rb_cFont, "name",						(RbFunc)dm_get_name,			0);
	rb_define_method(rb_cFont, "name=",						(RbFunc)dm_set_name,			1);
	rb_define_method(rb_cFont, "size",						(RbFunc)dm_get_size,			0);
	rb_define_method(rb_cFont, "size=",						(RbFunc)dm_set_size,			1);
	rb_define_method(rb_cFont, "bold",						(RbFunc)dm_get_bold,			0);
	rb_define_method(rb_cFont, "bold=",						(RbFunc)dm_set_bold,			1);
	rb_define_method(rb_cFont, "italic",					(RbFunc)dm_get_italic,			0);
	rb_define_method(rb_cFont, "italic=",					(RbFunc)dm_set_italic,			1);
	rb_define_method(rb_cFont, "color",						(RbFunc)dm_get_color,			0);
	rb_define_method(rb_cFont, "color=",					(RbFunc)dm_set_color,			1);
	rb_define_method(rb_cFont, "shadow",					(RbFunc)dm_get_shadow,			0);
	rb_define_method(rb_cFont, "shadow=",					(RbFunc)dm_set_shadow,			1);

	// class attribute
	rb_define_singleton_method(rb_cFont, "exist?",			(RbFunc)dm_is_exist,			1);

	// supplement
 	rb_define_method(rb_cFont, "to_s",						(RbFunc)dm_to_string,			0);
	rb_define_method(rb_cFont, "clone",						(RbFunc)dm_clone,				0);
}

void CRbFont::mark()
{
	if (m_color_ptr)	m_color_ptr->MarkObject();

	rb_gc_mark(m_name);
}

int CALLBACK FontCallback(LPENUMLOGFONT lpelf, LPNEWTEXTMETRIC lpntm, DWORD nFontType, long lparam)
{
	return 7;
}

bool CRbFont::IsExist(const wchar_t *filename)
{
	static LOGFONTW lfw;
	wcscpy_s(lfw.lfFaceName, filename);
	int lp = 0;
	HDC hScreenDC = GetDC(NULL);

	if (EnumFontFamiliesEx(hScreenDC, &lfw, (FONTENUMPROC)FontCallback, lp, 0) == 7)
		return true;
	else
		return false;
}

int CRbFont::GetTextWidth(const wchar_t * text)
{
	DWORD len = wcslen(text);
	int width = 0, size = FIX2LONG(m_size);
	for (DWORD i = 0; i < len; ++i)
	{
		if (text[i] < 128)
			width += size / 2;
		else
			width += size;
	}
	return width;
}

VALUE CRbFont::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE v_name, v_size, v_lfont, v_frombuf;

	rb_scan_args(argc, argv, "22", &v_name, &v_size, &v_lfont, &v_frombuf);

	SafeStringValue(v_name);
	SafeFontSetSize(v_size);
	
	//	初始化默认属性
	m_name			= rb_obj_dup(v_name);
	m_size			= v_size;
	m_bold			= Qfalse;
	m_italic		= Qfalse;
	m_shadow		= Qfalse;

	if (RTEST(v_lfont))
	{
		u32 size  = FIX2ULONG(v_size);
		
		if (size != 12 && size != 14 && size != 16)
			rb_raise(rb_eArgError, "bad value for size: %d.", size);

		if (size == 14) m_count = 16 * 16 / 8;
		else m_count = size * size / 8;
		
		size = MAX_SIZE * m_count;
		m_font_data = (u8 *)malloc(size);
		//memset(m_font_data, 0, size);
		if (RTEST(v_frombuf))
		{
			memcpy(m_font_data, RSTRING_PTR(m_name), size);
			m_name = Qnil;
		}
		else
		{
			char * filename = Kconv::UTF8ToAnsi(RSTRING_PTR(m_name));
		
			FILE * fp = 0;
			fopen_s(&fp, filename, "rb");
			if (fp == NULL) rb_raise(rb_eSinError, "Failed to load lattice font.");
			long fsize = _filelength(_fileno(fp));
			void * temp_data = malloc(fsize);
			fread(temp_data, fsize, 1, fp);
			fclose(fp);
			if (uncompress(m_font_data, &size, (u8 *)temp_data, fsize) != Z_OK)
				rb_raise(rb_eSinError, "Failed to uncompress lattice font data.");
			free(temp_data);
			temp_data = NULL;
		}
	}
	else
	{
		//	创建逻辑字体
		m_lfw.lfHeight = FIX2INT(m_size);
		m_lfw.lfItalic = (BYTE)RTEST(m_italic);
		m_lfw.lfWeight = RTEST(m_bold) ? FW_BOLD : FW_NORMAL;
		wcscpy_s(m_lfw.lfFaceName, Kconv::UTF8ToUnicode(RSTRING_PTR(m_name)));

		m_hFont = CreateFontIndirectW(&m_lfw);
	}

	VALUE __argv[1]	= { ULONG2NUM(0xffffffff) };
	VALUE color		= rb_class_new_instance(1, __argv, rb_cColor);//CRbColor::dm_clone(dm_get_default_color(rb_cFont));
	m_color_ptr		= GetObjectPtr<CRbColor>(color);

	return obj;
}

void CRbFont::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed font");
}

VALUE CRbFont::dispose()
{
	if (m_disposed)
		return Qnil;
	
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (m_font_data)
	{
		free(m_font_data);
		m_font_data= NULL;
	}
	m_disposed = true;
	return Qnil;
}

VALUE CRbFont::is_disposed()
{
	return C2RbBool(m_disposed);
}

VALUE CRbFont::clone()
{
	check_raise();

	VALUE __argv[2] = { m_name, m_size };
	VALUE font = rb_class_new_instance(2, __argv, obj_class());
	CRbFont * font_ptr = GetObjectPtr<CRbFont>(font);
	font_ptr->dm_set_bold(font, m_bold);
	font_ptr->dm_set_italic(font, m_italic);
	font_ptr->dm_set_shadow(font, m_shadow);
	font_ptr = NULL;
	return font;
}

VALUE CRbFont::dm_is_exist(int argc, VALUE name)
{
	SafeStringValue(name);
	return C2RbBool(IsExist(Kconv::UTF8ToUnicode(RSTRING_PTR(name))));
}

VALUE CRbFont::get_name()
{
	return m_name;
}

VALUE CRbFont::set_name(VALUE name)
{
	if (m_font_data) return Qfalse;

	SafeStringValue(name);

	if (rb_str_cmp(m_name, name) == 0)
		return Qnil;

	m_name = name;

	// 重新创建逻辑字体
	wcscpy_s(m_lfw.lfFaceName, Kconv::UTF8ToUnicode(RSTRING_PTR(name)));

	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	m_hFont = CreateFontIndirectW(&m_lfw);

	return name;
}

VALUE CRbFont::get_size()
{
	return m_size;
}

VALUE CRbFont::set_size(VALUE size)
{
	if (m_font_data) return Qfalse;

	SafeFontSetSize(size);

	if(m_size == size)
		return Qnil;

	m_size = size;

	// 重新创建逻辑字体
	m_lfw.lfHeight = FIX2INT(size);

	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	m_hFont = CreateFontIndirectW(&m_lfw);

	return size;
}

VALUE CRbFont::get_bold()
{
	return m_bold;
}

VALUE CRbFont::set_bold(VALUE bold)
{
	if (m_font_data) return Qfalse;

	bold = Ruby2RbBool(bold);

	if(m_bold == bold)
		return Qnil;

	m_bold = bold;

	// 重新创建逻辑字体
	m_lfw.lfWeight = RTEST(bold) ? FW_BOLD : FW_NORMAL;

	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	m_hFont = CreateFontIndirectW(&m_lfw);


	return bold;
}

VALUE CRbFont::get_italic()
{
	return m_italic;
}

VALUE CRbFont::set_italic(VALUE italic)
{
	if (m_font_data) return Qfalse;

	italic = Ruby2RbBool(italic);

	if(m_italic == italic)
		return Qnil;

	m_italic = italic;

	// 重新创建逻辑字体
	m_lfw.lfItalic = (BYTE)RTEST(italic);

	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	m_hFont = CreateFontIndirectW(&m_lfw);

	return italic;
}

VALUE CRbFont::get_color()
{
	check_raise();

	return ReturnObject(m_color_ptr);
}

VALUE CRbFont::set_color(VALUE color)
{
	check_raise();

	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<CRbColor>(color);
	return color;
}

VALUE CRbFont::get_shadow()
{
	check_raise();

	return m_shadow;
}

VALUE CRbFont::set_shadow(VALUE shadow)
{
	check_raise();

	m_shadow = Ruby2RbBool(shadow);
	return shadow;
}

/*
 *	以下定义ruby方法
 */

imp_method(CRbFont, dispose)
imp_method(CRbFont, is_disposed)

imp_attr_accessor(CRbFont, name)
imp_attr_accessor(CRbFont, size)
imp_attr_accessor(CRbFont, bold)
imp_attr_accessor(CRbFont, italic)
imp_attr_accessor(CRbFont, color)
imp_attr_accessor(CRbFont, shadow)
