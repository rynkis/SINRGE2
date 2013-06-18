/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Color
*/
#include "CRbColor.h"
#include "sin_types.h"
#include "sin_color.h"

VALUE rb_cColor;

CRbColor::CRbColor()
	: m_color(0)
	, m_red(RUBY_0)
	, m_green(RUBY_0)
	, m_blue(RUBY_0)
	, m_alpha(RUBY_0)
{
	memset(m_col_data, 0, sizeof(m_col_data));
}

void CRbColor::InitLibrary()
{
	/**
	 *	@classname
	 *		Color
	 *
	 *	@desc
	 *		ARGB 颜色的类。
	 */
	rb_cColor = rb_define_class_under(rb_mSin, "Color", rb_cObject);
	
	// special method
	rb_define_alloc_func(rb_cColor, ObjAllocate<CRbColor>);
	rb_define_method(rb_cColor, "initialize", (RbFunc)dm_initialize,	-1);

	// class method
	rb_define_singleton_method(rb_cColor, "_load",	(RbFunc)dm_load,	1);

	// instance method
	rb_define_method(rb_cColor, "set",		(RbFunc)dm_set,			-1);
	rb_define_method(rb_cColor, "_dump",	(RbFunc)dm_dump,		1);

	// object attribute
	rb_define_method(rb_cColor, "red",		(RbFunc)dm_get_red,		0);
	rb_define_method(rb_cColor, "red=",		(RbFunc)dm_set_red,		1);
	rb_define_method(rb_cColor, "green",	(RbFunc)dm_get_green,	0);
	rb_define_method(rb_cColor, "green=",	(RbFunc)dm_set_green,	1);
	rb_define_method(rb_cColor, "blue",		(RbFunc)dm_get_blue,	0);
	rb_define_method(rb_cColor, "blue=",	(RbFunc)dm_set_blue,	1);
	rb_define_method(rb_cColor, "alpha",	(RbFunc)dm_get_alpha,	0);
	rb_define_method(rb_cColor, "alpha=",	(RbFunc)dm_set_alpha,	1);

	// supplement
 	rb_define_method(rb_cColor, "to_s",		(RbFunc)dm_to_string,	0);
	rb_define_method(rb_cColor, "clone",	(RbFunc)dm_clone,		0);
}

void CRbColor::mark()
{
	rb_gc_mark(m_red);
	rb_gc_mark(m_green);
	rb_gc_mark(m_blue);
	rb_gc_mark(m_alpha);
}

/**
 *	@call
 *		Color.new(red, green, blue) 		-> color 对象。
 *		Color.new(red, green, blue, alpha) 	-> color 对象。
 *
 *	@desc
 *		生成 Color 对象。如省略 alpha 的话则默认为 255。
 */
VALUE CRbColor::initialize(int argc, VALUE * argv, VALUE obj)
{
	if (argc == 1)
	{
		if (rb_obj_is_kind_of(argv[0], rb_cInteger))
		{
			DWORD col = NUM2ULONG(argv[0]);
			BYTE r, g, b, a;
			GET_ARGB_8888(col, a, r, g, b);
			m_col_data[0] = r;
			m_col_data[1] = g;
			m_col_data[2] = b;
			m_col_data[3] = a;
		}
		else
		{
			SafeColorValue(argv[0]);
			CRbColor * color = GetObjectPtr<CRbColor>(argv[0]);
			memcpy(m_col_data, color->m_col_data, sizeof(m_col_data));
			color = NULL;
		}
	}
	else
	{
		rb_scan_args(argc, argv, "31", &m_red, &m_green, &m_blue, &m_alpha);

		for (int i = 0; i < argc; ++i)
			SafeNumericValue(argv[i]);

		m_col_data[0] = NUM2DBL(m_red);
		m_col_data[1] = NUM2DBL(m_green);
		m_col_data[2] = NUM2DBL(m_blue);
		m_col_data[3] = (NIL_P(m_alpha) ? 255 : NUM2DBL(m_alpha));
		
 		m_col_data[0] = SinBound(m_col_data[0], 0, 255);
 		m_col_data[1] = SinBound(m_col_data[1], 0, 255);
 		m_col_data[2] = SinBound(m_col_data[2], 0, 255);
 		m_col_data[3] = SinBound(m_col_data[3], 0, 255);
	}

	m_color = MAKE_ARGB_8888((BYTE)m_col_data[3], (BYTE)m_col_data[0], (BYTE)m_col_data[1], (BYTE)m_col_data[2]);

	m_red	= DBL2NUM(m_col_data[0]);
	m_green = DBL2NUM(m_col_data[1]);
	m_blue	= DBL2NUM(m_col_data[2]);
	m_alpha = DBL2NUM(m_col_data[3]);

	return obj;
}

VALUE CRbColor::set(int argc, VALUE * argv, VALUE obj)
{
	return initialize(argc, argv, obj);
}

VALUE CRbColor::_dump(VALUE depth)
{
	return rb_str_new((const char *)&m_col_data[0], sizeof(m_col_data));
}

VALUE CRbColor::clone()
{
	VALUE __argv[4] = { m_red, m_green, m_blue, m_alpha };

	return rb_class_new_instance(4, __argv, obj_class());
}

VALUE CRbColor::to_string()
{
	return rb_sprintf("#<%s(%f, %f, %f, %f)>", obj_classname(), m_col_data[0], m_col_data[1], m_col_data[2], m_col_data[3]);
}

VALUE CRbColor::get_red()
{
	return m_red;
}

VALUE CRbColor::set_red(VALUE red)
{
	SafeNumericValue(red);

	m_col_data[0] = NUM2DBL(red);
	m_col_data[0] = SinBound(m_col_data[0], 0, 255);

	m_color = MAKE_ARGB_8888((BYTE)m_col_data[3], (BYTE)m_col_data[0], (BYTE)m_col_data[1], (BYTE)m_col_data[2]);
	m_red	= DBL2NUM(m_col_data[0]);

	return m_red;
}

VALUE CRbColor::get_green()
{
	return m_green;
}

VALUE CRbColor::set_green(VALUE green)
{
	SafeNumericValue(green);

	m_col_data[1] = NUM2DBL(green);
	m_col_data[1] = SinBound(m_col_data[1], 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_col_data[3], (BYTE)m_col_data[0], (BYTE)m_col_data[1], (BYTE)m_col_data[2]);
	m_green	= DBL2NUM(m_col_data[1]);

	return m_green;
}

VALUE CRbColor::get_blue()
{
	return m_blue;
}

VALUE CRbColor::set_blue(VALUE blue)
{
	SafeNumericValue(blue);

	m_col_data[2] = NUM2DBL(blue);
	m_col_data[2] = SinBound(m_col_data[2], 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_col_data[3], (BYTE)m_col_data[0], (BYTE)m_col_data[1], (BYTE)m_col_data[2]);
	m_blue	= DBL2NUM(m_col_data[2]);

	return m_blue;
}

VALUE CRbColor::get_alpha()
{
	return m_alpha;
}

VALUE CRbColor::set_alpha(VALUE alpha)
{
	SafeNumericValue(alpha);

	m_col_data[3] = NUM2DBL(alpha);
	m_col_data[3] = SinBound(m_col_data[3], 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_col_data[3], (BYTE)m_col_data[0], (BYTE)m_col_data[1], (BYTE)m_col_data[2]);
	m_alpha	= DBL2NUM(m_col_data[3]);

	return m_alpha;
}

/*
 *	以下定义ruby方法
 */
VALUE CRbColor::dm_load(VALUE klass, VALUE str)
{
	if (4 * sizeof(double) != RSTRING_LEN(str))	//	error
		return Qnil;
	double * col = (double *)RSTRING_PTR(str);
	VALUE __argv[4] = { DBL2NUM(col[0]), DBL2NUM(col[1]), DBL2NUM(col[2]), DBL2NUM(col[3]) };
	VALUE new_col = rb_class_new_instance(4, __argv, klass);
	col = NULL;
	return new_col;
}

imp_method_vargs(CRbColor, set)

imp_attr_accessor(CRbColor, red)
imp_attr_accessor(CRbColor, green)
imp_attr_accessor(CRbColor, blue)
imp_attr_accessor(CRbColor, alpha)