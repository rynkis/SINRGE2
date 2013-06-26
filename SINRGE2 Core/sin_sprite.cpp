/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Class Sprite
*/
#include "sin_sprite.h"
#include <math.h>

HGE * SinSprite::hge = 0;

SinSprite::SinSprite()
	: m_width(0), m_height(0)
	, m_angle(0.0f)
	, m_src_rect_dirty(true)
	, m_ox(0), m_oy(0)
	, m_zoom_x(1.0f), m_zoom_y(1.0f)
	, m_flip_x(1.0f), m_flip_y(1.0f), m_flip_hot_spot(false)
	, m_rx(0), m_ry(0), m_rw(0), m_rh(0)
	, m_x1(0), m_y1(0), m_x2(0), m_y2(0), m_x3(0), m_y3(0), m_x4(0), m_y4(0)
{
	hge=hgeCreate(HGE_VERSION);
	
	quad.tex=0;

	quad.v[0].z = 
	quad.v[1].z = 
	quad.v[2].z = 
	quad.v[3].z = 0.5f;
	
	quad.v[0].col = 
	quad.v[1].col = 
	quad.v[2].col = 
	quad.v[3].col = 0xffffffff;

	quad.blend=BLEND_DEFAULT;

	quad.blend_color = 0x00000000;	//	SINRGE2
}

void SinSprite::Render(float x, float y)
{
	if (m_zoom_x == 0 || m_zoom_y == 0) return;

	quad.v[0].x = m_x1 + x; quad.v[0].y = m_y1 + y;
	quad.v[1].x = m_x2 + x; quad.v[1].y = m_y2 + y;
	quad.v[2].x = m_x3 + x; quad.v[2].y = m_y3 + y;
	quad.v[3].x = m_x4 + x; quad.v[3].y = m_y4 + y;

	hge->Gfx_RenderQuad(&quad);
}

void SinSprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
	float tx1, ty1, tx2, ty2;
	float sint, cost;

	if(vscale==0) vscale=hscale;

	tx1 = -m_ox*hscale;
	ty1 = -m_oy*vscale;
	tx2 = (m_width-m_ox)*hscale;
	ty2 = (m_height-m_oy)*vscale;

	if (rot != 0.0f)
	{
		cost = cosf(rot);
		sint = sinf(rot);
			
		quad.v[0].x  = tx1*cost - ty1*sint + x;
		quad.v[0].y  = tx1*sint + ty1*cost + y;	

		quad.v[1].x  = tx2*cost - ty1*sint + x;
		quad.v[1].y  = tx2*sint + ty1*cost + y;	

		quad.v[2].x  = tx2*cost - ty2*sint + x;
		quad.v[2].y  = tx2*sint + ty2*cost + y;	

		quad.v[3].x  = tx1*cost - ty2*sint + x;
		quad.v[3].y  = tx1*sint + ty2*cost + y;	
	}
	else
	{
		quad.v[0].x = tx1 + x; quad.v[0].y = ty1 + y;
		quad.v[1].x = tx2 + x; quad.v[1].y = ty1 + y;
		quad.v[2].x = tx2 + x; quad.v[2].y = ty2 + y;
		quad.v[3].x = tx1 + x; quad.v[3].y = ty2 + y;
	}

	hge->Gfx_RenderQuad(&quad);
}

void SinSprite::RenderStretch(float x1, float y1, float x2, float y2)
{
	quad.v[0].x = x1; quad.v[0].y = y1;
	quad.v[1].x = x2; quad.v[1].y = y1;
	quad.v[2].x = x2; quad.v[2].y = y2;
	quad.v[3].x = x1; quad.v[3].y = y2;

	hge->Gfx_RenderQuad(&quad);
}

void SinSprite::Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	quad.v[0].x = x0; quad.v[0].y = y0;
	quad.v[1].x = x1; quad.v[1].y = y1;
	quad.v[2].x = x2; quad.v[2].y = y2;
	quad.v[3].x = x3; quad.v[3].y = y3;

	hge->Gfx_RenderQuad(&quad);
}

hgeRect * SinSprite::GetBoundingBoxEx(float x, float y, hgeRect *rect) const
{
	rect->Clear();

	rect->Encapsulate(m_x1 + x, m_y1 + y);
	rect->Encapsulate(m_x2 + x, m_y2 + y);
	rect->Encapsulate(m_x3 + x, m_y3 + y);
	rect->Encapsulate(m_x4 + x, m_y4 + y);

	return rect;
}

void SinSprite::SetFlip(bool bX, bool bY, bool bHotSpot)
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

void SinSprite::SetTexture(HTEXTURE tex)
{
	quad.tex = tex;

	if (tex)
	{
		m_width = (float)hge->Texture_GetWidth(tex);
		m_height = (float)hge->Texture_GetHeight(tex);
	}
	else
	{
		m_width = 1.0f;
		m_height = 1.0f;
	}
}

void SinSprite::SetTextureRect(int x, int y, int w, int h)
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

	float tx1, ty1, tx2, ty2, sint, cost;
	
	tx1 = x / m_width;
	ty1 = y / m_height;
	tx2 = (x + w) / m_width;
	ty2 = (y + h) / m_height;

	quad.v[0].tx = tx1; quad.v[0].ty = ty1;
	quad.v[1].tx = tx2; quad.v[1].ty = ty1;
	quad.v[2].tx = tx2; quad.v[2].ty = ty2;
	quad.v[3].tx = tx1; quad.v[3].ty = ty2;

	//if(m_zoom_y==0) m_zoom_y=m_zoom_x;
	if (m_zoom_y == 0) return;

	tx1 = -m_ox * m_zoom_x * m_flip_x;
	ty1 = -m_oy * m_zoom_y * m_flip_y;
	tx2 = (m_rw-m_ox) * m_zoom_x * m_flip_x;
	ty2 = (m_rh-m_oy) * m_zoom_y * m_flip_y;

	if (m_angle != 0.0f)
	{
		cost = cosf(m_angle);
		sint = sinf(m_angle);
		
		quad.v[0].x  = tx1*cost - ty1*sint;
		quad.v[0].y  = tx1*sint + ty1*cost;

		quad.v[1].x  = tx2*cost - ty1*sint;
		quad.v[1].y  = tx2*sint + ty1*cost;

		quad.v[2].x  = tx2*cost - ty2*sint;
		quad.v[2].y  = tx2*sint + ty2*cost;

		quad.v[3].x  = tx1*cost - ty2*sint;
		quad.v[3].y  = tx1*sint + ty2*cost;
	}
	else
	{
		quad.v[0].x = tx1; quad.v[0].y = ty1;
		quad.v[1].x = tx2; quad.v[1].y = ty1;
		quad.v[2].x = tx2; quad.v[2].y = ty2;
		quad.v[3].x = tx1; quad.v[3].y = ty2;
	}
	
	m_x1 = quad.v[0].x; m_y1 = quad.v[0].y;
	m_x2 = quad.v[1].x; m_y2 = quad.v[1].y;
	m_x3 = quad.v[2].x; m_y3 = quad.v[2].y;
	m_x4 = quad.v[3].x; m_y4 = quad.v[3].y;

	//	取消脏标记
	m_src_rect_dirty = false;
}

void SinSprite::SetColor(u32 col, int i)
{
	if(i != -1)
		quad.v[i].col = col;
	else
		quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = col;
}

void SinSprite::SetZ(float z, int i)
{
	if(i != -1)
		quad.v[i].z = z;
	else
		quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = z;
}
