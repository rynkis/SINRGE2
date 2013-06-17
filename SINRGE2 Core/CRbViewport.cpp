/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Viewport
*/
#include "CRbViewport.h"
#include "CRbColor.h"
#include "CRbTone.h"
#include "CRbRect.h"
#include "sin_color.h"
#include "sin_app.h"

VALUE rb_cViewport;

namespace
{
	const u32	id_render_normal		= 0;
	const u32	id_render_to_texture	= 1;
}

HGE * CRbViewport::s_pHge = 0;

CRbViewport::CRbViewport()
	: m_flash_duration(0)
	, m_flash_reduce_count_per_frame(0)
	, m_flash_hide_spr(0)
	, m_flash_color(0)
	, m_node(0)
	, m_tail(0), m_head(0)
	, m_rect_ptr(0), m_color_ptr(0), m_tone_ptr(0)
{
	s_pHge = hgeCreate(HGE_VERSION);

	m_quad.v[0].z = 
	m_quad.v[1].z = 
	m_quad.v[2].z = 
	m_quad.v[3].z = 0.5f;
 	m_quad.tex	  = 0;
  	m_quad.blend  = BLEND_DEFAULT; 
	m_quad.blend_color = 0;
}

CRbViewport::~CRbViewport()
{
	for (; m_head; m_head = m_head->next)
		m_head->viewport = 0;	//	清空视口标记为0

	CRbRenderTree::DestroyNode(&m_node);

	s_pHge->Release();
}

void CRbViewport::InitLibrary()
{
	/**
	 *	@classname
	 *		Viewport
	 *
	 *	@desc
	 *		视口的类。在画面的一部分中显示精灵，而不影响其他部分的情况下使用。
	 */
	rb_cViewport = rb_define_class_under(rb_mSin, "Viewport", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cViewport, ObjAllocate<CRbViewport>);
	rb_define_method(rb_cViewport, "initialize", (RbFunc)dm_initialize,		-1);

	// instance method
	rb_define_method(rb_cViewport, "dispose",	(RbFunc)dm_dispose,		0);
	rb_define_method(rb_cViewport, "disposed?",	(RbFunc)dm_is_disposed, 0);
	rb_define_method(rb_cViewport, "flash",		(RbFunc)dm_flash,		2);
	rb_define_method(rb_cViewport, "update",	(RbFunc)dm_update,		0);

	// object attribute
	rb_define_method(rb_cViewport, "rect",		(RbFunc)dm_get_rect,	0);
	rb_define_method(rb_cViewport, "rect=",		(RbFunc)dm_set_rect,	1);
	rb_define_method(rb_cViewport, "visible",	(RbFunc)dm_get_visible,	0);
	rb_define_method(rb_cViewport, "visible=",	(RbFunc)dm_set_visible,	1);
	rb_define_method(rb_cViewport, "z",			(RbFunc)dm_get_z,		0);
	rb_define_method(rb_cViewport, "z=",		(RbFunc)dm_set_z,		1);
	rb_define_method(rb_cViewport, "ox",		(RbFunc)dm_get_ox,		0);
	rb_define_method(rb_cViewport, "ox=",		(RbFunc)dm_set_ox,		1);
	rb_define_method(rb_cViewport, "oy",		(RbFunc)dm_get_oy,		0);
	rb_define_method(rb_cViewport, "oy=",		(RbFunc)dm_set_oy,		1);
	rb_define_method(rb_cViewport, "color",		(RbFunc)dm_get_color,	0);
	rb_define_method(rb_cViewport, "color=",	(RbFunc)dm_set_color,	1);
	rb_define_method(rb_cViewport, "tone",		(RbFunc)dm_get_tone,	0);
	rb_define_method(rb_cViewport, "tone=",		(RbFunc)dm_set_tone,	1);

	// supplement
 	rb_define_method(rb_cViewport, "to_s",		(RbFunc)dm_to_string,	0);
}

void CRbViewport::mark()
{
	if (m_rect_ptr)		m_rect_ptr->MarkObject();
	if (m_color_ptr)	m_color_ptr->MarkObject();
	if (m_tone_ptr)		m_tone_ptr->MarkObject();
}

/**
 *	@call
 *		Viewport.new(x, y, width, height)	-> viewport 对象。
 *		Viewport.new(rect)		 			-> viewport 对象。
 *
 *	@desc
 *		生成 Viewport 对象。
 */
VALUE CRbViewport::initialize(int argc, VALUE * argv, VALUE obj)
{
	//	检查参数个数
	if(argc != 0 && argc != 1 && argc != 4)
		rb_raise(rb_eArgError, "wrong number of arguments (%d for 4)", argc);

	if (argc == 0)
	{
		VALUE __argv[] = {RUBY_0, RUBY_0, INT2FIX(GetAppPtr()->GetFrameWidth()), INT2FIX(GetAppPtr()->GetFrameHeight())};
		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);
		m_rect_ptr = GetObjectPtr<CRbRect>(rect);
	}
	else if (argc == 1)
	{
		//	检查参数类型
		SafeRectValue(argv[0])

		//	保存rect对象
		m_rect_ptr = GetObjectPtr<CRbRect>(argv[0]);
	}
	else
	{
		//	检查参数类型
		for(int i = 0; i < argc; ++i)
		{
			SafeFixnumValue(argv[i]);
		}

		//	创建rect对象
		VALUE __argv[] = {argv[0], argv[1], argv[2], argv[3]};

		VALUE rect = rb_class_new_instance(4, __argv, rb_cRect);

		m_rect_ptr = GetObjectPtr<CRbRect>(rect);
	}

	//	创建color和tone对象
	VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};
	
	VALUE color = rb_class_new_instance(4, __argv, rb_cColor);
	VALUE tone = rb_class_new_instance(4, __argv, rb_cTone);

	m_color_ptr = GetObjectPtr<CRbColor>(color);
	m_tone_ptr = GetObjectPtr<CRbTone>(tone);

	m_visible = Qtrue;

	//	创建并添加渲染结点
	m_node = CRbRenderTree::AllocNode(RenderProc, obj, id_render_normal, 0, Qnil);
	CRbRenderTree::InsertNode(m_node);

	m_disposed = false;

	return obj;
}

void CRbViewport::render(u32 id)
{
	//	如果视口处于闪烁中且闪烁颜色为nil则直接返回
	if (m_flash_duration > 0 && m_flash_hide_spr > 0)
		return;

	//	根据viewport矩形进行裁剪
	GetAppPtr()->GetRenderState()->ClipAndSave(m_rect_ptr->m_rect_data[0], m_rect_ptr->m_rect_data[1], m_rect_ptr->m_rect_data[2], m_rect_ptr->m_rect_data[3]);

	//	渲染viewport中的所有对象
	for (RbRenderNode * p = m_head; p; p = p->next)
	{
		p->renderproc(p->value, p->id);
	}

	//	取消裁剪 还原屏幕大小
	GetAppPtr()->GetRenderState()->ClipAndSave(0, 0, GetAppPtr()->GetFrameWidth(), GetAppPtr()->GetFrameHeight());

	//	视口内对象描绘完毕之后再处理视口的混色闪烁等

	// render it only the icolor isn't zero
	if (m_color_ptr->GetColor() != 0 || m_tone_ptr->GetColor() != 0 || m_flash_duration > 0)
	{
		m_quad.v[0].x = (float)m_rect_ptr->m_rect_data[0]; m_quad.v[0].y = (float)m_rect_ptr->m_rect_data[1];
		m_quad.v[1].x = (float)(m_rect_ptr->m_rect_data[0] + m_rect_ptr->m_rect_data[2]); m_quad.v[1].y = (float)m_rect_ptr->m_rect_data[1];
		m_quad.v[2].x = (float)m_rect_ptr->m_rect_data[0] + m_rect_ptr->m_rect_data[2]; m_quad.v[2].y = (float)(m_rect_ptr->m_rect_data[1] + m_rect_ptr->m_rect_data[3]);
		m_quad.v[3].x = (float)m_rect_ptr->m_rect_data[0]; m_quad.v[3].y = (float)(m_rect_ptr->m_rect_data[1] + m_rect_ptr->m_rect_data[3]);
	}

	//	视口混合颜色
	if (m_color_ptr->GetColor() != 0)
	{
		m_quad.v[0].col = 
		m_quad.v[1].col = 
		m_quad.v[2].col = 
		m_quad.v[3].col = m_color_ptr->GetColor();

		s_pHge->Gfx_RenderQuad(&m_quad);
	}

	//	视口闪烁颜色
	if (m_flash_duration > 0)
	{
		m_quad.v[0].col = 
		m_quad.v[1].col = 
		m_quad.v[2].col = 
		m_quad.v[3].col = m_flash_color;

		s_pHge->Gfx_RenderQuad(&m_quad);
	}
	
	// 使用减淡混合方式处理视口色调，效果会比较诡异，但有总比没有好 = =
	if (m_tone_ptr->GetColor() != 0)
	{
		m_quad.v[0].col = 
		m_quad.v[1].col = 
		m_quad.v[2].col = 
		m_quad.v[3].col = m_tone_ptr->GetColor();
		m_quad.blend = BLEND_COLORMUL;
			
		s_pHge->Gfx_RenderQuad(&m_quad);

		m_quad.blend = BLEND_DEFAULT;
	}
}

VALUE CRbViewport::dispose()
{
	if (m_disposed)
		return Qnil;

	m_disposed = true;
	return Qnil;
}

VALUE CRbViewport::is_disposed()
{
	return C2RbBool(m_disposed);
}

void CRbViewport::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed viewport");
}

VALUE CRbViewport::get_rect()
{
	check_raise();

	return ReturnObject(m_rect_ptr);
}

VALUE CRbViewport::set_rect(VALUE rect)
{
	check_raise();

	SafeRectValue(rect);
	m_rect_ptr = GetObjectPtr<CRbRect>(rect);
	return rect;
}

VALUE CRbViewport::get_color()
{
	check_raise();
	return ReturnObject(m_color_ptr);
}

VALUE CRbViewport::set_color(VALUE color)
{
	check_raise();
	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<CRbColor>(color);
	return color;
}

VALUE CRbViewport::get_tone()
{
	check_raise();
	return ReturnObject(m_tone_ptr);
}

VALUE CRbViewport::set_tone(VALUE tone)
{
	check_raise();
	SafeToneValue(tone);
	m_tone_ptr = GetObjectPtr<CRbTone>(tone);
	return tone;
}

VALUE CRbViewport::get_z()
{
	check_raise();
	return INT2FIX(m_z);
}

VALUE CRbViewport::set_z(VALUE z)
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

VALUE CRbViewport::update()
{
	check_raise();
	if (m_flash_duration)
	{
		BYTE a, r, g, b;

		GET_ARGB_8888(m_flash_color, a, r, g, b);
		a += m_flash_reduce_count_per_frame;
		m_flash_color = MAKE_ARGB_8888(a, r, g, b);
	}
	return Qnil;
}

VALUE CRbViewport::flash(VALUE color, VALUE duration)
{
	check_raise();
	SafeColorValue(color);
	SafeFixnumValue(duration);

	CRbColor * color_ptr = GetObjectPtr<CRbColor>(color);
	m_flash_color = color_ptr->GetColor();
	m_flash_duration = FIX2INT(duration);
	m_flash_reduce_count_per_frame = (int)(255.0f / m_flash_duration);
	color_ptr = NULL;

	return Qnil;
}

/*
 *	以下定义ruby方法
 */
imp_method(CRbViewport, dispose)
imp_method(CRbViewport, is_disposed)

imp_method(CRbViewport, update)
imp_method02(CRbViewport, flash)

imp_attr_accessor(CRbViewport, rect)
imp_attr_accessor(CRbViewport, color)
imp_attr_accessor(CRbViewport, tone)