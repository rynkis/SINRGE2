#ifndef __C_RGE_VIEWPORT_H__
#define __C_RGE_VIEWPORT_H__

#include "RbDisplayObject.h"

class RbRect;
class RbColor;
class RbTone;

class RbViewport : public DisplayObject
{
public:
	RbViewport();
	virtual ~RbViewport();

public:
	static void			InitLibrary();

public:
	RenderNodePtr*		GetHeadPtr() { return &m_head; }
	RenderNodePtr*		GetTailPtr() { return &m_tail; }
	const RbRect*		GetRectPtr() const { return m_rect_ptr; }

protected:
	virtual void		mark();
	virtual VALUE		initialize(int argc, VALUE *argv, VALUE obj);

	virtual	void		render(u32 id);

	virtual VALUE		set_z(VALUE z);

protected:
	inline bool			render_to_texture() const;

protected:
	int					m_flash_duration;					//	�ӿ���˸֡��
	int					m_flash_reduce_count_per_frame;		//	�ӿ���˸ÿ֡���͵�͸����
	int					m_flash_hide_spr;					//	�Ƿ���ȥ�ӿڣ�������˸��ɫָ��Ϊnilʱ����ȥ�ӿڡ�
	DWORD				m_flash_color;						//	�ӿ���˸����ɫֵ��

	hgeQuad				m_quad;

	RenderNode*			m_node;
	RenderNode*			m_node_to_texture;	//	��Ⱦ����������������ĳߴ糬���Կ��������ߴ磬��Ҫ���������������δ���������

	RenderNode*			m_tail;
	RenderNode*			m_head;

	RbRect*				m_rect_ptr;
	RbColor*			m_color_ptr;
	RbTone*				m_tone_ptr;

protected:
	dm_method(update)
	dm_method02(flash)
	attr_accessor(rect)
	attr_accessor(color)
	attr_accessor(tone)
	
protected:
	static HGE*			s_pHge;
};

#endif