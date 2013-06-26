/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Font
*/
#ifndef __C_RB_FONT_H__
#define __C_RB_FONT_H__

#ifdef WIN32
#pragma once
#endif

#define MAX_SIZE  65536

#include "CRbClassBase.h"

class CRbColor;

class CRbFont : public CRbClassBase
{
public:
	CRbFont();
	virtual ~CRbFont();

public:
	static void				InitLibrary();

	static bool				IsExist(const wchar_t * filename);
	static VALUE			dm_is_exist(int argc, VALUE name);

	CRbColor *				GetColorPtr() const { return m_color_ptr; }
	HFONT					GetHFont() const { return m_hFont; }
	bool					IsShadow() const { return RTEST(m_shadow); }
	bool					IsLattice() const { return m_font_data != 0; }
	int						GetTextWidth(const wchar_t * text);

protected:
	virtual void			mark();
	virtual VALUE			initialize(int argc, VALUE * argv, VALUE obj);
	virtual VALUE			clone();
	
protected:
	void					check_raise();

protected:
	VALUE					m_name;			// mark
	VALUE					m_size;
	VALUE					m_bold;
	VALUE					m_italic;
	VALUE					m_shadow;

	// 实现相关属性
	HFONT					m_hFont;
	LOGFONTW				m_lfw;

	CRbColor *				m_color_ptr;
	
	u32						m_count;
	bool					m_disposed;
	u8 *					m_font_data;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	attr_accessor(name)
	attr_accessor(size)
	attr_accessor(bold)
	attr_accessor(italic)
	attr_accessor(color)
	attr_accessor(shadow)

private:
	friend class	CRbFont;
	friend class	CRbBitmap;
};

#endif