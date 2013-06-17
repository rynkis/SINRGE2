/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Plane
*/
#include "RbExport.h"
#include "CRbBitmap.h"
#include "CRbViewport.h"
#include "CRbColor.h"
#include "CRbTone.h"
#include "CRbRect.h"
#include "CRbPlane.h"
#include "sin_sprite.h"
#include "sin_app.h"

VALUE rb_cPlane;

/*
 *	构造
 */
CRbPlane::CRbPlane()
	: m_opacity(255)
	, m_blend_type(0)
	, m_save_blend_type(0)
	, m_ref_bitmap_modify_count(-1)
	, m_ref_tone(0x0)
	, m_tone_tex(0)
	, m_node(0)
	, m_bitmap_ptr(0)
	, m_color_ptr(0)
	, m_tone_ptr(0)
	, m_tone_tex_width(0)
	, m_tone_tex_height(0)
{
	m_pSpr = new SinSprite();
}

/*
 *	释放
 */
CRbPlane::~CRbPlane()
{
	if (m_tone_tex)
	{
		GetAppPtr()->GetHgePtr()->Texture_Free(m_tone_tex);
		m_tone_tex = 0;
	}
	
	SAFE_DELETE(m_pSpr);

	CRbRenderTree::DestroyNode(&m_node);
}

void CRbPlane::InitLibrary()
{
	rb_cPlane = rb_define_class_under(rb_mSin, "Plane", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cPlane, ObjAllocate<CRbPlane>);
	rb_define_method(rb_cPlane, "initialize",	(RbFunc)dm_initialize,		-1);

	// instance method
	rb_define_method(rb_cPlane, "dispose",		(RbFunc)dm_dispose,			0);
	rb_define_method(rb_cPlane, "disposed?",	(RbFunc)dm_is_disposed,		0);

	// object attribute
	rb_define_method(rb_cPlane, "viewport",		(RbFunc)dm_get_viewport,	0);
	rb_define_method(rb_cPlane, "viewport=",	(RbFunc)dm_set_viewport,	1);

	// object attribute
	rb_define_method(rb_cPlane, "bitmap",		(RbFunc)dm_get_bitmap,		0);
	rb_define_method(rb_cPlane, "bitmap=",		(RbFunc)dm_set_bitmap,		1);
	rb_define_method(rb_cPlane, "visible",		(RbFunc)dm_get_visible,		0);
	rb_define_method(rb_cPlane, "visible=",		(RbFunc)dm_set_visible,		1);
	rb_define_method(rb_cPlane, "z",			(RbFunc)dm_get_z,			0);
	rb_define_method(rb_cPlane, "z=",			(RbFunc)dm_set_z,			1);
	rb_define_method(rb_cPlane, "ox",			(RbFunc)dm_get_ox,			0);
	rb_define_method(rb_cPlane, "ox=",			(RbFunc)dm_set_ox,			1);
	rb_define_method(rb_cPlane, "oy",			(RbFunc)dm_get_oy,			0);
	rb_define_method(rb_cPlane, "oy=",			(RbFunc)dm_set_oy,			1);
	rb_define_method(rb_cPlane, "zoom_x",		(RbFunc)dm_get_zoom_x,		0);
	rb_define_method(rb_cPlane, "zoom_x=",		(RbFunc)dm_set_zoom_x,		1);
	rb_define_method(rb_cPlane, "zoom_y",		(RbFunc)dm_get_zoom_y,		0);
	rb_define_method(rb_cPlane, "zoom_y=",		(RbFunc)dm_set_zoom_y,		1);

	rb_define_method(rb_cPlane, "opacity",		(RbFunc)dm_get_opacity,		0);
	rb_define_method(rb_cPlane, "opacity=",		(RbFunc)dm_set_opacity,		1);
	rb_define_method(rb_cPlane, "blend_type",	(RbFunc)dm_get_blend_type,	0);
	rb_define_method(rb_cPlane, "blend_type=",	(RbFunc)dm_set_blend_type,	1);
	rb_define_method(rb_cPlane, "color",		(RbFunc)dm_get_color,		0);
	rb_define_method(rb_cPlane, "color=",		(RbFunc)dm_set_color,		1);
	rb_define_method(rb_cPlane, "tone",			(RbFunc)dm_get_tone,		0);
	rb_define_method(rb_cPlane, "tone=",		(RbFunc)dm_set_tone,		1);

	// supplement
 	rb_define_method(rb_cPlane, "to_s",			(RbFunc)dm_to_string,		0);
}

/*
 *	标记
 */
void CRbPlane::mark()
{
	if (m_viewport_ptr) m_viewport_ptr->MarkObject();
	if (m_bitmap_ptr)	m_bitmap_ptr->MarkObject();
	if (m_color_ptr)	m_color_ptr->MarkObject();
	if (m_tone_ptr)		m_tone_ptr->MarkObject();
}

/**
 *	@call
 *		Plane.new				-> plane 对象 。
 *		Plane.new(viewport)		-> plane 对象。
 *
 *	@desc
 *		生成 Plane 对象。
 */
VALUE CRbPlane::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE viewport, color, tone;

	//	检查参数
	rb_scan_args(argc, argv, "01", &viewport);

	if (!NIL_P(viewport))
	{
		SafeViewportValue(viewport);
		m_viewport_ptr = GetObjectPtr<CRbViewport>(viewport);
	}

	//	创建并添加渲染结点
	m_node = CRbRenderTree::AllocNode(RenderProc, obj, 0, 0, viewport);
	CRbRenderTree::InsertNode(m_node);

	m_visible = Qtrue;

	//	创建color和tone对象
	VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};
	
	color = rb_class_new_instance(4, __argv, rb_cColor);
	tone = rb_class_new_instance(4, __argv, rb_cTone);

	m_color_ptr = GetObjectPtr<CRbColor>(color);
	m_tone_ptr = GetObjectPtr<CRbTone>(tone);

	//	取消释放标记
	m_disposed = false;

	return obj;
}

/*
 *	渲染（描绘）
 */
void CRbPlane::render(u32 id)
{
	//	有效性检查
	if (!m_bitmap_ptr)
		return;

	if (m_bitmap_ptr->IsDisposed())
		return;

	//	如果缩放比例小于0则直接返回
	if (m_pSpr->GetZoomX() <= 0.0f || m_pSpr->GetZoomY() <= 0.0f)
		return;

	//	渲染属性设置
	s32		save_blend_mode = -1;
	f32		ofs_x, ofs_y;
	u32		wrap_w, wrap_h;
	{
		if (m_color_ptr->GetColor() != 0)
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
			m_pSpr->SetBlendColor(m_color_ptr->GetColor());
		}
	
		//	混合颜色的处理
		if (save_blend_mode > 0)
		{
			m_pSpr->SetBlendMode(save_blend_mode | BLEND_COLORBLNED);
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
		
		if (!m_viewport_ptr)
		{
			wrap_w = GetAppPtr()->GetFrameWidth();
			wrap_h = GetAppPtr()->GetFrameHeight();

			ofs_x = 0;
			ofs_y = 0;
		}
		else
		{
			const CRbRect* rect_ptr = m_viewport_ptr->GetRectPtr();

			wrap_w = rect_ptr->m_rect_data[2];
			wrap_h = rect_ptr->m_rect_data[3];

			ofs_x = rect_ptr->m_rect_data[0] - m_viewport_ptr->m_ox;
			ofs_y = rect_ptr->m_rect_data[1] - m_viewport_ptr->m_oy;
		}
	}

	//	处理色调
	process_tone_texture();

	//	设置源矩形
	m_pSpr->SetTextureRect(0, 0, m_bitmap_ptr->GetWidth(), m_bitmap_ptr->GetHeight());

	//	渲染
	{
		int zoom_w = (int)(m_bitmap_ptr->GetWidth() * m_pSpr->GetZoomX());
		int zoom_h = (int)(m_bitmap_ptr->GetHeight() * m_pSpr->GetZoomY());

		int ox = (int)m_ox % zoom_w;
		int oy = (int)m_oy % zoom_h;

		if (ox < 0) ox += zoom_w;
		if (oy < 0) oy += zoom_h;

		int xloop = (wrap_w + zoom_w - 1) / zoom_w; 
		int yloop = (wrap_h + zoom_h - 1) / zoom_h; 

		for (int j = 0; j <= yloop; ++j)
		{
			for (int i = 0; i <= xloop; ++i)
			{
				m_pSpr->Render(ofs_x - ox + i * zoom_w, ofs_y - oy + j * zoom_h);
			}
		}
	}

	//	恢复处理
	if (save_blend_mode > 0)
	{
		m_pSpr->SetBlendMode(save_blend_mode);
		m_pSpr->SetBlendColor(0);
		save_blend_mode = -1;
	}
}

void CRbPlane::process_tone_texture()
{
	bool change = false;

	HGE * hge = GetAppPtr()->GetHgePtr();

	if (m_tone_ptr->GetColor() != m_ref_tone)
	{
		m_ref_tone = m_tone_ptr->GetColor();

		if (m_ref_tone != 0)
		{
			m_ref_bitmap_modify_count = m_bitmap_ptr->GetModifyCount();
			change = true;
		}
		else
		{
			m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
			if (m_tone_tex)
			{
				hge->Texture_Free(m_tone_tex);
				m_tone_tex = 0;
			}
		}
	}
	else if (m_ref_bitmap_modify_count != m_bitmap_ptr->GetModifyCount())
	{
		m_ref_bitmap_modify_count = m_bitmap_ptr->GetModifyCount();

		if (m_tone_tex)
			change = true;
	}

	if (change)
	{
		if (m_tone_tex_width != m_bitmap_ptr->GetWidth() || m_tone_tex_height != m_bitmap_ptr->GetHeight())
		{
			if (m_tone_tex)
			{
				hge->Texture_Free(m_tone_tex);
				m_tone_tex = 0;
			}

			m_tone_tex_width = m_bitmap_ptr->GetWidth();
			m_tone_tex_height = m_bitmap_ptr->GetHeight();

			m_tone_tex = hge->Texture_Create(m_tone_tex_width, m_tone_tex_height);
			if (!m_tone_tex)
				rb_raise(rb_eSinError,"Create Texture Error !");
		}

		if (CRbBitmap::AdjustTexturesToneDouble(m_bitmap_ptr->GetBitmapPtr(), m_tone_tex, m_ref_tone))
		{
			m_pSpr->SetTexture(m_tone_tex);
			m_pSpr->SetSrcRectDirty();
		}
		else
		{
			m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
			
			if (m_tone_tex)
			{
				hge->Texture_Free(m_tone_tex);
				m_tone_tex = 0;
			}
		}
	}
}

VALUE CRbPlane::dispose()
{
	if (m_disposed)
		return Qnil;

	m_bitmap_ptr = 0;
	m_disposed = true;
	return Qnil;
}

VALUE CRbPlane::is_disposed()
{
	return C2RbBool(m_disposed);
}

void CRbPlane::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed plane");
}

VALUE CRbPlane::get_bitmap()
{
	check_raise();
	return ReturnObject(m_bitmap_ptr);
}

VALUE CRbPlane::set_bitmap(VALUE bitmap)
{
	check_raise();

	// 设置位图修改计数值为-1
	m_ref_bitmap_modify_count = -1;

	if (NIL_P(bitmap))
	{
		m_bitmap_ptr = 0;
	}
	else
	{
		SafeBitmapValue(bitmap);

		m_bitmap_ptr = GetObjectPtr<CRbBitmap>(bitmap);

		m_pSpr->SetTexture(m_bitmap_ptr->GetBitmapPtr()->quad.tex);
		m_pSpr->SetSrcRectDirty();
	}

	return bitmap;
}

VALUE CRbPlane::get_zoom_x()
{
	check_raise();
	return rb_float_new(m_pSpr->GetZoomX());
}

VALUE CRbPlane::set_zoom_x(VALUE zoom_x)
{
	check_raise();
	SafeNumericValue(zoom_x);

	m_pSpr->SetZoomX((float)NUM2DBL(zoom_x));
	m_pSpr->SetSrcRectDirty();

	return zoom_x;
}

VALUE CRbPlane::get_zoom_y()
{
	check_raise();
	return rb_float_new(m_pSpr->GetZoomY());
}

VALUE CRbPlane::set_zoom_y(VALUE zoom_y)
{
	check_raise();
	SafeNumericValue(zoom_y);

	m_pSpr->SetZoomY((float)NUM2DBL(zoom_y));
	m_pSpr->SetSrcRectDirty(true);

	return zoom_y;
}

VALUE CRbPlane::get_opacity()
{
	check_raise();
	return INT2FIX(m_opacity);
}

VALUE CRbPlane::set_opacity(VALUE opacity)
{
	check_raise();
	SafeFixnumValue(opacity);

	m_opacity = FIX2INT(opacity);
	m_opacity = SinBound(m_opacity, 0, 255);

	return opacity;
}

VALUE CRbPlane::get_blend_type()
{
	check_raise();
	return INT2FIX(m_blend_type);
}

VALUE CRbPlane::set_blend_type(VALUE blend_type)
{
	check_raise();
	SafeFixnumValue(blend_type);
	m_blend_type = FIX2INT(blend_type);
	return blend_type;
}

VALUE CRbPlane::get_color()
{
	check_raise();
	return ReturnObject(m_color_ptr);
}

VALUE CRbPlane::set_color(VALUE color)
{
	check_raise();
	/*if (rb_obj_is_kind_of(color, rb_cInteger))
	{
		VALUE __argv[] = { color };
		VALUE col = rb_class_new_instance(1, __argv, rb_cColor);
		m_color_ptr = GetObjectPtr<CRbColor>(col);
	}
	else
	{*/
		SafeColorValue(color);
		m_color_ptr = GetObjectPtr<CRbColor>(color);
	//}
	return color;
}

VALUE CRbPlane::get_tone()
{
	check_raise();
	return ReturnObject(m_tone_ptr);
}

VALUE CRbPlane::set_tone(VALUE tone)
{
	check_raise();
	SafeToneValue(tone);
	m_tone_ptr = GetObjectPtr<CRbTone>(tone);
	return tone;
}

VALUE CRbPlane::get_z()
{
	check_raise();
	return INT2FIX(m_z);
}

VALUE CRbPlane::set_z(VALUE z)
{
	check_raise();
	SafeFixnumValue(z);

	if (m_z != FIX2INT(z))
	{
		m_z = FIX2INT(z);
		m_node->z = m_z;
		CRbRenderTree::InsertNode(CRbRenderTree::DeleteNode(m_node));
	}
	return z;
}

VALUE CRbPlane::set_viewport(VALUE viewport)
{
	check_raise();
	if (NIL_P(viewport))
	{
		m_viewport_ptr = 0;
	}
	else
	{
		SafeViewportValue(viewport);
		m_viewport_ptr = GetObjectPtr<CRbViewport>(viewport);
	}

	CRbRenderTree::DeleteNode(m_node);
	m_node->viewport = viewport;
	CRbRenderTree::InsertNode(m_node);

	return viewport;
}

/*
 *	以下定义ruby方法
 */
imp_method(CRbPlane, dispose)
imp_method(CRbPlane, is_disposed)

imp_attr_accessor(CRbPlane, bitmap)
imp_attr_accessor(CRbPlane, zoom_x)
imp_attr_accessor(CRbPlane, zoom_y)
imp_attr_accessor(CRbPlane, opacity)
imp_attr_accessor(CRbPlane, blend_type)
imp_attr_accessor(CRbPlane, color)
imp_attr_accessor(CRbPlane, tone)