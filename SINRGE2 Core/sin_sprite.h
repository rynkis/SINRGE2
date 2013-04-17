#ifndef SINSPRITE_H
#define SINSPRITE_H

#include "RbBitmap.h"

#include "hge.h"
#include "hgerect.h"

/*
** SIN Sprite class
*/
class SinSprite
{
public:
	SinSprite();
	~SinSprite() { hge->Release(); }
	
	
	void		Render(float x, float y);
	void		RenderEx(float x, float y, float rot, float hscale=1.0f, float vscale=0.0f);
	void		RenderStretch(float x1, float y1, float x2, float y2);
	void		Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);

	void		SetTexture(HTEXTURE tex);
	void		SetTextureRect(int x, int y, int w, int h);
	void		SetColor(DWORD col, int i=-1);
	void		SetZ(float z, int i=-1);
	void		SetBlendMode(int blend) { quad.blend=blend; }
	void		SetFlip(bool bX, bool bY, bool bHotSpot = false);

	HTEXTURE	GetTexture() const { return quad.tex; }
	void		GetTextureRect(float *x, float *y, float *w, float *h) const { *x=tx; *y=ty; *w=m_width; *h=m_height; }
	DWORD		GetColor(int i=0) const { return quad.v[i].col; }
	float		GetZ(int i=0) const { return quad.v[i].z; }
	int			GetBlendMode() const { return quad.blend; }

	float		GetWidth()											const { return m_width; }
	float		GetHeight()											const { return m_height; }
	hgeRect*	GetBoundingBoxEx(float x, float y, hgeRect *rect)	const;
	
	void		SetOX(float ox) { m_ox = ox; }
	void		SetOY(float oy) { m_oy = oy; }
	void		SetZoomX(float x) { m_zoom_x = x; }
	void		SetZoomY(float y) { m_zoom_y = y; }
	void		SetAngle(float rad) { m_angle = rad; }
	void		SetSrcRectDirty(bool v = true) { m_src_rect_dirty = v; }

	void		SetBlendColor(int color) { quad.blend_color=color; }	//	SINRGE2
	DWORD		GetBlendColor() const { return quad.blend_color; }		//	SINRGE2
	
	bool		GetSrcRectDirty()									const { return m_src_rect_dirty; }
	void		GetTextureRect(int *x, int *y, int *w, int *h)		const { *x=m_rx; *y=m_ry; *w=m_rw; *h=m_rh; }

	float		GetZoomX()											const { return m_zoom_x; }
	float		GetZoomY()											const { return m_zoom_y; }

	inline bool	IsFlipX()											const { return m_flip_x < 0.0f; }
	inline bool IsFlipY()											const { return m_flip_y < 0.0f; }

protected:
	static HGE	*hge;

	hgeQuad		quad;
	float		tx, ty;
	float		tex_width, tex_height;
	
	//	精灵位图的宽度和高度
	float		m_width, m_height;
	
	//	精灵的旋转、原点、缩放率以及翻转
	float		m_angle;
	float		m_ox, m_oy;
	float		m_zoom_x, m_zoom_y;
	float		m_flip_x, m_flip_y;

	//	是否翻转原点(ox, oy)
	bool		m_flip_hot_spot;

	//	精灵当前的src_rect
	int			m_rx, m_ry, m_rw, m_rh;

	//	精灵src_rect的藏标记
	bool		m_src_rect_dirty;

	//	精灵经过选择缩放后的四个顶点的坐标(不包括偏移)
	float		m_x1, m_y1, m_x2, m_y2, m_x3, m_y3, m_x4, m_y4;

};


#endif //SINSPRITE_H
