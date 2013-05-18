/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class LFont
*/
#ifndef __CLASS_RB_LFONT_H__
#define __CLASS_RB_LFONT_H__

#ifdef WIN32
#pragma once
#endif

#define MAX_SIZE  65536

#include "RbClassBase.h"
#include "sin_bitmap.h"
#include "sin_types.h"

class RbColor;

class RbLFont : public RbClassBase
{
public:
	RbLFont();
	virtual ~RbLFont();

public:
	static void				InitLibrary();

	RbColor *				GetColorPtr() const { return m_color_ptr; }
	//bool					IsShadow() const { return RTEST(m_shadow); }

protected:
	virtual void			mark();
	virtual VALUE			initialize(int argc, VALUE * argv, VALUE obj);

protected:
	void					check_raise();
	
protected:
	VALUE					m_filename;			// mark

	int						m_size;
	int						m_length;

	/*VALUE					m_bold;
	VALUE					m_italic;*/
	VALUE					m_shadow;

	RbColor *				m_color_ptr;

	bool					m_closed;

	BYTE *					m_font_data;

protected:
	dm_method(close)
	dm_method(is_closed)
	dm_method(get_size)
	
	dm_method01(get_font_bmp)
	dm_method01(text_size)
	//dm_method_vargs(draw_text)

	/*attr_accessor(bold)
	attr_accessor(italic)*/
	attr_accessor(color)
	attr_accessor(shadow)

private:
	friend class	RbLFont;
};

#endif // __CLASS_RB_LFONT_H__