/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class DisplayObject
*/
#ifndef __DISPLAY_OBJECT_H__
#define __DISPLAY_OBJECT_H__

#include "CRbClassBase.h"
#include "CRbRenderTree.h"

class CRbViewport;

class DisplayObject : public CRbClassBase
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

	CRbViewport *	m_viewport_ptr;

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