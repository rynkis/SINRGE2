/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Cpp Struct RbRenderTree
*/
#include "RbRenderTree.h"
#include "RbViewport.h"
#include "sin_app.h"

HGE* RbRenderState::s_pHge = 0;

//RbRenderNode*	RbRenderTree::s_pViewportLists	= 0;
RbRenderNode*	RbRenderTree::s_pRenderHead		= 0;
RbRenderNode*	RbRenderTree::s_pRenderTail		= 0;

void RbRenderTree::Init()
{
}

/**
 *	渲染函数
 */
bool RbRenderTree::RenderProc()
{
	if(!NIL_P(rb_errinfo()))
		return true;

	RbRenderNode*	p1;

	/*for (p1 = s_pViewportLists; p1; p1 = p1->next)
	{
		p1->renderproc(p1->value, p1->id);
	}*/

	HGE* pHGE = GetAppPtr()->GetHgePtr();

	pHGE->Gfx_BeginScene();
	pHGE->Gfx_Clear(0);
	pHGE->Gfx_SetClipping();

	for (p1 = s_pRenderHead; p1; p1 = p1->next)
	{
		p1->renderproc(p1->value, p1->id);
	}

	GetAppPtr()->BrightnessUpdate();

	pHGE->Gfx_SetClipping();
	pHGE->Gfx_EndScene();

	return false;
}

//void RbRenderTree::ViewportAddToFront(RbRenderNode* node)
//{
//	s_pViewportLists = DoubleLinkAddToFront(s_pViewportLists, node);
//}
//
//void RbRenderTree::ViewportDelete(RbRenderNode* node)
//{
//	s_pViewportLists = DoubleLinkDelete(s_pViewportLists, node);
//}

RbRenderNode*	RbRenderTree::AllocNode(RbRenderProc proc, VALUE value, u32 id, s32 z, VALUE viewport)
{
	RbRenderNode* node	= ALLOC(RbRenderNode);
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

RbRenderNode*	RbRenderTree::DeleteNode(RbRenderNode *node)
{
	RbRenderNode	*p1;
	RbRenderNodePtr	*cur_head, *cur_tail;
	
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

void RbRenderTree::InsertNode(RbRenderNode *node)
{	
	RbRenderNode	*p1;
	RbRenderNodePtr	*cur_head, *cur_tail;
	
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

void RbRenderTree::DestroyNode(RbRenderNodePtr* node)
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

void RbRenderTree::FreeNode(RbRenderNodePtr* node)
{
	if (*node)
	{
		xfree(*node);
		*node = 0;
	}
}

RbRenderNode* RbRenderTree::DoubleLinkAddToFront(RbRenderNode* list, RbRenderNode* node)
{
	if (!node)
		return list;

	node->next = list;
	node->prev = 0;

	if (list)
		list->prev = node;

	return node;
}

RbRenderNode* RbRenderTree::DoubleLinkDelete(RbRenderNode* list, RbRenderNode* node)
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