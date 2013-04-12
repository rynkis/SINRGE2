#include "CRgeSprite.h"

#include "RgeSpriteEx.h"

#include "CRgeBitmap.h"
#include "CRgeViewport.h"
#include "CRgeColor.h"
#include "CRgeTone.h"
#include "CRgeRect.h"

VALUE rb_cSprite;

CRgeSprite::CRgeSprite()
	: m_mirror(Qfalse)
	, m_bush_depth(RUBY_0)
	, m_bush_opacity(128)
	, m_angle(0.0f)
	, m_angle_rad(RUBY_0)
	, m_flash_duration(0)
	, m_flash_reduce_count_per_frame(0)
	, m_flash_hide_spr(0)
	, m_flash_color(0)
	, m_src_rect_ptr(0)
{
}

CRgeSprite::~CRgeSprite()
{
}

void CRgeSprite::InitLibrary()
{
	/**
	 *	@classname
	 *		Sprite
	 *
	 *	@desc
	 *		RGE中处理精灵的类。所谓精灵，是为了在游戏画面上显示人物等的基本概念。
	 */
	rb_cSprite = rb_define_class_under(rb_mRGE2Core, "Sprite", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cSprite, ObjAllocate<CRgeSprite>);
	rb_define_method(rb_cSprite, "initialize",		(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cSprite, "dispose",			(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cSprite, "disposed?",		(RbFunc)dm_is_disposed,			0);
	rb_define_method(rb_cSprite, "flash",			(RbFunc)dm_flash,				2);
	rb_define_method(rb_cSprite, "update",			(RbFunc)dm_update,				0);

	rb_define_method(rb_cSprite, "invisible_reason",	(RbFunc)dm_invisible_reason,	0);

	// object attribute
	rb_define_method(rb_cSprite, "viewport",		(RbFunc)dm_get_viewport,		0);
	rb_define_method(rb_cSprite, "viewport=",		(RbFunc)dm_set_viewport,		1);

	rb_define_method(rb_cSprite, "visible",			(RbFunc)dm_get_visible,			0);
	rb_define_method(rb_cSprite, "visible=",		(RbFunc)dm_set_visible,			1);
	rb_define_method(rb_cSprite, "x",				(RbFunc)dm_get_x,				0);
	rb_define_method(rb_cSprite, "x=",				(RbFunc)dm_set_x,				1);
	rb_define_method(rb_cSprite, "y",				(RbFunc)dm_get_y,				0);
	rb_define_method(rb_cSprite, "y=",				(RbFunc)dm_set_y,				1);
	rb_define_method(rb_cSprite, "z",				(RbFunc)dm_get_z,				0);
	rb_define_method(rb_cSprite, "z=",				(RbFunc)dm_set_z,				1);
	rb_define_method(rb_cSprite, "ox",				(RbFunc)dm_get_ox,				0);
	rb_define_method(rb_cSprite, "ox=",				(RbFunc)dm_set_ox,				1);
	rb_define_method(rb_cSprite, "oy",				(RbFunc)dm_get_oy,				0);
	rb_define_method(rb_cSprite, "oy=",				(RbFunc)dm_set_oy,				1);

	rb_define_method(rb_cSprite, "zoom_x",			(RbFunc)dm_get_zoom_x,			0);
	rb_define_method(rb_cSprite, "zoom_x=",			(RbFunc)dm_set_zoom_x,			1);
	rb_define_method(rb_cSprite, "zoom_y",			(RbFunc)dm_get_zoom_y,			0);
	rb_define_method(rb_cSprite, "zoom_y=",			(RbFunc)dm_set_zoom_y,			1);

	rb_define_method(rb_cSprite, "opacity",			(RbFunc)dm_get_opacity,			0);
	rb_define_method(rb_cSprite, "opacity=",		(RbFunc)dm_set_opacity,			1);
	rb_define_method(rb_cSprite, "blend_type",		(RbFunc)dm_get_blend_type,		0);
	rb_define_method(rb_cSprite, "blend_type=",		(RbFunc)dm_set_blend_type,		1);
	rb_define_method(rb_cSprite, "color",			(RbFunc)dm_get_color,			0);
	rb_define_method(rb_cSprite, "color=",			(RbFunc)dm_set_color,			1);
	rb_define_method(rb_cSprite, "tone",			(RbFunc)dm_get_tone,			0);
	rb_define_method(rb_cSprite, "tone=",			(RbFunc)dm_set_tone,			1);

	rb_define_method(rb_cSprite, "bitmap",			(RbFunc)dm_get_bitmap,			0);
	rb_define_method(rb_cSprite, "bitmap=",			(RbFunc)dm_set_bitmap,			1);
	rb_define_method(rb_cSprite, "src_rect",		(RbFunc)dm_get_src_rect,		0);
	rb_define_method(rb_cSprite, "src_rect=",		(RbFunc)dm_set_src_rect,		1);
	rb_define_method(rb_cSprite, "angle",			(RbFunc)dm_get_angle,			0);
	rb_define_method(rb_cSprite, "angle=",			(RbFunc)dm_set_angle,			1);
	rb_define_method(rb_cSprite, "mirror",			(RbFunc)dm_get_mirror,			0);
	rb_define_method(rb_cSprite, "mirror=",			(RbFunc)dm_set_mirror,			1);
	rb_define_method(rb_cSprite, "bush_depth",		(RbFunc)dm_get_bush_depth,		0);
	rb_define_method(rb_cSprite, "bush_depth=",		(RbFunc)dm_set_bush_depth,		1);
	rb_define_method(rb_cSprite, "bush_opacity",	(RbFunc)dm_get_bush_opacity,	0);
	rb_define_method(rb_cSprite, "bush_opacity=",	(RbFunc)dm_set_bush_opacity,	1);

	rb_define_method(rb_cSprite, "wave_amp",		(RbFunc)dm_get_wave_amp,		0);
	rb_define_method(rb_cSprite, "wave_amp=",		(RbFunc)dm_set_wave_amp,		1);
	rb_define_method(rb_cSprite, "wave_length",		(RbFunc)dm_get_wave_length,		0);
	rb_define_method(rb_cSprite, "wave_length=",	(RbFunc)dm_set_wave_length,		1);
	rb_define_method(rb_cSprite, "wave_speed",		(RbFunc)dm_get_wave_speed,		0);
	rb_define_method(rb_cSprite, "wave_speed=",		(RbFunc)dm_set_wave_speed,		1);
	rb_define_method(rb_cSprite, "wave_phase",		(RbFunc)dm_get_wave_phase,		0);
	rb_define_method(rb_cSprite, "wave_phase=",		(RbFunc)dm_set_wave_phase,		1);

	// supplement
 	rb_define_method(rb_cSprite, "to_s",			(RbFunc)dm_to_string,			0);
}

void CRgeSprite::mark()
{
	super::mark();

	if (m_src_rect_ptr)	m_src_rect_ptr->MarkObject();
}

/**
 *	@call
 *		Sprite.new				-> sprite 对象 。
 *		Sprite.new(viewport)	-> sprite 对象。
 *
 *	@desc
 *		生成 Sprite 对象。
 */
VALUE CRgeSprite::initialize(int argc, VALUE *argv, VALUE obj)
{
	//	调用Plane的初始化函数
	super::initialize(argc, argv, obj);

	//	创建src_rect对象
	VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};

	VALUE src_rect = rb_class_new_instance(4, __argv, rb_cRect);

	m_src_rect_ptr = GetObjectPtr<CRgeRect>(src_rect);

	return obj;
}

/**
 *	更新纹理矩形以及相关坐标位置
 *
 *	其中 mirror、angle、zoom_x、zoom_y、ox、oy、x、y、src_rect、bitmap 等属性的改变都应该调用该方法进行更新 (bush_depth?)
 */
//bool CRgeSprite::update_texture_rect_and_position()
//{
//	s32	left	= m_src_rect_ptr->x;	
//	s32 right	= m_src_rect_ptr->x + m_src_rect_ptr->width;
//	s32 top		= m_src_rect_ptr->y;	
//	s32 bottom	= m_src_rect_ptr->y + m_src_rect_ptr->height;
//
//	if (m_src_rect_ptr->width <= 0 || m_src_rect_ptr->height <= 0)
//		return false;
//
//	if (right < 0 || left > m_bitmap_ptr->GetBmpWidth() || bottom < 0 || top > m_bitmap_ptr->GetBmpHeight())
//		return false;
//
//	if (left < 0)								left	= 0;
//	if (right > m_bitmap_ptr->GetBmpWidth())	right	= m_bitmap_ptr->GetBmpWidth();
//	if (top < 0)								top		= 0;
//	if (bottom > m_bitmap_ptr->GetBmpHeight())	bottom	= m_bitmap_ptr->GetBmpHeight();
//
//	s32 x		= left;
//	s32 y		= top;
//	s32 width	= right - left;
//	s32 height	= bottom - top;
//
//	int xBgn	= x / m_bitmap_ptr->GetSubTextures()[0].width;
//	int yBgn	= y / m_bitmap_ptr->GetSubTextures()[0].height;
//
//	int xEnd	= (x + width - 1) / m_bitmap_ptr->GetSubTextures()[0].width;
//	int yEnd	= (y + height - 1) / m_bitmap_ptr->GetSubTextures()[0].height;
//
//	for (u32 idx = 0; idx < m_uSpriteCnt; ++idx)
//		m_pSpriteArrs[idx]->SetRenderFlag(false);
//
//	//	translation、scaling、rotation
//	float fcos = cosf(m_angle);
//	float fsin = sinf(m_angle);
//
//#define __TRANSLATION_SCALING_X(mx)	(((mx) - m_ox) * m_zoom_x)
//#define __TRANSLATION_SCALING_Y(my)	(((my) - m_oy) * m_zoom_y)
//#define __ROTATION_X(__x, __y)		((float)(((__x) * fcos - (__y) * fsin + m_x)))
//#define __ROTATION_Y(__x, __y)		((float)(((__x) * fsin + (__y) * fcos + m_y)))
//
//	int	index;
//
//	float tmp_x1, tmp_x2, tmp_y1, tmp_y2;
//	float fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
//
//	for (int yidx = yBgn, cost_h = 0; yidx <= yEnd; ++yidx)
//	{
//		int h = RgeMin(RgeMin(height - cost_h, m_bitmap_ptr->GetSubTextures()[0].height), m_bitmap_ptr->GetSubTextures()[0].height - y);
//
//		for (int xidx = xBgn, cost_w = 0, sx = x % m_bitmap_ptr->GetSubTextures()[0].width; xidx <= xEnd; ++xidx)
//		{
//			int w = RgeMin(RgeMin(width - cost_w, m_bitmap_ptr->GetSubTextures()[0].width), m_bitmap_ptr->GetSubTextures()[0].width - sx);
//
//			index = yidx * m_bitmap_ptr->GetCols() + xidx;
//
//			m_pSpriteArrs[index]->SetTextureRect(sx, y, w, h, true);
//
//			tmp_x1	= __TRANSLATION_SCALING_X(cost_w);
//			tmp_y1	= __TRANSLATION_SCALING_Y(cost_h);
//			tmp_x2	= __TRANSLATION_SCALING_X(cost_w + w);
//			tmp_y2	= __TRANSLATION_SCALING_Y(cost_h + h);
//
//			fx1	= __ROTATION_X(tmp_x1, tmp_y1);
//			fy1 = __ROTATION_Y(tmp_x1, tmp_y1);
//			fx2 = __ROTATION_X(tmp_x2, tmp_y1);
//			fy2 = __ROTATION_Y(tmp_x2, tmp_y1);
//			fx3 = __ROTATION_X(tmp_x2, tmp_y2);
//			fy3 = __ROTATION_Y(tmp_x2, tmp_y2);
//			fx4 = __ROTATION_X(tmp_x1, tmp_y2);
//			fy4 = __ROTATION_Y(tmp_x1, tmp_y2);
//
//			if (xidx != xBgn)
//			{
//				m_pSpriteArrs[yidx * m_bitmap_ptr->GetCols() + xidx - 1]->GetV23(&fx1, &fy1, &fx4, &fy4);
//			}
//
//			if (yidx != yBgn)
//			{
//				m_pSpriteArrs[(yidx - 1) * m_bitmap_ptr->GetCols() + xidx]->GetV34(&fx2, &fy2, &fx1, &fy1);
//			}
//
//			m_pSpriteArrs[index]->Set4V(fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4);
//
//			m_pSpriteArrs[index]->SetRenderFlag(true);
//
//			cost_w += w;
//			sx = 0;
//		}
//
//		cost_h += h;
//		y = 0;
//	}
//
//#undef __ROTATION_Y
//#undef __ROTATION_X
//#undef __TRANSLATION_SCALING_Y
//#undef __TRANSLATION_SCALING_X
//
//	return true;
//}

void CRgeSprite::render(u32 id)
{
	if (!m_bitmap_ptr)
		return;

	//	精灵对应的显示位图已经释放则直接返回
	if (m_bitmap_ptr->IsDisposed())
		return;

	//	如果精灵处理闪烁中且闪烁颜色为nil则直接返回
	if (m_flash_duration > 0 && m_flash_hide_spr > 0)
		return;

	////	如果缩放比例小于0则直接返回
	//if (m_zoom_x <= 0.0f || m_zoom_y <= 0.0f)
	//	return;

	//	处理色调
	process_tone_texture();

	//	设置源矩形
	m_pSpr->SetTextureRect(m_src_rect_ptr->x, m_src_rect_ptr->y, m_src_rect_ptr->width, m_src_rect_ptr->height);


	//	关于精灵混合颜色的管理：

	//	如果指定了flash颜色的情况下：
	//		如果也指定了color颜色的情况下：
	//			如果color颜色的不透明度大于flash颜色的不透明度，则以color颜色混合。否则以flash颜色混合。
	//		如果没有指定color颜色的情况下：
	//			直接以flash颜色混合。
	//	如果没指定flash颜色的情况下：
	//		如果指定了color颜色则以color颜色混合，否则不混合。

	int		save_blend_mode = -1;
	DWORD	save_blend_color = 0;

	//	如果指定了flash颜色的情况下
	if (m_flash_duration > 0 && m_flash_color != 0)
	{
		save_blend_mode = m_pSpr->GetBlendMode();
		save_blend_color = m_flash_color;
	}

	//	如果指定了color颜色的情况下
	if (m_color_ptr->GetColor().value != 0)
	{
		//	并且设置了flash颜色的情况下
		if (save_blend_color > 0)
		{
			if (GETA(m_color_ptr->GetColor().value) > GETA(m_flash_color))
			{
				save_blend_color = m_color_ptr->GetColor().value;
			}
		}
		//	没有设置flash颜色的情况下
		else
		{
			save_blend_mode = m_pSpr->GetBlendMode();
			save_blend_color = m_color_ptr->GetColor().value;
		}
	}

	//	混合颜色的处理
	if (save_blend_mode > 0)
	{
		m_pSpr->SetBlendMode(save_blend_mode | BLEND_COLORBLNED);
		m_pSpr->SetBlendColor(save_blend_color);
	}

	// set the sprite's opacity
	m_pSpr->SetColor(ARGB(m_opacity, 255, 255, 255));

	// render the sprite to the screen
	float x, y;

	if (!m_viewport_ptr || GetRgeSysPtr()->GetRenderState()->IsRenderToTexture())
	{
		x = m_x;
		y = m_y;
	}
	else
	{
		const CRgeRect* rect_ptr = m_viewport_ptr->GetRectPtr();

		x = m_x + rect_ptr->x - m_viewport_ptr->m_ox;
		y = m_y + rect_ptr->y - m_viewport_ptr->m_oy;
	}

	//	草丛深度不为0并且草丛不透明度不为255的情况下才描绘草丛透明度
	if (m_bush_depth != RUBY_0 && m_bush_opacity != 255)
	{
		hgeRect bound_rect;

		m_pSpr->GetBoundingBoxEx(x, y, &bound_rect);

		int bush_depth = FIX2INT(m_bush_depth);

		hgeRect top_rect(bound_rect.x1, bound_rect.y1, bound_rect.x2, bound_rect.y2 - bush_depth);
		hgeRect bottom_rect(bound_rect.x1, bound_rect.y2 - bush_depth, bound_rect.x2, bound_rect.y2);

		int cw, ch;

		int min_x, min_y, max_x, max_y;

		const RgeRenderState::RgeRenderClipRect& clip_rect = GetRgeSysPtr()->GetRenderState()->GetClipRect();

		//	先描绘不透明部分
		min_x = RgeMax(clip_rect.x, top_rect.x1);
		min_y = RgeMax(clip_rect.y, top_rect.y1);

		max_x = RgeMin(clip_rect.x + clip_rect.width, top_rect.x2);
		max_y = RgeMin(clip_rect.y + clip_rect.height, top_rect.y2);

		cw = max_x - min_x;
		ch = max_y - min_y;

		if (cw > 0 && ch > 0)
		{
			GetRgeSysPtr()->GetRenderState()->Clip(min_x, min_y, cw, ch);
			m_pSpr->Render(x, y);
		//	hge->Gfx_SetClipping(min_x, min_y, cw, ch);
		//	self->spr->RenderEx(x, y, self->angle, self->zoom_x, self->zoom_y);
		}

		//	其次描绘透明草丛部分
		// half opacity : simulate bush effect
		m_pSpr->SetColor(URgeARGB(m_opacity * m_bush_opacity / 255, 255, 255, 255).value);
		//m_pSpr->SetColor(ARGB(m_opacity / 2,255,255,255));

		min_x = RgeMax(clip_rect.x, bottom_rect.x1);
		min_y = RgeMax(clip_rect.y, bottom_rect.y1);

		max_x = RgeMin(clip_rect.x + clip_rect.width,	bottom_rect.x2);
		max_y = RgeMin(clip_rect.y + clip_rect.height,	bottom_rect.y2);

		cw = max_x - min_x;
		ch = max_y - min_y;

		if (cw > 0 && ch > 0)
		{
			GetRgeSysPtr()->GetRenderState()->Clip(min_x, min_y, cw, ch);
			m_pSpr->Render(x, y);
			//hge->Gfx_SetClipping(min_x, min_y, cw, ch);
			//self->spr->RenderEx(x,y,self->angle,self->zoom_x,self->zoom_y);
		}

		GetRgeSysPtr()->GetRenderState()->Restore();
		m_pSpr->SetColor(URgeARGB(m_opacity, 255, 255, 255).value);
		//// restore the clip region
		//hge->Gfx_SetClipping(rge->last_clip_rect.x,
		//					 rge->last_clip_rect.y,
		//					 rge->last_clip_rect.w,
		//					 rge->last_clip_rect.h);

		//// restore the opacity
		//self->spr->SetColor(ARGB(self->opacity,255,255,255));

	} 
	else
	{
		m_pSpr->Render(x, y);
	}

	//	如果混合了颜色的情况下则执行恢复处理
	if (save_blend_mode > 0)
	{
		m_pSpr->SetBlendMode(save_blend_mode);
		m_pSpr->SetBlendColor(0);

		save_blend_mode = -1;
	}
}

VALUE CRgeSprite::update()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::flash(VALUE color, VALUE duration)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::invisible_reason()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::set_bitmap(VALUE bitmap)
{
	if (m_disposed) 
		return Qnil;

	// 设置位图修改计数值为-1
	m_ref_bitmap_modify_count = -1;

	if (NIL_P(bitmap))
	{
		m_bitmap_ptr = 0;

		rb_funcall(m_src_rect_ptr->GetObject(), rb_intern("set"), 4, RUBY_0, RUBY_0, RUBY_0, RUBY_0);
	}
	else
	{
		SafeBitmapValue(bitmap);

		m_bitmap_ptr = GetObjectPtr<CRgeBitmap>(bitmap);

		m_pSpr->SetTexture(m_bitmap_ptr->GetSRgeTexture());
		m_pSpr->SetSrcRectDirty();
		
		VALUE __argv[] = { RUBY_0, RUBY_0, INT2FIX(m_bitmap_ptr->GetBmpWidth()), INT2FIX(m_bitmap_ptr->GetBmpHeight()) };

		rb_funcall2(m_src_rect_ptr->GetObject(), rb_intern("set"), 4, __argv);
	}

	return Qnil;
}

VALUE CRgeSprite::set_ox(VALUE ox)
{
	super::set_ox(ox);

	m_pSpr->SetOX(m_ox);
	m_pSpr->SetSrcRectDirty();

	return Qnil;
}

VALUE CRgeSprite::set_oy(VALUE oy)
{
	super::set_oy(oy);

	m_pSpr->SetOX(m_oy);
	m_pSpr->SetSrcRectDirty();

	return Qnil;
}

VALUE CRgeSprite::get_src_rect()
{
	return ReturnObject(m_src_rect_ptr);
}

VALUE CRgeSprite::set_src_rect(VALUE src_rect)
{
	SafeRectValue(src_rect);
	m_src_rect_ptr = GetObjectPtr<CRgeRect>(src_rect);
	return Qnil;
}

VALUE CRgeSprite::get_angle()
{
	return m_angle_rad;
}

VALUE CRgeSprite::set_angle(VALUE angle)
{
	SafeFixnumValue(angle);

	m_angle_rad = angle;

	m_angle = RgeDeg2Rad(360 - FIX2INT(angle));
	m_pSpr->SetAngle(m_angle);
	m_pSpr->SetSrcRectDirty();

	return Qnil;
}

VALUE CRgeSprite::get_mirror()
{
	return m_mirror;
}

VALUE CRgeSprite::set_mirror(VALUE mirror)
{
	m_mirror = Ruby2RbBool(mirror);

	m_pSpr->SetFlip(RTEST(m_mirror), m_pSpr->IsFlipY(), true);
	//m_pSpr->SetSrcRectDirty();	//	SetFlip会自动设置藏标记

	return Qnil;
}

VALUE CRgeSprite::get_bush_depth()
{
	return m_bush_depth;
}

VALUE CRgeSprite::set_bush_depth(VALUE bush_depth)
{
	SafeFixnumValue(bush_depth);

	m_bush_depth = bush_depth;

	return Qnil;
}

VALUE CRgeSprite::get_bush_opacity()
{
	return INT2FIX(m_bush_opacity);
}

VALUE CRgeSprite::set_bush_opacity(VALUE bush_opacity)
{
	SafeFixnumValue(bush_opacity);

	m_bush_opacity = FIX2INT(bush_opacity);
	m_bush_opacity = RgeBound(m_bush_opacity, 0, 255);

	return Qnil;
}

VALUE CRgeSprite::get_wave_amp()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::set_wave_amp(VALUE wave_amp)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::get_wave_length()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::set_wave_length(VALUE wave_length)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::get_wave_speed()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::set_wave_speed(VALUE wave_speed)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::get_wave_phase()
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

VALUE CRgeSprite::set_wave_phase(VALUE wave_phase)
{
#pragma message("		<<Rge2Core:innerclass>> 未完成 " __FUNCTION__)

	return Qnil;
}

/*
 *	以下定义ruby方法
 */
imp_method(CRgeSprite, update)
imp_method02(CRgeSprite, flash)
imp_attr_accessor(CRgeSprite, src_rect)
imp_attr_accessor(CRgeSprite, angle)
imp_attr_accessor(CRgeSprite, mirror)
imp_attr_accessor(CRgeSprite, bush_depth)
imp_attr_accessor(CRgeSprite, bush_opacity)

imp_attr_accessor(CRgeSprite, wave_amp)
imp_attr_accessor(CRgeSprite, wave_length)
imp_attr_accessor(CRgeSprite, wave_speed)
imp_attr_accessor(CRgeSprite, wave_phase)