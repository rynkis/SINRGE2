/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Cpp Struct CRbRenderTree
*/
#ifndef __C_RB_RENDER_TREE_H__
#define __C_RB_RENDER_TREE_H__

#ifdef WIN32
#pragma once
#endif

#include "RbExport.h"
#include "hge.h"

class CRbRenderState
{
public:
	typedef struct __RbRenderClipRect
	{
		int x;
		int y;
		int width;
		int height;

		__RbRenderClipRect() : x(0), y(0), width(0), height(0) {}

		void set(int _x, int _y, int _w, int _h)
		{
			x		= _x;
			y		= _y;
			width	= _w;
			height	= _h;
		}

	} RbRenderClipRect;

public:
	CRbRenderState()
		//: m_render2texture(false)
	{
		s_pHge = hgeCreate(HGE_VERSION);
	}

	~CRbRenderState()
	{
		s_pHge->Release();
	}

public:
	inline void	Clip(int x, int y, int w, int h)
	{
		s_pHge->Gfx_SetClipping(x, y, w, h);
	}

	inline void	Restore()
	{
		s_pHge->Gfx_SetClipping(m_rect.x, m_rect.y, m_rect.width, m_rect.height);
	}

	inline void	Save(int x, int y, int w, int h)
	{
		m_rect.set(x, y, w, h);
	}

	inline void	ClipAndSave(int x, int y, int w, int h)
	{
		Save(x, y, w, h);
		Clip(x, y, w, h);
	}

	inline const RbRenderClipRect& GetClipRect() const { return m_rect; }

	/*inline void	SetRenderToTexture(bool b)
	{
		m_render2texture = b;
	}*/

	/*inline bool	IsRenderToTexture() const
	{
		return m_render2texture;
	}*/

private:
	RbRenderClipRect			m_rect;
	//bool						m_render2texture;

	static HGE *				s_pHge;
};

/*
 *	渲染函数指针
 */
typedef	void (*RbRenderProc)(VALUE, u32);

typedef struct __RbRenderNode
{
	RbRenderProc				renderproc;	//	渲染函数指针
	VALUE						value;		//	该结点对ruby对象的引用，比如Sprite、Window等。
	u32							id;			//	ID标识符，对于有的ruby对象，比如Window有两个渲染对象，一个边框，一个实际内容，这种情况下则用ID进行标示。
	s32							z;			//	该结点在渲染树中的z坐标值。该值越大的显示在越上面。
	VALUE						viewport;	//	结点所在视口，如果该值为Qnil的情况下，则结点直接在窗口中渲染，否则在Viewport中渲染。

	struct __RbRenderNode *		prev;		//	上一个结点指针。
	struct __RbRenderNode *		next;		//	下一个结点指针。
} RbRenderNode, *RbRenderNodePtr;

class CRbRenderTree
{
public:
	static void					Init();

public:
	static bool					RenderProc();

public:
	/*static void					ViewportAddToFront(RbRenderNode* node);
	static void					ViewportDelete(RbRenderNode* node);*/

	static RbRenderNode*		AllocNode(RbRenderProc proc, VALUE value, u32 id, s32 z, VALUE viewport);

	static RbRenderNode*		DeleteNode(RbRenderNode * node);
	static void					InsertNode(RbRenderNode * node);

	static void					DestroyNode(RbRenderNodePtr * node);
	static void					FreeNode(RbRenderNodePtr * node);

private:
	static RbRenderNode *		DoubleLinkAddToFront(RbRenderNode * list, RbRenderNode * node);
	static RbRenderNode *		DoubleLinkDelete(RbRenderNode * list, RbRenderNode * node);

private:
	static RbRenderNode *		s_pRenderHead;
	static RbRenderNode *		s_pRenderTail;
	//static RbRenderNode*		s_pViewportLists;
};

#endif	//	__C_RB_RENDER_TREE_H__