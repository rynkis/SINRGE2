#ifndef __SIN_SPRITE_EX_H__
#define __SIN_SPRITE_EX_H__

#include "hge.h"
#include "hgerect.h"

#include "RbBitmap.h"

class RgeSpriteEx
{
public:
	RgeSpriteEx();
	~RgeSpriteEx();

	void		Render();
	void		Render(float x, float y);

	void		SetTexture(const bitmap_p pTex);
	void		SetTextureRect(int x, int y, int w, int h);
	void		SetColor(DWORD col, int i=-1);
	void		SetZ(float z, int i=-1);
	void		SetBlendMode(int blend) { for (u32 idx = 0; idx < m_quad_cnt; ++idx) m_quads[idx].quad.blend = blend; }
	void		SetBlendColor(int color) { for (u32 idx = 0; idx < m_quad_cnt; ++idx) m_quads[idx].quad.blend_color = color; }

	void		SetOX(float ox) { m_ox = ox; }
	void		SetOY(float oy) { m_oy = oy; }
	void		SetZoomX(float x) { m_zoom_x = x; }
	void		SetZoomY(float y) { m_zoom_y = y; }
	void		SetAngle(float rad) { m_angle = rad; }
	void		SetSrcRectDirty(bool v = true) { m_src_rect_dirty = v; }
	void		SetFlip(bool bX, bool bY, bool bHotSpot = false);

	const bitmap_p GetTexture()										const { return m_tex; }

	bool		GetSrcRectDirty()									const { return m_src_rect_dirty; }
	void		GetTextureRect(int *x, int *y, int *w, int *h)		const { *x=m_rx; *y=m_ry; *w=m_rw; *h=m_rh; }

	DWORD		GetColor(int i = 0)									const { return m_quads.quad.v[i].col; }
	float		GetZ(int i = 0)										const { return m_quads.quad.v[i].z; }
	int			GetBlendMode()										const { return m_quads.quad.blend; }
	DWORD		GetBlendColor()										const { return m_quads.quad.blend_color; }

	float		GetZoomX()											const { return m_zoom_x; }
	float		GetZoomY()											const { return m_zoom_y; }

	float		GetWidth()											const { return m_width; }
	float		GetHeight()											const { return m_height; }
	hgeRect*	GetBoundingBoxEx(float x, float y, hgeRect *rect)	const;

	inline bool	IsFlipX()											const { return m_flip_x < 0.0f; }
	inline bool IsFlipY()											const { return m_flip_y < 0.0f; }

protected:
	struct __RgeQuad
	{
		hgeQuad	quad;			//	小方块
		bool	render;			//	该小方块是否描绘
		float	x[4], y[4];		//	该小方块4个顶点的坐标
	};

	//	所有小四方形块的指针和块数目
	struct __RgeQuad			m_quads;
	u32							m_quad_cnt;

	//	精灵位图的宽度和高度
	float						m_width, m_height;
	
	//	SRgeTexture指针的引用
	/*const*/ bitmap_p				m_tex;
	
	//	精灵的旋转、原点、缩放率以及翻转
	float						m_angle;
	float						m_ox, m_oy;
	float						m_zoom_x, m_zoom_y;
	float						m_flip_x, m_flip_y;

	//	是否翻转原点(ox, oy)
	bool						m_flip_hot_spot;

	//	精灵当前的src_rect
	int							m_rx, m_ry, m_rw, m_rh;

	//	精灵src_rect的藏标记
	bool						m_src_rect_dirty;

	//	精灵经过选择缩放后的四个顶点的坐标(不包括偏移)
	float						m_x1, m_y1, m_x2, m_y2, m_x3, m_y3, m_x4, m_y4;

protected:
	static HGE*					s_pHge;
};


#endif	//	__SIN_SPRITE_EX_H__

