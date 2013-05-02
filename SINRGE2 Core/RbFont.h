/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Font
*/
#ifndef __RB_FONT_H__
#define __RB_FONT_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"

class RbColor;

class RbFont : public RbClassBase
{
public:
	RbFont();
	virtual ~RbFont();

public:
	static void		InitLibrary();

	RbColor *		GetColorPtr() const { return m_color_ptr; }
	HFONT			GetHFont() const { return m_hFont; }
	bool			IsShadow() const { return RTEST(m_shadow); }
	static bool		IsExist(const wchar_t * filename);
	static VALUE	dm_is_exist(int argc, VALUE name);

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);

protected:
	VALUE			m_name;			// mark
	VALUE			m_size;
	VALUE			m_bold;
	VALUE			m_italic;
	VALUE			m_shadow;

	// 实现相关属性
	HFONT			m_hFont;
	LOGFONTW		m_lfw;

	RbColor *		m_color_ptr;

protected:

	attr_accessor(name)
	attr_accessor(size)
	attr_accessor(bold)
	attr_accessor(italic)
	attr_accessor(color)
	attr_accessor(shadow)

	class_attr_accessor(default_name)
	class_attr_accessor(default_size)
	class_attr_accessor(default_bold)
	class_attr_accessor(default_italic)
	class_attr_accessor(default_color)
	class_attr_accessor(default_shadow)

protected:
	static VALUE	__default_name__;
	static VALUE	__default_size__;
	static VALUE	__default_bold__;
	static VALUE	__default_italic__;
	static VALUE	__default_color__;
	static VALUE	__default_shadow__;
};

#endif