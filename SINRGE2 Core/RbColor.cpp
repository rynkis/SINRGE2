/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Color
*/
#include "RbColor.h"
#include "sin_types.h"
#include "sin_color.h"

VALUE rb_cColor;

RbColor::RbColor()
	: m_color(0)
	, m_r(0)
	, m_g(0)
	, m_b(0)
	, m_a(0)
	, m_red(RUBY_0)
	, m_green(RUBY_0)
	, m_blue(RUBY_0)
	, m_alpha(RUBY_0)
{
}

void RbColor::InitLibrary()
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
	rb_define_alloc_func(rb_cColor, ObjAllocate<RbColor>);
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

/*
 *	以下为ruby方法的c实现
 */

/**
 *	@call
 *		Color.new(red, green, blue) 		-> color 对象。
 *		Color.new(red, green, blue, alpha) 	-> color 对象。
 *
 *	@desc
 *		生成 Color 对象。如省略 alpha 的话则默认为 255。
 */
VALUE RbColor::initialize(int argc, VALUE * argv, VALUE obj)
{
	if (argc == 1)
	{
		if (rb_obj_is_kind_of(argv[0], rb_cInteger))
		{
			DWORD col = NUM2ULONG(argv[0]);
			GET_ARGB_8888(col, m_a, m_r, m_g, m_b);
		}
		else
		{
			SafeColorValue(argv[0]);
			RbColor * color = GetObjectPtr<RbColor>(argv[0]);
			m_r = color->m_r;
			m_g = color->m_g;
			m_b = color->m_b;
			m_a = color->m_a;
			color = NULL;
		}
	}
	else
	{
		rb_scan_args(argc, argv, "31", &m_red, &m_green, &m_blue, &m_alpha);

		for (int i = 0; i < argc; ++i)
			SafeNumericValue(argv[i]);

		m_r = NUM2DBL(m_red);
		m_g = NUM2DBL(m_green);
		m_b = NUM2DBL(m_blue);
		m_a = (NIL_P(m_alpha) ? 255 : NUM2DBL(m_alpha));

 		m_r = SinBound(m_r, 0, 255);
		m_g = SinBound(m_g, 0, 255);
		m_b = SinBound(m_b, 0, 255);
		m_a = SinBound(m_a, 0, 255);
	}

	m_color = MAKE_ARGB_8888((BYTE)m_a, (BYTE)m_r, (BYTE)m_g, (BYTE)m_b);

	m_red	= DBL2NUM(m_r);
	m_green = DBL2NUM(m_g);
	m_blue	= DBL2NUM(m_b);
	m_alpha = DBL2NUM(m_a);

	return obj;
}

VALUE RbColor::set(int argc, VALUE * argv, VALUE obj)
{
	return initialize(argc, argv, obj);
}

VALUE RbColor::_dump(VALUE depth)
{
	return rb_str_new((const char *)&m_dump_data[0], sizeof(m_dump_data));
}

VALUE RbColor::clone()
{
	VALUE __argv[4] = { m_red, m_green, m_blue, m_alpha };

	return rb_class_new_instance(4, __argv, obj_class());
}

VALUE RbColor::to_string()
{
	return rb_sprintf("#<%s(%f, %f, %f, %f)>", obj_classname(), m_r, m_g, m_b, m_a);
}

VALUE RbColor::get_red()
{
	return m_red;
}

VALUE RbColor::set_red(VALUE red)
{
	SafeNumericValue(red);

	m_r = NUM2DBL(red);
	m_r = SinBound(m_r, 0, 255);

	m_color = MAKE_ARGB_8888((BYTE)m_a, (BYTE)m_r, (BYTE)m_g, (BYTE)m_b);
	m_red	= DBL2NUM(m_r);

	return m_red;
}

VALUE RbColor::get_green()
{
	return m_green;
}

VALUE RbColor::set_green(VALUE green)
{
	SafeNumericValue(green);

	m_g = NUM2DBL(green);
	m_g = SinBound(m_g, 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_a, (BYTE)m_r, (BYTE)m_g, (BYTE)m_b);
	m_green	= DBL2NUM(m_g);

	return m_green;
}

VALUE RbColor::get_blue()
{
	return m_blue;
}

VALUE RbColor::set_blue(VALUE blue)
{
	SafeNumericValue(blue);

	m_b = NUM2DBL(blue);
	m_b = SinBound(m_b, 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_a, (BYTE)m_r, (BYTE)m_g, (BYTE)m_b);
	m_blue	= DBL2NUM(m_b);

	return m_blue;
}

VALUE RbColor::get_alpha()
{
	return m_alpha;
}

VALUE RbColor::set_alpha(VALUE alpha)
{
	SafeNumericValue(alpha);

	m_a = NUM2DBL(alpha);
	m_a = SinBound(m_a, 0, 255);
	
	m_color = MAKE_ARGB_8888((BYTE)m_a, (BYTE)m_r, (BYTE)m_g, (BYTE)m_b);
	m_alpha	= DBL2NUM(m_a);

	return m_alpha;
}

/*
 *	以下定义ruby方法
 */
VALUE RbColor::dm_load(VALUE klass, VALUE str)
{
	if (4 * sizeof(VALUE) != RSTRING_LEN(str))	//	error
		return Qnil;

	return rb_class_new_instance(4, (VALUE*)RSTRING_PTR(str), klass);
}

imp_method_vargs(RbColor, set)

imp_attr_accessor(RbColor, red)
imp_attr_accessor(RbColor, green)
imp_attr_accessor(RbColor, blue)
imp_attr_accessor(RbColor, alpha)