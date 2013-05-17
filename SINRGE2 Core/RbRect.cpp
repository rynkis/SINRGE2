/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Rect
*/
#include "RbRect.h"

VALUE rb_cRect;

void RbRect::InitLibrary()
{
	/**
	 *	@classname
	 *		Rect
	 *
	 *	@desc
	 *		¾ØÐÎµÄÀà¡£
	 */
	rb_cRect = rb_define_class_under(rb_mSin, "Rect", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cRect, ObjAllocate<RbRect>);
	rb_define_method(rb_cRect, "initialize", (RbFunc)dm_initialize, -1);

	// class method
	rb_define_singleton_method(rb_cRect, "_load", (RbFunc)dm_load, 1);

	// instance method
	rb_define_method(rb_cRect, "set",		(RbFunc)dm_set,			-1);
	rb_define_method(rb_cRect, "_dump",		(RbFunc)dm_dump,		1);
	rb_define_method(rb_cRect, "empty",		(RbFunc)dm_empty,		0);

	// object attribute
	rb_define_method(rb_cRect, "x",			(RbFunc)dm_get_x,		0);
	rb_define_method(rb_cRect, "x=",		(RbFunc)dm_set_x,		1);
	rb_define_method(rb_cRect, "y",			(RbFunc)dm_get_y,		0);
	rb_define_method(rb_cRect, "y=",		(RbFunc)dm_set_y,		1);
	rb_define_method(rb_cRect, "width",		(RbFunc)dm_get_width,	0);
	rb_define_method(rb_cRect, "width=",	(RbFunc)dm_set_width,	1);
	rb_define_method(rb_cRect, "height",	(RbFunc)dm_get_height,	0);
	rb_define_method(rb_cRect, "height=",	(RbFunc)dm_set_height,	1);

	// supplement
 	rb_define_method(rb_cRect,	"to_s",		(RbFunc)dm_to_string,	0);
	rb_define_method(rb_cRect,	"clone",	(RbFunc)dm_clone,		0);
}

VALUE RbRect::initialize(int argc, VALUE * argv, VALUE obj)
{
	if (argc)
	{
		rb_scan_args(argc, argv, "4", &vx, &vy, &vw, &vh);

		SafeNumericValue(vx);
		SafeNumericValue(vy);
		SafeNumericValue(vw);
		SafeNumericValue(vh);
	}
	else
	{
		vx = vy = vw = vh = RUBY_0;
	}

	x		= NUM2INT(vx);
	y		= NUM2INT(vy);
	width	= NUM2INT(vw);
	height	= NUM2INT(vh);
	
	vx		= INT2FIX(x);
	vy		= INT2FIX(y);
	vw		= INT2FIX(width);
	vh		= INT2FIX(height);

	return obj;
}

VALUE RbRect::get_x()
{
	return vx;
}

VALUE RbRect::set_x(VALUE nx)
{
	SafeNumericValue(nx);
	
	x	= NUM2INT(nx);
	vx	= INT2FIX(x);

	return nx;
}

VALUE RbRect::get_y()
{
	return vy;
}

VALUE RbRect::set_y(VALUE ny)
{
	SafeNumericValue(ny);
	
	y	= NUM2INT(ny);
	vy	= INT2FIX(y);

	return ny;
}

VALUE RbRect::get_width()
{
	return vw;
}

VALUE RbRect::set_width(VALUE nw)
{
	SafeNumericValue(nw);
	
	width	= NUM2INT(nw);
	vw		= INT2FIX(width);

	return nw;
}

VALUE RbRect::get_height()
{
	return vh;
}

VALUE RbRect::set_height(VALUE nh)
{
	SafeNumericValue(nh);
	
	height	= NUM2INT(nh);
	vh		= INT2FIX(height);

	return nh;
}

VALUE RbRect::set(int argc, VALUE * argv, VALUE obj)
{
	return initialize(argc, argv, obj);
}

VALUE RbRect::_dump(VALUE depth)
{
	return rb_str_new((const char *)&m_dump_data[0], sizeof(m_dump_data));
}

VALUE RbRect::empty()
{
	vx = vy = vw = vh = RUBY_0;
	x = y = width = height = 0;
	return __obj;
}

VALUE RbRect::clone()
{
	VALUE __argv[] = { vx, vy, vw, vh };

	return rb_class_new_instance(4, __argv, obj_class());
}

VALUE RbRect::to_string()
{
	return rb_sprintf("#<%s(%d, %d, %d, %d)>", obj_classname(), x, y, width, height);
}

VALUE RbRect::dm_load(VALUE klass, VALUE str)
{
	if (4 * sizeof(VALUE) != RSTRING_LEN(str))	//	error
		return Qnil;

	return rb_class_new_instance(4, (VALUE*)RSTRING_PTR(str), klass);
}

VALUE RbRect::dm_set(int argc, VALUE * argv, VALUE obj)
{
	RbRect * cobj;
	Data_Get_Struct(obj, RbRect, cobj);

	return cobj->set(argc, argv, obj);
}

imp_method(RbRect, empty)

imp_attr_accessor(RbRect, x)
imp_attr_accessor(RbRect, y)
imp_attr_accessor(RbRect, width)
imp_attr_accessor(RbRect, height)