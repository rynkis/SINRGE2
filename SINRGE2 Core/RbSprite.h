#ifndef __RB_SPRITE_H__
#define __RB_SPRITE_H__

#include "RbPlane.h"

class RbRect;

class RbSprite : public RbPlane
{
public:
	typedef RbPlane	super;

public:
	RbSprite();
	virtual ~RbSprite();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE *argv, VALUE obj);

	virtual	void	render(u32 id);

	virtual VALUE	invisible_reason();

	virtual VALUE	set_ox(VALUE ox);
	virtual VALUE	set_oy(VALUE oy);

	virtual	VALUE	set_bitmap(VALUE bmp);

protected:
	VALUE			m_mirror;
	VALUE			m_bush_depth;
	u32				m_bush_opacity;
	VALUE			m_angle_rad;
	float			m_angle;

	int				m_flash_duration;					//	精灵闪烁帧数
	int				m_flash_reduce_count_per_frame;		//	精灵闪烁每帧降低的透明度
	int				m_flash_hide_spr;					//	是否消去精灵，仅当闪烁颜色指定为nil时才消去精灵。
	DWORD			m_flash_color;						//	精灵闪烁的颜色值。

	RbRect*			m_src_rect_ptr;

protected:
	dm_method(update)
	dm_method02(flash)
	attr_accessor(src_rect)
	attr_accessor(angle)
	attr_accessor(mirror)
	attr_accessor(bush_depth)
	attr_accessor(bush_opacity)

	attr_accessor(wave_amp)
	attr_accessor(wave_length)
	attr_accessor(wave_speed)
	attr_accessor(wave_phase)
};

#endif