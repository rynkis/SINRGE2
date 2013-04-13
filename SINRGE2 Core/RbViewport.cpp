#include "RbViewport.h"
#include "RbColor.h"
#include "RbTone.h"
#include "RbRect.h"
#include "SINRGE2.h"

using namespace Sin;

VALUE rb_cViewport;

namespace
{
	const u32	id_render_normal		= 0;
	const u32	id_render_to_texture	= 1;
}

HGE* RbViewport::s_pHge = 0;

RbViewport::RbViewport()
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

RbViewport::~RbViewport()
{
	for (; m_head; m_head = m_head->next)
		m_head->viewport = 0;	//	清空视口标记为0

	RbRenderTree::DestroyNode(&m_node);

	s_pHge->Release();
}

void RbViewport::InitLibrary()
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
	rb_define_alloc_func(rb_cViewport, ObjAllocate<RbViewport>);
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

void RbViewport::mark()
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
VALUE RbViewport::initialize(int argc, VALUE *argv, VALUE obj)
{
	//	检查参数个数
	if(argc != 1 && argc != 4)
		rb_raise(rb_eArgError, "wrong number of arguments (%d for 4)", argc);

	//	Viewport.new(rect) 的情况
	if(argc == 1)
	{
		//	检查参数类型
		SafeRectValue(argv[0])

		//	保存rect对象
		m_rect_ptr = GetObjectPtr<RbRect>(argv[0]);
	}
	//	Viewport.new(x, y, width, height) 的情况
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

		m_rect_ptr = GetObjectPtr<RbRect>(rect);
	}

	//	创建color和tone对象
	VALUE __argv[] = {RUBY_0, RUBY_0, RUBY_0, RUBY_0};
	
	VALUE color = rb_class_new_instance(4, __argv, rb_cColor);
	VALUE tone = rb_class_new_instance(4, __argv, rb_cTone);

	m_color_ptr = GetObjectPtr<RbColor>(color);
	m_tone_ptr = GetObjectPtr<RbTone>(tone);

	m_visible = Qtrue;

	//	创建并添加渲染结点
	m_node = RbRenderTree::AllocNode(RenderProc, obj, id_render_normal, 0, Qnil);
	RbRenderTree::InsertNode(m_node);

	m_disposed = false;

	return obj;
}

void RbViewport::render(u32 id)
{
	//	如果视口处于闪烁中且闪烁颜色为nil则直接返回
	if (m_flash_duration > 0 && m_flash_hide_spr > 0)
		return;

	//	根据viewport矩形进行裁剪
	GetRenderState()->ClipAndSave(m_rect_ptr->x, m_rect_ptr->y, m_rect_ptr->width, m_rect_ptr->height);

	//	渲染viewport中的所有对象
	for (RbRenderNode* p = m_head; p; p = p->next)
	{
		p->renderproc(p->value, p->id);
	}

	//	取消裁剪 还原屏幕大小
	GetRenderState()->ClipAndSave(0, 0, GetFrameWidth(), GetFrameHeight());

	//	视口内对象描绘完毕之后再处理视口的混色闪烁等

	// render it only the icolor isn't zero
	if (m_color_ptr->GetColor() != 0 || m_tone_ptr->GetColor() != 0 || m_flash_duration > 0)
	{
		m_quad.v[0].x = (float)m_rect_ptr->x;						m_quad.v[0].y = (float)m_rect_ptr->y;
		m_quad.v[1].x = (float)m_rect_ptr->x + m_rect_ptr->width;	m_quad.v[1].y = (float)m_rect_ptr->y;
		m_quad.v[2].x = (float)m_rect_ptr->x + m_rect_ptr->width;	m_quad.v[2].y = (float)m_rect_ptr->y + m_rect_ptr->height;
		m_quad.v[3].x = (float)m_rect_ptr->x;						m_quad.v[3].y = (float)m_rect_ptr->y + m_rect_ptr->height;
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
	
	// 使用减淡混合方式处理视口色调
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

VALUE RbViewport::get_rect()
{
	return ReturnObject(m_rect_ptr);
}

VALUE RbViewport::set_rect(VALUE rect)
{
	SafeRectValue(rect);
	m_rect_ptr = GetObjectPtr<RbRect>(rect);
	return Qnil;
}

VALUE RbViewport::get_color()
{
	return ReturnObject(m_color_ptr);
}

VALUE RbViewport::set_color(VALUE color)
{
	SafeColorValue(color);
	m_color_ptr = GetObjectPtr<RbColor>(color);
	return Qnil;
}

VALUE RbViewport::get_tone()
{
	return ReturnObject(m_tone_ptr);
}

VALUE RbViewport::set_tone(VALUE tone)
{
	SafeToneValue(tone);
	m_tone_ptr = GetObjectPtr<RbTone>(tone);
	return Qnil;
}

VALUE RbViewport::set_z(VALUE z)
{
	SafeFixnumValue(z);

	if (m_z != FIX2INT(z))
	{
		m_z = FIX2INT(z);
		m_node->z = m_z;
		RbRenderTree::InsertNode(RbRenderTree::DeleteNode(m_node));
	}

	return Qnil;
}

VALUE RbViewport::update()
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

VALUE RbViewport::flash(VALUE color, VALUE duration)
{
#pragma message("		Unfinished Function " __FUNCTION__)

	return Qnil;
}

/*
 *	以下定义ruby方法
 */
imp_method(RbViewport, update)
imp_method02(RbViewport, flash)
imp_attr_accessor(RbViewport, rect)
imp_attr_accessor(RbViewport, color)
imp_attr_accessor(RbViewport, tone)