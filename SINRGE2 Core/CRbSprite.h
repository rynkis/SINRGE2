/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Sprite
*/
#ifndef __C_RB_SPRITE_H__
#define __C_RB_SPRITE_H__

#include "CRbPlane.h"

class CRbRect;

class CRbSprite : public CRbPlane
{
public:
	typedef CRbPlane	super;

public:
	CRbSprite();
	virtual ~CRbSprite();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);

	virtual	void	render(u32 id);

	virtual VALUE	set_ox(VALUE ox);
	virtual VALUE	set_oy(VALUE oy);

	virtual	VALUE	set_bitmap(VALUE bmp);
	
protected:
	void			check_raise();

protected:
	VALUE			m_mirror;
	VALUE			m_bush_depth;
	int				m_bush_opacity;
	VALUE			m_angle_rad;
	float			m_angle;
	
	int				m_flash_duration;					//	精灵闪烁帧数
	int				m_flash_reduce_count_per_frame;		//	精灵闪烁每帧降低的透明度
	int				m_flash_hide_spr;					//	是否消去精灵，仅当闪烁颜色指定为nil时才消去精灵。
	DWORD			m_flash_color;						//	精灵闪烁的颜色值。
	
	bool			m_disposed;
	bool			m_movie_playing;

	CRbRect *		m_src_rect_ptr;
	CRbTone *		m_tone_ptr;

	//hgeQuad			m_quad;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	dm_method_vargs(play_movie)
	dm_method(is_playing)
	dm_method(get_volume)
	dm_method01(set_volume)
	dm_method(replay_at_finish)
	dm_method(stop_movie)
	dm_method(rewind_movie)
	
	dm_method(update)
	dm_method(get_width)
	dm_method(get_height)
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