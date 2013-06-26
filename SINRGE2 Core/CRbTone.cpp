/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Tone
*/
#include "CRbTone.h"
#include "sin_color.h"

VALUE rb_cTone;

void CRbTone::InitLibrary()
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
	rb_define_alloc_func(rb_cTone, ObjAllocate<CRbTone>);
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

VALUE CRbTone::initialize(int argc, VALUE * argv, VALUE obj)
{
	if (argc == 1)
	{
		if (rb_obj_is_kind_of(argv[0], rb_cInteger))
		{
			u32 col = NUM2ULONG(argv[0]);
			BYTE r, g, b, a;
			GET_ARGB_8888(col, a, r, g, b);
			m_col_data[0] = r;
			m_col_data[1] = g;
			m_col_data[2] = b;
			m_col_data[3] = a;
		}
		else
		{
			SafeToneValue(argv[0]);

			CRbTone * tone = GetObjectPtr<CRbTone>(argv[0]);
			memcpy(m_col_data, tone->m_col_data, sizeof(m_col_data));
			tone = NULL;
		}
	}
	else
	{
		rb_scan_args(argc, argv, "31", &m_red, &m_green, &m_blue, &m_alpha);

		for (int i = 0; i < argc; ++i)
		{
			SafeNumericValue(argv[i]);
		}
		
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