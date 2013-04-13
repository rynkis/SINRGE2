#ifndef __C_RGE_PLANE_H__
#define __C_RGE_PLANE_H__

#include "RbDisplayObject.h"
#include "hge.h"

class SinSprite;

class RbBitmap;
class RbColor;
class RbTone;

class RbPlane : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	RbPlane();
	virtual ~RbPlane();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE *argv, VALUE obj);

	virtual	void	render(u32 id);

	virtual VALUE	invisible_reason();

	virtual VALUE	set_z(VALUE z);
	virtual VALUE	set_viewport(VALUE viewport);

protected:
	//inline bool		use_linear_render_mode() const;
	void			process_tone_texture();

protected:
	RbRenderNode*	m_node;

	u32				m_opacity;
	VALUE			m_blend_type;	//	（0：正常，1：加法，2：减法）。 ！Ruby层面的0、1、2。

	// 保存位图修改计数值和先前色调值
	s32				m_ref_bitmap_modify_count;
	DWORD			m_ref_tone;
	HTEXTURE		m_tone_tex;

	SinSprite*		m_pSpr;

	RbBitmap*		m_bitmap_ptr;
	RbColor*		m_color_ptr;
	RbTone*			m_tone_ptr;

protected:
	attr_accessor(bitmap)
	attr_accessor(zoom_x)
	attr_accessor(zoom_y)
	attr_accessor(opacity)
	attr_accessor(blend_type)
	attr_accessor(color)
	attr_accessor(tone)
};

#endif