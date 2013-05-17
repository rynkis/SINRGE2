/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Tone
*/
#include "RbTone.h"
#include "sin_types.h"
#include "sin_color.h"

VALUE rb_cTone;

void RbTone::InitLibrary()
{
	/**
	 *	@classname
	 *		Tone
	 *
	 *	@desc
	 *		色调的类。
	 */
	rb_cTone = rb_define_class_under(rb_mSin, "Tone", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cTone, ObjAllocate<RbTone>);
	rb_define_method(rb_cTone, "initialize", (RbFunc)dm_initialize,		-1);

	// class method
	rb_define_singleton_method(rb_cTone, "_load",	(RbFunc)dm_load,	1);

	// instance method
	rb_define_method(rb_cTone, "set",		(RbFunc)dm_set,			-1);
	rb_define_method(rb_cTone, "_dump",		(RbFunc)dm_dump,		1);

	// object attribute
	rb_define_method(rb_cTone, "red",		(RbFunc)dm_get_red,		0);
	rb_define_method(rb_cTone, "red=",		(RbFunc)dm_set_red,		1);
	rb_define_method(rb_cTone, "green",		(RbFunc)dm_get_green,	0);
	rb_define_method(rb_cTone, "green=",	(RbFunc)dm_set_green,	1);
	rb_define_method(rb_cTone, "blue",		(RbFunc)dm_get_blue,	0);
	rb_define_method(rb_cTone, "blue=",		(RbFunc)dm_set_blue,	1);
	rb_define_method(rb_cTone, "gray",		(RbFunc)dm_get_alpha,	0);
	rb_define_method(rb_cTone, "gray=",		(RbFunc)dm_set_alpha,	1);

	// supplement
 	rb_define_method(rb_cTone, "to_s",		(RbFunc)dm_to_string,	0);
	rb_define_method(rb_cTone, "clone",		(RbFunc)dm_clone,		0);
}

VALUE RbTone::initialize(int argc, VALUE * argv, VALUE obj)
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
			SafeToneValue(argv[0]);

			RbTone * tone = GetObjectPtr<RbTone>(argv[0]);

			m_r = tone->m_r;
			m_g = tone->m_g;
			m_b = tone->m_b;
			m_a = tone->m_a;
		}
	}
	else
	{
		rb_scan_args(argc, argv, "31", &m_red, &m_green, &m_blue, &m_alpha);

		for (int i = 0; i < argc; ++i)
		{
			SafeNumericValue(argv[i]);
		}

		m_r = NUM2DBL(m_red);
		m_g = NUM2DBL(m_green);
		m_b = NUM2DBL(m_blue);
		m_a = (NIL_P(m_alpha) ? 0 : NUM2DBL(m_alpha));

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