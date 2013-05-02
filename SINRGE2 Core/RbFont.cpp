/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Font
*/
#include "RbFont.h"
#include "RbColor.h"

VALUE rb_cFont;

#define SafeFontSetSize(_size) do			\
{											\
	SafeFixnumValue(_size);					\
	if(FIX2INT(_size) < 6) rb_raise(rb_eArgError, "bad value for size, the min value is 6.");	\
	if(FIX2INT(_size) > 96) rb_raise(rb_eArgError, "bad value for size, the max value is 96.");	\
} while(0)

VALUE RbFont::__default_name__		= Qnil;
VALUE RbFont::__default_size__		= Qnil;
VALUE RbFont::__default_bold__		= Qnil;
VALUE RbFont::__default_italic__	= Qnil;
VALUE RbFont::__default_color__		= Qnil;
VALUE RbFont::__default_shadow__	= Qnil;

RbFont::RbFont()
	: m_hFont(0)
	, m_name(Qnil)
	, m_color_ptr(0)
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

RbFont::~RbFont()
{
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

void RbFont::InitLibrary()
{
	/**
	 *	@classname
	 *		Font
	 *
	 *	@desc
	 *		字体的类。
	 */
	rb_cFont = rb_define_class_under(rb_mSin, "Font", rb_cObject);

	VALUE __argv[3]		= { INT2FIX(255), INT2FIX(255), INT2FIX(255) };

	__default_name__	= rb_str_new2("simhei");
	
	rb_str_freeze(__default_name__);

	__default_size__	= INT2FIX(16);
	__default_bold__	= Qfalse;
	__default_italic__	= Qfalse;
	__default_color__	= rb_class_new_instance(3, __argv, rb_cColor);
	__default_shadow__	= Qtrue;

	rb_gc_register_address(&__default_name__);
	rb_gc_register_address(&__default_color__);

	// special method
	rb_define_alloc_func(rb_cFont, ObjAllocate<RbFont>);
	rb_define_method(rb_cFont, "initialize", (RbFunc)dm_initialize, -1);

	// object attribute
	rb_define_method(rb_cFont, "name",		(RbFunc)dm_get_name,	0);
	rb_define_method(rb_cFont, "name=",		(RbFunc)dm_set_name,	1);
	rb_define_method(rb_cFont, "size",		(RbFunc)dm_get_size,	0);
	rb_define_method(rb_cFont, "size=",		(RbFunc)dm_set_size,	1);
	rb_define_method(rb_cFont, "bold",		(RbFunc)dm_get_bold,	0);
	rb_define_method(rb_cFont, "bold=",		(RbFunc)dm_set_bold,	1);
	rb_define_method(rb_cFont, "italic",	(RbFunc)dm_get_italic,	0);
	rb_define_method(rb_cFont, "italic=",	(RbFunc)dm_set_italic,	1);
	rb_define_method(rb_cFont, "color",		(RbFunc)dm_get_color,	0);
	rb_define_method(rb_cFont, "color=",	(RbFunc)dm_set_color,	1);
	rb_define_method(rb_cFont, "shadow",	(RbFunc)dm_get_shadow,	0);
	rb_define_method(rb_cFont, "shadow=",	(RbFunc)dm_set_shadow,	1);

	// class attribute
	rb_define_singleton_method(rb_cFont, "exist?",			(RbFunc)dm_is_exist,			1);

	rb_define_singleton_method(rb_cFont, "default_name",	(RbFunc)dm_get_default_name,	0);
	rb_define_singleton_method(rb_cFont, "default_name=",	(RbFunc)dm_set_default_name,	1);
	rb_define_singleton_method(rb_cFont, "default_size",	(RbFunc)dm_get_default_size,	0);
	rb_define_singleton_method(rb_cFont, "default_size=",	(RbFunc)dm_set_default_size,	1);
	rb_define_singleton_method(rb_cFont, "default_bold",	(RbFunc)dm_get_default_bold,	0);
	rb_define_singleton_method(rb_cFont, "default_bold=",	(RbFunc)dm_set_default_bold,	1);
	rb_define_singleton_method(rb_cFont, "default_italic",	(RbFunc)dm_get_default_italic,	0);
	rb_define_singleton_method(rb_cFont, "default_italic=",	(RbFunc)dm_set_default_italic,	1);
	rb_define_singleton_method(rb_cFont, "default_color",	(RbFunc)dm_get_default_color,	0);
	rb_define_singleton_method(rb_cFont, "default_color=",	(RbFunc)dm_set_default_color,	1);
	rb_define_singleton_method(rb_cFont, "default_shadow",	(RbFunc)dm_get_default_shadow,	0);
	rb_define_singleton_method(rb_cFont, "default_shadow=",	(RbFunc)dm_set_default_shadow,	1);

	// supplement
 	rb_define_method(rb_cFont, "to_s",	(RbFunc)dm_to_string,	0);
}

void RbFont::mark()
{
	if (m_color_ptr)	m_color_ptr->MarkObject();

	rb_gc_mark(m_name);
}

int CALLBACK FontCallback(LPENUMLOGFONT lpelf, LPNEWTEXTMETRIC lpntm, DWORD nFontType, long lparam)
{
	return 7;
}

bool RbFont::IsExist(const wchar_t *filename)
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

/**
 *	@call
 *		Font.new						-> font	对象。
 *		Font.new(font_name) 			-> font 对象。
 *		Font.new(font_name, font_size) 	-> font 对象。
 *
 *	@desc
 *		创建一个字体对象。
 */
VALUE RbFont::initialize(int argc, VALUE * argv, VALUE obj)
{
	//	检查参数个数
	if(argc > 2) rb_raise(rb_eArgError, "wrong number of arguments (%d for 2)", argc);

	//	检查参数有效性
	if(argc > 0) SafeStringValue(argv[0]);
	if(argc > 1) SafeFontSetSize(argv[1]);

	//	初始化默认属性
	m_name			= rb_obj_dup(argc > 0 ? argv[0] : dm_get_default_name(rb_cFont));
	m_size			= argc > 1 ? argv[1] : dm_get_default_size(rb_cFont);
	m_bold			= dm_get_default_bold(rb_cFont);
	m_italic		= dm_get_default_italic(rb_cFont);
	m_shadow		= dm_get_default_shadow(rb_cFont);

	VALUE color		= RbColor::dm_clone(dm_get_default_color(rb_cFont));
	m_color_ptr		= GetObjectPtr<RbColor>(color);
	
	//	创建逻辑字体
	m_lfw.lfHeight = FIX2INT(m_size);
	m_lfw.lfItalic = (BYTE)RTEST(m_italic);
	m_lfw.lfWeight = RTEST(m_bold) ? FW_BOLD : FW_NORMAL;
	wcscpy_s(m_lfw.lfFaceName, Kconv::UTF8ToUnicode(RSTRING_PTR(m_name)));

	m_hFont = CreateFontIndirectW(&m_lfw);

	return obj;
}

VALUE RbFont::dm_is_exist(int argc, VALUE name)
{
	SafeStringValue(name);
	return C2RbBool(IsExist(Kconv::UTF8ToUnicode(RSTRING_PTR(name))));
}

VALUE RbFont::get_name()
{
	return m_name;
}

VALUE RbFont::set_name(VALUE name)
{
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

VALUE RbFont::get_size()
{
	return m_size;
}

VALUE RbFont::set_size(VALUE size)
{
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

VALUE RbFont::get_bold()
{
	return m_bold;
}

VALUE RbFont::set_bold(VALUE bold)
{
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

VALUE RbFont::get_italic()
{
	return m_italic;
}

VALUE RbFont::set_italic(VALUE italic)
{
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

VALUE RbFont::get_color()
{
	return ReturnObject(m_color_ptr);
}

VALUE RbFont::set_color(VALUE color)
{
	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<RbColor>(color);
	return color;
}

VALUE RbFont::get_shadow()
{
	return m_shadow;
}

VALUE RbFont::set_shadow(VALUE shadow)
{
	m_shadow = Ruby2RbBool(shadow);
	return shadow;
}

/*
 *	以下定义ruby方法
 */
imp_attr_accessor(RbFont, name)
imp_attr_accessor(RbFont, size)
imp_attr_accessor(RbFont, bold)
imp_attr_accessor(RbFont, italic)
imp_attr_accessor(RbFont, color)
imp_attr_accessor(RbFont, shadow)

VALUE RbFont::dm_get_default_name(VALUE obj)
{
	return __default_name__;
}

VALUE RbFont::dm_set_default_name(VALUE obj, VALUE attr)
{
	SafeStringValue(attr);
	__default_name__ = attr;
	return Qnil;
}

VALUE RbFont::dm_get_default_size(VALUE obj)
{
	return __default_size__;
}

VALUE RbFont::dm_set_default_size(VALUE obj, VALUE attr)
{
	SafeFontSetSize(attr);
	__default_size__ = attr;
	return Qnil;
}

VALUE RbFont::dm_get_default_bold(VALUE obj)
{
	return __default_bold__;
}

VALUE RbFont::dm_set_default_bold(VALUE obj, VALUE attr)
{
	__default_bold__ = Ruby2RbBool(attr);
	return Qnil;
}

VALUE RbFont::dm_get_default_italic(VALUE obj)
{
	return __default_italic__;
}

VALUE RbFont::dm_set_default_italic(VALUE obj, VALUE attr)
{
	__default_italic__ = Ruby2RbBool(attr);
	return Qnil;
}

VALUE RbFont::dm_get_default_color(VALUE obj)
{
	return __default_color__;
}

VALUE RbFont::dm_set_default_color(VALUE obj, VALUE attr)
{
	SafeColorValue(attr);
	__default_color__ = attr;
	return Qnil;
}

VALUE RbFont::dm_get_default_shadow(VALUE obj)
{
	return __default_shadow__;
}

VALUE RbFont::dm_set_default_shadow(VALUE obj, VALUE attr)
{
	__default_shadow__ = Ruby2RbBool(attr);
	return Qnil;
}
