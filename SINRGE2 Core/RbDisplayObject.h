#ifndef __DISPLAY_OBJECT_H__
#define __DISPLAY_OBJECT_H__

#include "RbClassBase.h"
#include "RbRenderTree.h"

class RbViewport;

class DisplayObject : public RbClassBase
{
public:
	DisplayObject();
	virtual	~DisplayObject() {}
	
protected:
	static	void	RenderProc(VALUE obj, u32 id);

protected:
	virtual	void	render(u32 id);

public:
	float			m_x;
	float			m_y;

	s32				m_z;

	float			m_ox;
	float			m_oy;

	VALUE			m_visible;
	bool			m_disposed;

	RbViewport*		m_viewport_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	attr_accessor(x)
	attr_accessor(y)
	attr_accessor(z)
	attr_accessor(ox)
	attr_accessor(oy)
	attr_accessor(visible)
	attr_accessor(viewport)
};

#endif