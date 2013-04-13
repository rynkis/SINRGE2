#include "SinSprite.h"
#include <math.h>


HGE *SinSprite::hge=0;


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
	
	hotX=0;
	hotY=0;
	bXFlip=false;
	bYFlip=false;
	bHSFlip=false;
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
	float tempx1, tempy1, tempx2, tempy2;

	tempx1 = x-hotX;
	tempy1 = y-hotY;
	tempx2 = x+m_width-hotX;
	tempy2 = y+m_height-hotY;

	quad.v[0].x = tempx1; quad.v[0].y = tempy1;
	quad.v[1].x = tempx2; quad.v[1].y = tempy1;
	quad.v[2].x = tempx2; quad.v[2].y = tempy2;
	quad.v[3].x = tempx1; quad.v[3].y = tempy2;

	hge->Gfx_RenderQuad(&quad);
}


void SinSprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
	float tx1, ty1, tx2, ty2;
	float sint, cost;

	if(vscale==0) vscale=hscale;

	tx1 = -hotX*hscale;
	ty1 = -hotY*vscale;
	tx2 = (m_width-hotX)*hscale;
	ty2 = (m_height-hotY)*vscale;

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


hgeRect* SinSprite::GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale, hgeRect *rect) const
{
	float tx1, ty1, tx2, ty2;
	float sint, cost;

	rect->Clear();
	
	tx1 = -hotX*hscale;
	ty1 = -hotY*vscale;
	tx2 = (m_width-hotX)*hscale;
	ty2 = (m_height-hotY)*vscale;

	if (rot != 0.0f)
	{
		cost = cosf(rot);
		sint = sinf(rot);
			
		rect->Encapsulate(tx1*cost - ty1*sint + x, tx1*sint + ty1*cost + y);	
		rect->Encapsulate(tx2*cost - ty1*sint + x, tx2*sint + ty1*cost + y);	
		rect->Encapsulate(tx2*cost - ty2*sint + x, tx2*sint + ty2*cost + y);	
		rect->Encapsulate(tx1*cost - ty2*sint + x, tx1*sint + ty2*cost + y);	
	}
	else
	{
		rect->Encapsulate(tx1 + x, ty1 + y);
		rect->Encapsulate(tx2 + x, ty1 + y);
		rect->Encapsulate(tx2 + x, ty2 + y);
		rect->Encapsulate(tx1 + x, ty2 + y);
	}

	return rect;
}

void SinSprite::SetFlip(bool bX, bool bY, bool bHotSpot)
{
	float tx, ty;

	if(bHSFlip && bXFlip) hotX = m_width - hotX;
	if(bHSFlip && bYFlip) hotY = m_height - hotY;

	bHSFlip = bHotSpot;
	
	if(bHSFlip && bXFlip) hotX = m_width - hotX;
	if(bHSFlip && bYFlip) hotY = m_height - hotY;

	if(bX != bXFlip)
	{
		tx=quad.v[0].tx; quad.v[0].tx=quad.v[1].tx; quad.v[1].tx=tx;
		ty=quad.v[0].ty; quad.v[0].ty=quad.v[1].ty; quad.v[1].ty=ty;
		tx=quad.v[3].tx; quad.v[3].tx=quad.v[2].tx; quad.v[2].tx=tx;
		ty=quad.v[3].ty; quad.v[3].ty=quad.v[2].ty; quad.v[2].ty=ty;

		bXFlip=!bXFlip;
	}

	if(bY != bYFlip)
	{
		tx=quad.v[0].tx; quad.v[0].tx=quad.v[3].tx; quad.v[3].tx=tx;
		ty=quad.v[0].ty; quad.v[0].ty=quad.v[3].ty; quad.v[3].ty=ty;
		tx=quad.v[1].tx; quad.v[1].tx=quad.v[2].tx; quad.v[2].tx=tx;
		ty=quad.v[1].ty; quad.v[1].ty=quad.v[2].ty; quad.v[2].ty=ty;

		bYFlip=!bYFlip;
	}
}


void SinSprite::SetTexture(HTEXTURE tex)
{
	float tx1,ty1,tx2,ty2;
	float tw,th;

	quad.tex=tex;

	if(tex)
	{
		tw = (float)hge->Texture_GetWidth(tex);
		th = (float)hge->Texture_GetHeight(tex);
	}
	else
	{
		tw = 1.0f;
		th = 1.0f;
	}

	if(tw!=tex_width || th!=tex_height)
	{
		tx1=quad.v[0].tx*tex_width;
		ty1=quad.v[0].ty*tex_height;
		tx2=quad.v[2].tx*tex_width;
		ty2=quad.v[2].ty*tex_height;

		tex_width=tw;
		tex_height=th;

		tx1/=tw; ty1/=th;
		tx2/=tw; ty2/=th;

		quad.v[0].tx=tx1; quad.v[0].ty=ty1; 
		quad.v[1].tx=tx2; quad.v[1].ty=ty1; 
		quad.v[2].tx=tx2; quad.v[2].ty=ty2; 
		quad.v[3].tx=tx1; quad.v[3].ty=ty2; 
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

	float tx1, ty1, tx2, ty2;
	bool bX,bY,bHS;

	tx=x;
	ty=y;
	
	/*if(adjSize)
	{*/
		m_width=w;
		m_height=h;
	//}

	tx1=tx/tex_width; ty1=ty/tex_height;
	tx2=(tx+w)/tex_width; ty2=(ty+h)/tex_height;

	quad.v[0].tx=tx1; quad.v[0].ty=ty1; 
	quad.v[1].tx=tx2; quad.v[1].ty=ty1; 
	quad.v[2].tx=tx2; quad.v[2].ty=ty2; 
	quad.v[3].tx=tx1; quad.v[3].ty=ty2; 

	bX=bXFlip; bY=bYFlip; bHS=bHSFlip;
	bXFlip=false; bYFlip=false;
	SetFlip(bX,bY,bHS);
}


void SinSprite::SetColor(DWORD col, int i)
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
