/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class LFont
*/
#ifndef __C_RB_LFONT_H__
#define __C_RB_LFONT_H__

#ifdef WIN32
#pragma once
#endif

#define MAX_SIZE  65536

#include "CRbClassBase.h"
#include "sin_bitmap.h"
#include "sin_types.h"

class CRbColor;

class CRbLFont : public CRbClassBase
{
public:
	CRbLFont();
	virtual ~CRbLFont();

public:
	static void				InitLibrary();

	CRbColor *				GetColorPtr() const { return m_color_ptr; }
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

	CRbColor *				m_color_ptr;

	bool					m_disposed;

	BYTE *					m_font_data;

protected:
	dm_method(dispose)
	dm_method(is_disposed)
	dm_method(get_size)
	
	dm_method01(get_char_bmp)
	dm_method01(char_width)
	//dm_method_vargs(draw_text)

	/*attr_accessor(bold)
	attr_accessor(italic)*/
	attr_accessor(color)
	//attr_accessor(shadow)

private:
	friend class	CRbLFont;
};

#endif // __C_RB_LFONT_H__