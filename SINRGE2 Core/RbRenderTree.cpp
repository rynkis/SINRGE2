#include "RbRenderTree.h"
#include "RbViewport.h"
#include "SINRGE2.h"

using namespace Sin;

HGE* RenderState::s_pHge = 0;

RenderNode*	RenderTree::s_pViewportLists		= 0;
RenderNode*	RenderTree::s_pRenderHead		= 0;
RenderNode*	RenderTree::s_pRenderTail		= 0;

void RenderTree::Init()
{
}

/**
 *	渲染函数
 */
bool RenderTree::RenderProc()
{
	if(!NIL_P(rb_errinfo()))
		return true;

	RenderNode*	p1;

	for (p1 = s_pViewportLists; p1; p1 = p1->next)
	{
		p1->renderproc(p1->value, p1->id);
	}

	HGE* pHGE = GetHgePtr();

	pHGE->Gfx_BeginScene();
	pHGE->Gfx_Clear(0);

	for (p1 = s_pRenderHead; p1; p1 = p1->next)
	{
		p1->renderproc(p1->value, p1->id);
	}

	pHGE->Gfx_SetClipping();
	pHGE->Gfx_EndScene();

	return false;
}

void RenderTree::ViewportAddToFront(RenderNode* node)
{
	s_pViewportLists = DoubleLinkAddToFront(s_pViewportLists, node);
}

void RenderTree::ViewportDelete(RenderNode* node)
{
	s_pViewportLists = DoubleLinkDelete(s_pViewportLists, node);
}

RenderNode*	RenderTree::AllocNode(RbRenderProc proc, VALUE value, u32 id, s32 z, VALUE viewport)
{
	RenderNode* node	= ALLOC(RenderNode);
	{
		node->renderproc	= proc;
		node->value			= value;
		node->id			= id;
		node->z				= z;
		node->viewport		= viewport;

		node->prev			= 0;
		node->next			= 0;
	}
	return node;
}

RenderNode*	RenderTree::DeleteNode(RenderNode *node)
{
	RenderNode		*p1;
	RenderNodePtr	*cur_head, *cur_tail;
	
	if (NIL_P(node->viewport))
	{
		cur_head = &s_pRenderHead;
		cur_tail = &s_pRenderTail;
	}
	else
	{
		RbViewport*	vp;
		Data_Get_Struct(node->viewport, RbViewport, vp);
		cur_head = vp->GetHeadPtr();
		cur_tail = vp->GetTailPtr();
	}

	p1 = *cur_head;
	
	while(p1)
	{
		if (p1->value == node->value && p1->id == node->id)
			break;
		p1 = p1->next;
	}
	if (p1->prev == NULL)
	{
		*cur_head = p1->next;
		if (p1->next)
			p1->next->prev = NULL;
		else
			*cur_tail = NULL;
	}
	else
	{
		if (p1->next == NULL)
		{
			p1->prev->next = NULL;
			*cur_tail = p1->prev;
		}
		else
		{
			p1->prev->next = p1->next;
			p1->next->prev = p1->prev;
		}
	}

	p1->prev = NULL;
	p1->next = NULL;

	return p1;
}

void RenderTree::InsertNode(RenderNode *node)
{	
	RenderNode		*p1;
	RenderNodePtr	*cur_head, *cur_tail;
	
	if (NIL_P(node->viewport))
	{
		cur_head = &s_pRenderHead;
		cur_tail = &s_pRenderTail;
	}
	else
	{
		RbViewport*	vp;
		Data_Get_Struct(node->viewport, RbViewport, vp);
		cur_head = vp->GetHeadPtr();
		cur_tail = vp->GetTailPtr();
	}

	if (*cur_head == NULL)
	{
		*cur_tail = *cur_head = node;
	}
	else
	{
		p1 = *cur_tail;
		while (p1)
		{
			if (node->z >= p1->z)
				break;
			else
				p1 = p1->prev;
		}
		if (p1 == NULL)
		{
			// insert before *cur_head node
			node->next = *cur_head;
			(*cur_head)->prev = node;
			*cur_head = node;
		}
		else
		{
			// insert after p1 node
			node->prev = p1;
			node->next = p1->next;
			if (p1->next)
				p1->next->prev = node;
			else
				*cur_tail = node;
			p1->next = node;
		}
	}
}

void RenderTree::DestroyNode(RenderNodePtr* node)
{
	if (*node)
	{
		//	当viewport为Qnil或value值的情况下，从渲染数移除结点。
		if ((*node)->viewport)
			DeleteNode(*node);

		//	释放结点
		xfree(*node);
		*node = 0;
	}
}

void RenderTree::FreeNode(RenderNodePtr* node)
{
	if (*node)
	{
		xfree(*node);
		*node = 0;
	}
}

RenderNode* RenderTree::DoubleLinkAddToFront(RenderNode* list, RenderNode* node)
{
	if (!node)
		return list;

	node->next = list;
	node->prev = 0;

	if (list)
		list->prev = node;

	return node;
}

RenderNode* RenderTree::DoubleLinkDelete(RenderNode* list, RenderNode* node)
{
	if (!node)
		return list;

	if (node->next)
		node->next->prev = node->prev;

	if (node->prev)
	{
		node->prev->next = node->next;
		return list;
	}
	else
		return node->next;
}