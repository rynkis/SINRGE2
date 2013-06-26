/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class Charmap
*/
#include "RbExport.h"
#include "CRbViewport.h"
#include "CRbBitmap.h"
#include "CRbSprite.h"
#include "CRbRect.h"
#include "CRbCharmap.h"
#include "sin_color.h"
#include "sin_sprite.h"
#include "sin_app.h"

VALUE rb_cCharmap;

CRbCharmap::CRbCharmap()
	: m_node(0)
	, m_bitmap_ptr(0)
	, m_width(0), m_height(0), m_charsize(0), m_charmap_size(0)
	, charmap(0)
{
	m_pSpr = new SinSprite();
}

CRbCharmap::~CRbCharmap()
{
	if (charmap)
	{
		free(charmap);
		charmap = NULL;
	}
	SAFE_DELETE(m_pSpr);
	CRbRenderTree::DestroyNode(&m_node);
}

void CRbCharmap::InitLibrary()
{
	rb_cCharmap = rb_define_class_under(rb_mSin, "Charmap", rb_cObject);

	// special method
	rb_define_alloc_func(rb_cCharmap, ObjAllocate<CRbCharmap>);
	rb_define_method(rb_cCharmap, "initialize",	(RbFunc)dm_initialize,		-1);

	// instance method
	rb_define_method(rb_cCharmap, "dispose",		(RbFunc)dm_dispose,			0);
	rb_define_method(rb_cCharmap, "disposed?",		(RbFunc)dm_is_disposed,		0);

	rb_define_method(rb_cCharmap, "reset",			(RbFunc)dm_reset,			-1);

	// object attribute
	/*rb_define_method(rb_cCharmap, "viewport",		(RbFunc)dm_get_viewport,	0);
	rb_define_method(rb_cCharmap, "viewport=",		(RbFunc)dm_set_viewport,	1);*/

	// object attribute
	rb_define_method(rb_cCharmap, "bitmap",			(RbFunc)dm_get_bitmap,		0);
	rb_define_method(rb_cCharmap, "bitmap=",		(RbFunc)dm_set_bitmap,		1);
	rb_define_method(rb_cCharmap, "visible",		(RbFunc)dm_get_visible,		0);
	rb_define_method(rb_cCharmap, "visible=",		(RbFunc)dm_set_visible,		1);
	rb_define_method(rb_cCharmap, "z",				(RbFunc)dm_get_z,			0);
	rb_define_method(rb_cCharmap, "z=",				(RbFunc)dm_set_z,			1);
	/*rb_define_method(rb_cCharmap, "ox",				(RbFunc)dm_get_ox,			0);
	rb_define_method(rb_cCharmap, "ox=",			(RbFunc)dm_set_ox,			1);
	rb_define_method(rb_cCharmap, "oy",				(RbFunc)dm_get_oy,			0);
	rb_define_method(rb_cCharmap, "oy=",			(RbFunc)dm_set_oy,			1);*/

	// supplement
 	rb_define_method(rb_cCharmap, "to_s",			(RbFunc)dm_to_string,		0);
}

void CRbCharmap::mark()
{
	if (m_viewport_ptr) m_viewport_ptr->MarkObject();
	if (m_bitmap_ptr)	m_bitmap_ptr->MarkObject();
}

VALUE CRbCharmap::initialize(int argc, VALUE * argv, VALUE obj)
{
	VALUE width, height, size;

	//	检查参数
	rb_scan_args(argc, argv, "30", &width, &height, &size);
	
	SafeFixnumValue(width);
	SafeFixnumValue(height);
	SafeFixnumValue(size);
	
	m_width = FIX2INT(width);
	m_height = FIX2INT(height);
	m_charsize = FIX2INT(size);

	m_charmap_size = m_width * m_height + m_height;
	charmap = (char *)malloc(m_charmap_size * sizeof(char));

	/*if (!NIL_P(viewport))
	{
		SafeViewportValue(viewport);
		m_viewport_ptr = GetObjectPtr<CRbViewport>(viewport);
	}*/

	//	创建并添加渲染结点
	m_node = CRbRenderTree::AllocNode(RenderProc, obj, 0, 0, Qnil);
	CRbRenderTree::InsertNode(m_node);

	m_visible = Qtrue;

	//	取消释放标记
	m_disposed = false;

	return obj;
}

/*
 *	渲染（描绘）
 */
void CRbCharmap::render(u32 id)
{
	//	有效性检查
	if (!m_bitmap_ptr || !charmap)
		return;

	if (m_bitmap_ptr->IsDisposed())
		return;

	int lx = 0, ly = 0;
	for (int i = 0; i < m_charmap_size; ++i)
	{
		char idx = charmap[i];
		if (idx < 0)
		{
			lx = 0;
			ly += m_charsize;
			continue;
		}
		m_pSpr->SetTextureRect(idx * m_charsize, 0, m_charsize, m_charsize);
		m_pSpr->Render(lx, ly);
		lx += m_charsize;
	}
}

VALUE CRbCharmap::reset(int argc, VALUE * argv, VALUE obj)
{
	check_raise();

	VALUE bitmap, vgray;
	rb_scan_args(argc, argv, "11", &bitmap, &vgray);

	SafeBitmapValue(bitmap);
	CRbBitmap * bmp_ptr = GetObjectPtr<CRbBitmap>(bitmap);

	if (bmp_ptr->GetBitmapPtr()->width != m_width || bmp_ptr->GetBitmapPtr()->height != m_height)
		rb_raise(rb_eSinError, "wrong size of bitmap: %d * %d.\ncharmap is: %d * %d",
			bmp_ptr->GetBitmapPtr()->width, bmp_ptr->GetBitmapPtr()->height, m_width, m_height);

	bool gray = RTEST(vgray);
	int count = m_bitmap_ptr->GetWidth() / m_charsize;
	int span = 0xFF / count;
	
	HGE * hge = GetAppPtr()->GetHgePtr();
	BYTE a, r, g, b;
	long lx, ly, lineno, offset, cidx;
	u32 * pTexData = GetAppPtr()->GetHgePtr()->Texture_Lock(bmp_ptr->GetBitmapPtr()->quad.tex, true);
	for (ly = 0; ly < bmp_ptr->GetBitmapPtr()->height; ++ly)
	{
		lineno = bmp_ptr->GetBitmapPtr()->width * ly;
		for (lx = 0; lx < bmp_ptr->GetBitmapPtr()->width; ++lx)
		{
			offset = lineno + lx;
			GET_ARGB_8888(pTexData[offset], a, r, g, b);
			cidx = gray ? r : (r + g + b) / 3;//(r * 38 + g * 75 + b * 15 ) >> 7;//
			cidx /= span;
			cidx = SinBound(cidx, 0, count - 1);
			charmap[offset + ly] = cidx;
		}
		charmap[offset + ly + 1] = -1;
	}
	GetAppPtr()->GetHgePtr()->Texture_Unlock(bmp_ptr->GetBitmapPtr()->quad.tex);
	bmp_ptr = NULL;
	return Qnil;
}

VALUE CRbCharmap::dispose()
{
	if (m_disposed)
		return Qnil;
	if (charmap)
	{
		free(charmap);
		charmap = NULL;
	}
	m_bitmap_ptr = NULL;
	m_disposed = true;
	return Qnil;
}

VALUE CRbCharmap::is_disposed()
{
	return C2RbBool(m_disposed);
}

void CRbCharmap::check_raise()
{
	if (m_disposed)
		rb_raise(rb_eSinError, "disposed charmap");
}

VALUE CRbCharmap::get_bitmap()
{
	check_raise();
	return ReturnObject(m_bitmap_ptr);
}

VALUE CRbCharmap::set_bitmap(VALUE bitmap)
{
	check_raise();

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

VALUE CRbCharmap::get_z()
{
	check_raise();
	return INT2FIX(m_z);
}

VALUE CRbCharmap::set_z(VALUE z)
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


imp_method(CRbCharmap, dispose)
imp_method(CRbCharmap, is_disposed)

imp_method_vargs(CRbCharmap, reset)

imp_attr_accessor(CRbCharmap, bitmap)
