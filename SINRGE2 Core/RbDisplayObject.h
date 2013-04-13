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
	typedef enum __InvisibleReason
	{
		IR_OBJECT_DISPOSED = 0,
		IR_OBJECT_INVISIBLE,
		IR_VIEWPORT_DISPOSED,

		IR_BITMAP_NIL,
		IR_BITMAP_DISPOSED,
		
		IR_NEGATIVE_ZOOMX,
		IR_NEGATIVE_ZOOMY,

		IR_FLASH_COLOR_NIL,

		IR_UNKNOWN,

		IR_MAX
	} InvisibleReason;
	
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
	dm_method(invisible_reason)

	attr_accessor(x)
	attr_accessor(y)
	attr_accessor(z)
	attr_accessor(ox)
	attr_accessor(oy)
	attr_accessor(visible)
	attr_accessor(viewport)
};

#endif