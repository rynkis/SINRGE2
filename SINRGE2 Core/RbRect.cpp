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
}

VALUE RbRect::initialize(int argc, VALUE *argv, VALUE obj)
{
	rb_scan_args(argc, argv, "4", &vx, &vy, &vw, &vh);

	SafeFixnumValue(vx);
	SafeFixnumValue(vy);
	SafeFixnumValue(vw);
	SafeFixnumValue(vh);

	x		= FIX2INT(vx);
	y		= FIX2INT(vy);
	width	= FIX2INT(vw);
	height	= FIX2INT(vh);

	return obj;
}

VALUE RbRect::get_x()
{
	return vx;
}

VALUE RbRect::set_x(VALUE nx)
{
	SafeFixnumValue(nx);

	vx	= nx;
	x	= FIX2INT(nx);

	return Qnil;
}

VALUE RbRect::get_y()
{
	return vy;
}

VALUE RbRect::set_y(VALUE ny)
{
	SafeFixnumValue(ny);

	vy	= ny;
	y	= FIX2INT(ny);

	return Qnil;
}

VALUE RbRect::get_width()
{
	return vw;
}

VALUE RbRect::set_width(VALUE nw)
{
	SafeFixnumValue(nw);

	vw		= nw;
	width	= FIX2INT(nw);

	return Qnil;
}

VALUE RbRect::get_height()
{
	return vh;
}

VALUE RbRect::set_height(VALUE nh)
{
	SafeFixnumValue(nh);

	vh		= nh;
	height	= FIX2INT(nh);

	return Qnil;
}

VALUE RbRect::set(int argc, VALUE *argv, VALUE obj)
{
	return initialize(argc, argv, obj);
}

VALUE RbRect::_dump(VALUE depth)
{
	return rb_str_new((const char*)&m_dump_data[0], sizeof(m_dump_data));
}

VALUE RbRect::empty()
{
	//VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};

	//return initialize(4, __argv, __obj);

	vx = vy = vw = vh = RUBY_0;

	x		= FIX2INT(vx);
	y		= FIX2INT(vy);
	width	= FIX2INT(vw);
	height	= FIX2INT(vh);

	return __obj;
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

VALUE RbRect::dm_set(int argc, VALUE *argv, VALUE obj)
{
	RbRect	*cobj;
	Data_Get_Struct(obj, RbRect, cobj);

	return cobj->set(argc, argv, obj);
}

imp_method(RbRect, empty)

imp_attr_accessor(RbRect, x)
imp_attr_accessor(RbRect, y)
imp_attr_accessor(RbRect, width)
imp_attr_accessor(RbRect, height)