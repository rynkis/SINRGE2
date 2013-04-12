#include "RgeSpriteEx.h"
#include "sin_types.h"
#include <math.h>

HGE *RgeSpriteEx::s_pHge=0;

RgeSpriteEx::RgeSpriteEx()
	: /*m_quads(0), m_quad_cnt(0)
	,*/ m_width(0), m_height(0)
	, m_angle(0.0f)
	, m_tex(0)
	, m_src_rect_dirty(true)
	, m_ox(0), m_oy(0)
	, m_zoom_x(1.0f), m_zoom_y(1.0f)
	, m_flip_x(1.0f), m_flip_y(1.0f), m_flip_hot_spot(false)
	, m_rx(0), m_ry(0), m_rw(0), m_rh(0)
	, m_x1(0), m_y1(0), m_x2(0), m_y2(0), m_x3(0), m_y3(0), m_x4(0), m_y4(0)
{
	s_pHge = hgeCreate(HGE_VERSION);
}

RgeSpriteEx::~RgeSpriteEx()
{
	//SAFE_DELETE_ARRAY(m_quads);
	
	s_pHge->Release();
}

void RgeSpriteEx::Render()
{
	Render(0, 0);
}

void RgeSpriteEx::Render(float x, float y)
{
	/*for (u32 idx = 0; idx < m_quad_cnt; ++idx)
	{*/
		if (!m_quads.render)
			return;

		m_quads.quad.v[0].x = m_quads.x[0] + x;	m_quads.quad.v[0].y = m_quads.y[0] + y;
		m_quads.quad.v[1].x = m_quads.x[1] + x;	m_quads.quad.v[1].y = m_quads.y[1] + y;
		m_quads.quad.v[2].x = m_quads.x[2] + x;	m_quads.quad.v[2].y = m_quads.y[2] + y;
		m_quads.quad.v[3].x = m_quads.x[3] + x;	m_quads.quad.v[3].y = m_quads.y[3] + y;

		s_pHge->Gfx_RenderQuad(&m_quads.quad);
	//}
}

hgeRect* RgeSpriteEx::GetBoundingBoxEx(float x, float y, hgeRect *rect) const
{
	rect->Clear();

	rect->Encapsulate(m_x1 + x, m_y1 + y);
	rect->Encapsulate(m_x2 + x, m_y2 + y);
	rect->Encapsulate(m_x3 + x, m_y3 + y);
	rect->Encapsulate(m_x4 + x, m_y4 + y);

	return rect;
}

void RgeSpriteEx::SetFlip(bool bX, bool bY, bool bHotSpot)
{
	float ox = m_ox, oy = m_oy;

	if (m_flip_hot_spot && IsFlipX()) m_ox = m_width - m_ox;
	if (m_flip_hot_spot && IsFlipY()) m_oy = m_height - m_oy;

	m_flip_hot_spot = bHotSpot;
	
	if (m_flip_hot_spot && IsFlipX()) m_ox = m_width - m_ox;
	if (m_flip_hot_spot && IsFlipY()) m_oy = m_height - m_oy;

	bool dirty = (ox != m_ox || oy != m_oy);

	if (IsFlipX() != bX)
	{
		m_flip_x *= -1.0f;
		dirty = true;
	}
	if (IsFlipY() != bY)
	{
		m_flip_y *= -1.0f;
		dirty = true;
	}
	if (dirty)
	{
		SetSrcRectDirty();
	}
}

/**
 *	更新纹理矩形并相关坐标位置
 *
 *	其中 mirror、angle、zoom_x、zoom_y、ox、oy、src_rect、bitmap 等属性的改变都应该调用该方法进行更新 (bush_depth?)
 */
void RgeSpriteEx::SetTextureRect(int x, int y, int w, int h)
{
	//	检测src_rect是否改变
	if (x != m_rx || y != m_ry || w != m_rw || h != m_rh)
	{
		m_rx	= x;
		m_ry	= y;
		m_rw	= w;
		m_rh	= h;

		SetSrcRectDirty();
	}

	//	如果源矩形没脏则直接返回
	if (!m_src_rect_dirty)
		return;

	if (!m_tex->quad.tex/* || !m_tex->sub_textures*/)
		return;

	float left		= x;
	float right		= x + w;
	float top		= y;
	float bottom	= y + h;

	if (w <= 0 || h <= 0)
		return;

	if (right < 0 || left > m_width || bottom < 0 || top > m_height)
		return;

	if (left < 0)			left	= 0;
	if (right > m_width)	right	= m_width;
	if (top < 0)			top		= 0;
	if (bottom > m_height)	bottom	= m_height;

	x		= left;
	y		= top;
	int width	= right - left;
	int height	= bottom - top;

	int	xBgn	= x / m_tex->width;//sub_textures[0].width;
	int yBgn	= y / m_tex->height;//sub_textures[0].height;

	int	xEnd	= (x + width - 1) / m_tex->width;//sub_textures[0].width;
	int yEnd	= (y + height - 1) / m_tex->height;//sub_textures[0].height;

	//for (u32 idx = 0; idx < m_quad_cnt; ++idx)
		m_quads.render = false;

	//	translation、scaling、rotation
	float fcos = cosf(m_angle);
	float fsin = sinf(m_angle);

#define __TRANSLATION_SCALING_X(mx)	(((mx) - m_ox) * m_zoom_x * m_flip_x)
#define __TRANSLATION_SCALING_Y(my)	(((my) - m_oy) * m_zoom_y * m_flip_y)
#define __ROTATION_X(__x, __y)		((float)(((__x) * fcos - (__y) * fsin /*+ m_x*/)))
#define __ROTATION_Y(__x, __y)		((float)(((__x) * fsin + (__y) * fcos /*+ m_y*/)))

	//int	index;

	float tmp_x1, tmp_x2, tmp_y1, tmp_y2;
	float fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;

	for (int yidx = yBgn, cost_h = 0; yidx <= yEnd; ++yidx)
	{
		int h = SinMin(SinMin(height - cost_h, m_tex->height/*sub_textures[0].height*/), m_tex->height/*sub_textures[0].height*/ - y);

		for (int xidx = xBgn, cost_w = 0, sx = x % m_tex->height/*sub_textures[0].width*/; xidx <= xEnd; ++xidx)
		{
			int w = SinMin(SinMin(width - cost_w, m_tex->width/*sub_textures[0].width*/), m_tex->width/*sub_textures[0].width*/ - sx);

	//		index = yidx * m_tex->width/*cols*/ + xidx;

			{
				float tx1, ty1, tx2, ty2;

				tx1 = (float)sx / m_tex->width/*sub_textures[0].width*/;			ty1 = (float)y / m_tex->height/*sub_textures[0].height*/;
				tx2 = (float)(sx + w) / m_tex->width/*sub_textures[0].width*/;	ty2 = (float)(y + h) / m_tex->height/*sub_textures[0].height*/;

				m_quads.quad.v[0].tx=tx1; m_quads.quad.v[0].ty=ty1; 
				m_quads.quad.v[1].tx=tx2; m_quads.quad.v[1].ty=ty1; 
				m_quads.quad.v[2].tx=tx2; m_quads.quad.v[2].ty=ty2; 
				m_quads.quad.v[3].tx=tx1; m_quads.quad.v[3].ty=ty2;
			}

			tmp_x1	= __TRANSLATION_SCALING_X(cost_w);
			tmp_y1	= __TRANSLATION_SCALING_Y(cost_h);
			tmp_x2	= __TRANSLATION_SCALING_X(cost_w + w);
			tmp_y2	= __TRANSLATION_SCALING_Y(cost_h + h);

			fx1	= __ROTATION_X(tmp_x1, tmp_y1);
			fy1 = __ROTATION_Y(tmp_x1, tmp_y1);
			fx2 = __ROTATION_X(tmp_x2, tmp_y1);
			fy2 = __ROTATION_Y(tmp_x2, tmp_y1);
			fx3 = __ROTATION_X(tmp_x2, tmp_y2);
			fy3 = __ROTATION_Y(tmp_x2, tmp_y2);
			fx4 = __ROTATION_X(tmp_x1, tmp_y2);
			fy4 = __ROTATION_Y(tmp_x1, tmp_y2);

			if (xidx != xBgn)
			{
				fx1 = m_quads.x[1]; fy1 = m_quads.y[1];
				fx4 = m_quads.x[2]; fy4 = m_quads.y[2];
			}

			if (yidx != yBgn)
			{
				//int index2 = (yidx - 1) * m_tex->cols + xidx;

				fx2 = m_quads.x[2]; fy2 = m_quads.y[2];
				fx1 = m_quads.x[3]; fy1 = m_quads.y[3];
			}

			if (yidx == yBgn)
			{
				if (xidx == xBgn) { m_x1 = fx1; m_y1 = fy1; }
				if (xidx == xEnd) { m_x2 = fx2; m_y2 = fy2; }
			}
			if (yidx == yEnd)
			{
				if (xidx == xEnd) { m_x3 = fx3; m_y3 = fy3; }
				if (xidx == xBgn) { m_x4 = fx4; m_y4 = fy4; }
			}

			m_quads.x[0] = fx1; m_quads.y[0] = fy1;
			m_quads.x[1] = fx2; m_quads.y[1] = fy2;
			m_quads.x[2] = fx3; m_quads.y[2] = fy3;
			m_quads.x[3] = fx4; m_quads.y[3] = fy4;

			m_quads.render = true;

			cost_w += w;
			sx = 0;
		}

		cost_h += h;
		y = 0;
	}

#undef __ROTATION_Y
#undef __ROTATION_X
#undef __TRANSLATION_SCALING_Y
#undef __TRANSLATION_SCALING_X

	//	取消藏标记
	m_src_rect_dirty = false;
}

void RgeSpriteEx::SetTexture(const bitmap_p pBmp)
{
	if (!pBmp->quad.tex/*tex || !tex->sub_textures*/)
		return;

	//u32 cnt = tex->GetSubTextureCount();

	//if (cnt != m_quad_cnt)
	//{
	//	//m_quad_cnt = cnt;

	//	SAFE_DELETE_ARRAY(m_quads);

	//	if (m_quad_cnt)
	//		m_quads = new struct __RgeQuad[m_quad_cnt];
	//}

	m_tex = pBmp;

	m_width = pBmp->width;//tex->width;
	m_height = pBmp->height;//tex->height;

	/*for (u32 idx = 0; idx < m_quad_cnt; ++idx)
	{*/
		m_quads.render = false;

		m_quads.quad.tex = pBmp->quad.tex;//tex->sub_textures[idx].tex;

		//	纹理的宽度和高度很等于textures[0]的宽度和高度
		//tex_width = pTex->textures[0].width;	
		//tex_height = pTex->textures[0].height;

		m_quads.quad.v[0].z = 
		m_quads.quad.v[1].z = 
		m_quads.quad.v[2].z = 
		m_quads.quad.v[3].z = 0.5f;
		
		m_quads.quad.v[0].col = 
		m_quads.quad.v[1].col = 
		m_quads.quad.v[2].col = 
		m_quads.quad.v[3].col = 0xffffffff;

		m_quads.quad.blend = BLEND_DEFAULT;

		m_quads.quad.blend_color = 0x00000000;
	//}
}

void RgeSpriteEx::SetColor(DWORD col, int i)
{
	if(i != -1)
	{
		//for (u32 idx = 0; idx < m_quad_cnt; ++idx)
			m_quads.quad.v[i].col = col;
	}
	else
	{
		/*for (u32 idx = 0; idx < m_quad_cnt; ++idx)
		{*/
			m_quads.quad.v[0].col = 
			m_quads.quad.v[1].col = 
			m_quads.quad.v[2].col = 
			m_quads.quad.v[3].col = col;
		//}
	}
}

void RgeSpriteEx::SetZ(float z, int i)
{
	if(i != -1)
	{
		//for (u32 idx = 0; idx < m_quad_cnt; ++idx)
			m_quads.quad.v[i].z = z;
	}
	else
	{
		/*for (u32 idx = 0; idx < m_quad_cnt; ++idx)
		{*/
			m_quads.quad.v[0].z = 
			m_quads.quad.v[1].z = 
			m_quads.quad.v[2].z = 
			m_quads.quad.v[3].z = z;
		//}
	}
}
