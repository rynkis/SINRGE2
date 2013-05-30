/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class Charmap
*/
#ifndef __C_RB_CHARMAP_H__
#define __C_RB_CHARMAP_H__

#include "CRbClassBase.h"

class SinSprite;

class CRbBitmap;

class CRbCharmap : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	CRbCharmap();													//	allocate
	virtual ~CRbCharmap();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize
	
	virtual	void	render(u32 id);
	
	virtual VALUE	get_z();
	virtual VALUE	set_z(VALUE z);

	//virtual VALUE	set_viewport(VALUE viewport);

protected:
	void			check_raise();
	void			get_charmap();

protected:
	RbRenderNode *	m_node;
	
	SinSprite *		m_pSpr;

	CRbBitmap *		m_bitmap_ptr;

	long			m_width, m_height, m_charsize, m_charmap_size;

	char *			charmap;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	dm_method01(reset)
	
	attr_accessor(bitmap)
};

#endif // __C_RB_CHARMAP_H__