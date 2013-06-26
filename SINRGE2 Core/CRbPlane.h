/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Plane
*/
#ifndef __C_RB_PLANE_H__
#define __C_RB_PLANE_H__

#include "CRbDisplayObject.h"

class SinSprite;

class CRbBitmap;
class CRbColor;
class CRbTone;

class CRbPlane : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	CRbPlane();
	virtual ~CRbPlane();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);

	virtual	void	render(u32 id);
	
	virtual VALUE	get_z();
	virtual VALUE	set_z(VALUE z);

	virtual VALUE	set_viewport(VALUE viewport);

protected:
	void			check_raise();
	void			process_tone_texture();

protected:
	RbRenderNode *	m_node;

	int				m_opacity;
	int				m_blend_type;
	int				m_save_blend_type;

	// 保存位图修改计数值和先前色调值
	s32				m_ref_bitmap_modify_count;
	u32				m_ref_tone;
	HTEXTURE		m_tone_tex;
	int				m_tone_tex_width;
	int				m_tone_tex_height;

	SinSprite *		m_pSpr;

	CRbBitmap *		m_bitmap_ptr;
	CRbColor *		m_color_ptr;
	CRbTone *		m_tone_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	attr_accessor(bitmap)
	attr_accessor(zoom_x)
	attr_accessor(zoom_y)
	attr_accessor(opacity)
	attr_accessor(blend_type)
	attr_accessor(color)
	attr_accessor(tone)
};

#endif