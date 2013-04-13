#ifndef __RGE_RENDER_TREE_H__
#define __RGE_RENDER_TREE_H__

#ifdef WIN32
#pragma once
#endif

#include "RbExport.h"
#include "sin_types.h"
#include "hge.h"

using namespace Sin;

class RenderState
{
public:
	typedef struct __RenderClipRect
	{
		int x;
		int y;
		int width;
		int height;

		__RenderClipRect() : x(0), y(0), width(0), height(0) {}

		void set(int _x, int _y, int _w, int _h)
		{
			x		= _x;
			y		= _y;
			width	= _w;
			height	= _h;
		}

	} RenderClipRect;

public:
	RenderState()
		: m_render2texture(false)
	{
		s_pHge = hgeCreate(HGE_VERSION);
	}

	~RenderState()
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

	inline const RenderClipRect& GetClipRect() const { return m_rect; }

	inline void	SetRenderToTexture(bool b)
	{
		m_render2texture = b;
	}

	inline bool	IsRenderToTexture() const
	{
		return m_render2texture;
	}

private:
	RenderClipRect				m_rect;
	bool						m_render2texture;

	static HGE*					s_pHge;
};

/*
 *	渲染函数指针
 */
typedef	void (*RbRenderProc)(VALUE, u32);

typedef struct __RenderNode
{
	RbRenderProc				renderproc;	//	渲染函数指针
	VALUE						value;		//	该结点对ruby对象的引用，比如Sprite、Window等。
	u32							id;			//	ID标识符，对于有的ruby对象，比如Window有两个渲染对象，一个边框，一个实际内容，这种情况下则用ID进行标示。
	s32							z;			//	该结点在渲染树中的z坐标值。该值越大的显示在越上面。
	VALUE						viewport;	//	结点所在视口，如果该值为Qnil的情况下，则结点直接在窗口中渲染，否则在Viewport中渲染。

	struct __RenderNode*		prev;		//	上一个结点指针。
	struct __RenderNode*		next;		//	下一个结点指针。
} RenderNode, *RenderNodePtr;

class RenderTree
{
public:
	static void				Init();

public:
	static bool				RenderProc();

public:
	static void				ViewportAddToFront(RenderNode* node);
	static void				ViewportDelete(RenderNode* node);

	static RenderNode*		AllocNode(RbRenderProc proc, VALUE value, u32 id, s32 z, VALUE viewport);

	static RenderNode*		DeleteNode(RenderNode *node);
	static void				InsertNode(RenderNode *node);

	static void				DestroyNode(RenderNodePtr* node);
	static void				FreeNode(RenderNodePtr* node);

private:
	static RenderNode*		DoubleLinkAddToFront(RenderNode* list, RenderNode* node);
	static RenderNode*		DoubleLinkDelete(RenderNode* list, RenderNode* node);

private:
	static RenderNode*		s_pRenderHead;
	static RenderNode*		s_pRenderTail;
	static RenderNode*		s_pViewportLists;
};

#endif	//	__RGE_RENDER_TREE_H__