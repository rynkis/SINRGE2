/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Sprite
*/
#include "CRbSprite.h"
#include "CRbBitmap.h"
#include "CRbViewport.h"
#include "CRbColor.h"
#include "CRbTone.h"
#include "CRbRect.h"
#include "sin_app.h"
#include "sin_color.h"
#include "sin_video.h"
#include "sin_sprite.h"

VALUE rb_cSprite;

CRbSprite::CRbSprite()
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
	, m_movie_playing(false)
{
}

CRbSprite::~CRbSprite()
{
	super::~CRbPlane();
}

void CRbSprite::InitLibrary()
{
	rb_cSprite = rb_define_class_under(rb_mSin, "Sprite", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cSprite, ObjAllocate<CRbSprite>);
	rb_define_method(rb_cSprite, "initialize",		(RbFunc)dm_initialize,			-1);

	// instance method
	rb_define_method(rb_cSprite, "dispose",			(RbFunc)dm_dispose,				0);
	rb_define_method(rb_cSprite, "disposed?",		(RbFunc)dm_is_disposed,			0);
	rb_define_method(rb_cSprite, "flash",			(RbFunc)dm_flash,				2);
	rb_define_method(rb_cSprite, "update",			(RbFunc)dm_update,				0);
	rb_define_method(rb_cSprite, "width",			(RbFunc)dm_get_width,			0);
	rb_define_method(rb_cSprite, "height",			(RbFunc)dm_get_height,			0);

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
	
	rb_define_method(rb_cSprite, "play_movie",		(RbFunc)dm_play_movie,			-1);
	rb_define_method(rb_cSprite, "playing?",		(RbFunc)dm_is_playing,			0);
	rb_define_method(rb_cSprite, "volume",			(RbFunc)dm_get_volume,			0);
	rb_define_method(rb_cSprite, "volume=",			(RbFunc)dm_set_volume,			1);
	rb_define_method(rb_cSprite, "replay_at_finish",(RbFunc)dm_replay_at_finish,	0);
	rb_define_method(rb_cSprite, "stop_movie",		(RbFunc)dm_stop_movie,			0);
	rb_define_method(rb_cSprite, "rewind_movie",	(RbFunc)dm_rewind_movie,		0);

	// supplement
 	rb_define_method(rb_cSprite, "to_s",			(RbFunc)dm_to_string,			0);
}

void CRbSprite::mark()
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
VALUE CRbSprite::initialize(int argc, VALUE * argv, VALUE obj)
{
	//	调用Plane的初始化函数
	super::initialize(argc, argv, obj);

	//	创建src_rect对象
	VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};

	VALUE src_rect = rb_class_new_instance(4, __argv, rb_cRect);

	m_src_rect_ptr = GetObjectPtr<CRbRect>(src_rect);

	m_disposed = false;
	return obj;
}

void CRbSprite::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed sprite");
}

VALUE CRbSprite::dispose()
{
	if (m_disposed)
		return Qnil;

	if (m_movie_playing)
		GetVideoMgr()->StopMovie();
	m_bitmap_ptr = 0;

	m_disposed = true;

	return Qnil;
}

VALUE CRbSprite::is_disposed()
{
	return C2RbBool(m_disposed);
}

void CRbSprite::render(u32 id)
{
	if (!m_bitmap_ptr)
		return;

	//	精灵对应的显示位图已经释放则直接返回
	if (m_bitmap_ptr->IsDisposed())
		return;

	//	如果精灵处理闪烁中且闪烁颜色为nil则直接返回
	if (m_flash_duration > 0 && m_flash_hide_spr > 0)
		return;

	//	处理色调
	process_tone_texture();

	//	设置源矩形
	m_pSpr->SetTextureRect(m_src_rect_ptr->m_rect_data[0], m_src_rect_ptr->m_rect_data[1], m_src_rect_ptr->m_rect_data[2], m_src_rect_ptr->m_rect_data[3]);

	//	关于精灵混合颜色的管理：

	//	如果指定了flash颜色的情况下：
	//		如果也指定了color颜色的情况下：
	//			如果color颜色的不透明度大于flash颜色的不透明度，则以color颜色混合。否则以flash颜色混合。
	//		如果没有指定color颜色的情况下：
	//			直接以flash颜色混合。
	//	如果没指定flash颜色的情况下：
	//		如果指定了color颜色则以color颜色混合，否则不混合。

	s32		save_blend_mode = -1;
	DWORD	save_blend_color = 0;

	//	如果指定了flash颜色的情况下
	if (m_flash_duration > 0 && m_flash_color != 0)
	{
		save_blend_mode = m_pSpr->GetBlendMode();
		save_blend_color = m_flash_color;
	}

	//	如果指定了color颜色的情况下
	if (m_color_ptr->GetColor() != 0)
	{
		//	并且设置了flash颜色的情况下
		if (save_blend_color > 0)
		{
			if (GETA(m_color_ptr->GetColor()) > GETA(m_flash_color))
			{
				save_blend_color = m_color_ptr->GetColor();
			}
		}
		//	没有设置flash颜色的情况下
		else
		{
			switch (m_blend_type)
			{
			case 1:
				m_save_blend_type = 1;
				save_blend_mode = BLEND_COLORADD;
				break;
			case 2:
				m_save_blend_type = 2;
				save_blend_mode = BLEND_ALPHASUBTRACT;
				break;
			default:
				m_save_blend_type = 0;
				save_blend_mode = BLEND_DEFAULT;
				break;
			}
			save_blend_color = m_color_ptr->GetColor();
		}
	}
	
	//	混合颜色的处理
	if (save_blend_mode > 0)
	{
		m_pSpr->SetBlendMode(save_blend_mode | BLEND_COLORBLNED);
		m_pSpr->SetBlendColor(save_blend_color);
	}
	else if (m_save_blend_type != m_blend_type)
	{
		switch (m_blend_type)
		{
		case 1:
			m_save_blend_type = 1;
			m_pSpr->SetBlendMode(BLEND_COLORMUL);
			break;
		case 2:
			m_save_blend_type = 2;
			m_pSpr->SetBlendMode(BLEND_ALPHASUBTRACT);
			break;
		default:
			m_save_blend_type = 0;
			m_pSpr->SetBlendMode(BLEND_DEFAULT);
			break;
		}
	}

	// set the sprite's opacity
	m_pSpr->SetColor(ARGB(m_opacity, 255, 255, 255));

	// render the sprite to the screen
	float x, y;

	if (!m_viewport_ptr)
	{
		x = m_x;
		y = m_y;
	}
	else
	{
		const CRbRect * rect_ptr = m_viewport_ptr->GetRectPtr();

		x = m_x + rect_ptr->m_rect_data[0] - m_viewport_ptr->m_ox;
		y = m_y + rect_ptr->m_rect_data[1] - m_viewport_ptr->m_oy;
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

		const CRbRenderState::RbRenderClipRect& clip_rect = GetAppPtr()->GetRenderState()->GetClipRect();

		//	先描绘不透明部分
		min_x = SinMax(clip_rect.x, top_rect.x1);
		min_y = SinMax(clip_rect.y, top_rect.y1);

		max_x = SinMin(clip_rect.x + clip_rect.width, top_rect.x2);
		max_y = SinMin(clip_rect.y + clip_rect.height, top_rect.y2);

		cw = max_x - min_x;
		ch = max_y - min_y;

		if (cw > 0 && ch > 0)
		{
			GetAppPtr()->GetRenderState()->Clip(min_x, min_y, cw, ch);
			m_pSpr->Render(x, y);
		}

		//	其次描绘透明草丛部分
		// half opacity : simulate bush effect
		m_pSpr->SetColor(0x00FFFFFF + ((m_opacity * m_bush_opacity / 255) << 24));//MAKE_ARGB_8888(m_opacity * m_bush_opacity / 255, 255, 255, 255));

		min_x = SinMax(clip_rect.x, bottom_rect.x1);
		min_y = SinMax(clip_rect.y, bottom_rect.y1);

		max_x = SinMin(clip_rect.x + clip_rect.width,	bottom_rect.x2);
		max_y = SinMin(clip_rect.y + clip_rect.height,	bottom_rect.y2);

		cw = max_x - min_x;
		ch = max_y - min_y;

		if (cw > 0 && ch > 0)
		{
			GetAppPtr()->GetRenderState()->Clip(min_x, min_y, cw, ch);
			m_pSpr->Render(x, y);
		}

		GetAppPtr()->GetRenderState()->Restore();
		m_pSpr->SetColor(0x00FFFFFF + (m_opacity << 24));//MAKE_ARGB_8888(m_opacity, 255, 255, 255));
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

VALUE CRbSprite::update()
{
	check_raise();

	if (m_movie_playing)
	{
		HGE* hge = GetAppPtr()->GetHgePtr();
		DWORD* pTexData = hge->Texture_Lock(m_bitmap_ptr->GetBitmapPtr()->quad.tex, false);
		GetVideoMgr()->UpdateMovieTexture(pTexData);
		hge->Texture_Unlock(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
		m_bitmap_ptr->SetModifyCount();
	}
	if (m_flash_duration)
	{
		BYTE a, r, g, b;

		GET_ARGB_8888(m_flash_color, a, r, g, b);
		a += m_flash_reduce_count_per_frame;
		m_flash_color = MAKE_ARGB_8888(a, r, g, b);
	}
	return Qnil;
}

VALUE CRbSprite::flash(VALUE color, VALUE duration)
{
	check_raise();
	SafeColorValue(color);
	SafeFixnumValue(duration);

	CRbColor* color_ptr = GetObjectPtr<CRbColor>(color);
	m_flash_color = color_ptr->GetColor();
	m_flash_duration = FIX2INT(duration);
	m_flash_reduce_count_per_frame = (int)(255.0f / m_flash_duration);
	color_ptr = NULL;

	return Qnil;
}

VALUE CRbSprite::set_bitmap(VALUE bitmap)
{
	check_raise();

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

		m_bitmap_ptr = GetObjectPtr<CRbBitmap>(bitmap);

		m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
		m_pSpr->SetSrcRectDirty();
		
		VALUE __argv[] = { RUBY_0, RUBY_0, INT2FIX(m_bitmap_ptr->GetWidth()), INT2FIX(m_bitmap_ptr->GetHeight()) };

		rb_funcall2(m_src_rect_ptr->GetObject(), rb_intern("set"), 4, __argv);
	}

	return bitmap;
}

VALUE CRbSprite::set_ox(VALUE ox)
{
	check_raise();
	super::set_ox(ox);

	m_pSpr->SetOX(m_ox);
	m_pSpr->SetSrcRectDirty();

	return ox;
}

VALUE CRbSprite::set_oy(VALUE oy)
{
	check_raise();
	super::set_oy(oy);

	m_pSpr->SetOY(m_oy);
	m_pSpr->SetSrcRectDirty();

	return oy;
}

VALUE CRbSprite::get_src_rect()
{
	check_raise();
	return ReturnObject(m_src_rect_ptr);
}

VALUE CRbSprite::set_src_rect(VALUE src_rect)
{
	check_raise();
	SafeRectValue(src_rect);
	m_src_rect_ptr = GetObjectPtr<CRbRect>(src_rect);
	return Qnil;
}

VALUE CRbSprite::get_angle()
{
	check_raise();
	return m_angle_rad;
}

VALUE CRbSprite::set_angle(VALUE angle)
{
	check_raise();
	SafeNumericValue(angle);

	int agl = NUM2INT(angle);
	m_angle_rad = INT2FIX(agl);

	m_angle = SinDeg2Rad(360.0f - agl);
	m_pSpr->SetAngle(m_angle);
	m_pSpr->SetSrcRectDirty();

	return angle;
}

VALUE CRbSprite::get_mirror()
{
	check_raise();
	return m_mirror;
}

VALUE CRbSprite::set_mirror(VALUE mirror)
{
	check_raise();
	m_mirror = Ruby2RbBool(mirror);

	m_pSpr->SetFlip(RTEST(m_mirror), m_pSpr->IsFlipY(), true);
	return mirror;
}

VALUE CRbSprite::get_width()
{
	check_raise();
	return INT2FIX(m_src_rect_ptr->m_rect_data[2]);
}

VALUE CRbSprite::get_height()
{
	check_raise();
	return INT2FIX(m_src_rect_ptr->m_rect_data[3]);
}

VALUE CRbSprite::get_bush_depth()
{
	check_raise();
	return m_bush_depth;
}

VALUE CRbSprite::set_bush_depth(VALUE bush_depth)
{
	check_raise();
	SafeFixnumValue(bush_depth);

	m_bush_depth = bush_depth;

	return bush_depth;
}

VALUE CRbSprite::get_bush_opacity()
{
	check_raise();
	return INT2FIX(m_bush_opacity);
}

VALUE CRbSprite::set_bush_opacity(VALUE bush_opacity)
{
	check_raise();
	SafeFixnumValue(bush_opacity);

	m_bush_opacity = FIX2INT(bush_opacity);
	m_bush_opacity = SinBound(m_bush_opacity, 0, 255);

	return bush_opacity;
}

VALUE CRbSprite::get_wave_amp()
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::set_wave_amp(VALUE wave_amp)
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::get_wave_length()
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::set_wave_length(VALUE wave_length)
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::get_wave_speed()
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::set_wave_speed(VALUE wave_speed)
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::get_wave_phase()
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::set_wave_phase(VALUE wave_phase)
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE CRbSprite::play_movie(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	if (GetVideoMgr()->IsOccupied())
		rb_raise(rb_eSinError, "Video Player is occupied.\n You must stop first.");

	VALUE filename, volume;

	if (rb_scan_args(argc, argv, "11", &filename, &volume) == 1)
		volume = INT2FIX(0);
	else
		SafeFixnumValue(volume);

	SafeStringValue(filename);

	int width, height;
	HGE * hge = GetAppPtr()->GetHgePtr();

	if (!GetVideoMgr()->LoadMovie(Kconv::UTF8ToUnicode(RSTRING_PTR(filename)), width, height))
		rb_raise(rb_eSinError, "Falied to load video: `%s'.", Kconv::UTF8ToAnsi(RSTRING_PTR(filename)));

	VALUE __argv1[] = {INT2FIX(width), INT2FIX(height)};
	VALUE bitmap = rb_class_new_instance(2, __argv1, rb_cBitmap);

	// 设置位图修改计数值为-1
	m_ref_bitmap_modify_count = -1;

	SafeBitmapValue(bitmap);
	m_bitmap_ptr = GetObjectPtr<CRbBitmap>(bitmap);
	m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
	m_pSpr->SetSrcRectDirty();
	VALUE __argv2[] = { RUBY_0, RUBY_0, INT2FIX(m_bitmap_ptr->GetWidth()), INT2FIX(m_bitmap_ptr->GetHeight()) };
	rb_funcall2(m_src_rect_ptr->GetObject(), rb_intern("set"), 4, __argv2);

	if (!m_bitmap_ptr->GetBitmapPtr()->quad.tex)
	{
		GetVideoMgr()->StopMovie();
		return Qfalse;
	}
	GetVideoMgr()->PlayMovie(FIX2LONG(volume));
	m_movie_playing = true;
	return Qnil;
}

VALUE CRbSprite::is_playing()
{
	if (!m_movie_playing)
		return Qfalse;

	if (GetVideoMgr()->IsMoviePlaying())
		return Qtrue;

	GetVideoMgr()->StopMovie();
	m_bitmap_ptr = 0;
	m_movie_playing = false;
	return Qfalse;
}

VALUE CRbSprite::get_volume()
{
	if (!m_movie_playing)
		return Qnil;
	return LONG2FIX(GetVideoMgr()->GetVolume());
}

VALUE CRbSprite::set_volume(VALUE volume)
{
	if (!m_movie_playing)
		return Qfalse;
	SafeFixnumValue(volume);
	GetVideoMgr()->SetVolume(FIX2LONG(volume));
	return volume;
}

VALUE CRbSprite::replay_at_finish()
{
	if (!m_movie_playing)
		return Qfalse;

	if (GetVideoMgr()->IsMoviePlaying())
		return Qtrue;

	GetVideoMgr()->RewindMovie();
	return Qnil;
}

VALUE CRbSprite::stop_movie()
{
	if (!m_movie_playing)
		return Qfalse;

	GetVideoMgr()->StopMovie();
	m_bitmap_ptr = 0;
	m_movie_playing = false;
	return Qnil;
}

VALUE CRbSprite::rewind_movie()
{
	if (!m_movie_playing)
		return Qfalse;
	GetVideoMgr()->RewindMovie();
	return Qnil;
}

/*
 *	以下定义ruby方法
 */
imp_method(CRbSprite, dispose)
imp_method(CRbSprite, is_disposed)
imp_method_vargs(CRbSprite, play_movie)
imp_method(CRbSprite, get_volume)
imp_method01(CRbSprite, set_volume)
imp_method(CRbSprite, is_playing)
imp_method(CRbSprite, replay_at_finish)
imp_method(CRbSprite, stop_movie)
imp_method(CRbSprite, rewind_movie)

imp_method(CRbSprite, update)
imp_method(CRbSprite, get_width)
imp_method(CRbSprite, get_height)
imp_method02(CRbSprite, flash)

imp_attr_accessor(CRbSprite, src_rect)
imp_attr_accessor(CRbSprite, angle)
imp_attr_accessor(CRbSprite, mirror)

imp_attr_accessor(CRbSprite, bush_depth)
imp_attr_accessor(CRbSprite, bush_opacity)

imp_attr_accessor(CRbSprite, wave_amp)
imp_attr_accessor(CRbSprite, wave_length)
imp_attr_accessor(CRbSprite, wave_speed)
imp_attr_accessor(CRbSprite, wave_phase)