/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Rect
*/
#ifndef __C_RB_RECT_H__
#define __C_RB_RECT_H__

#include "CRbClassBase.h"

class CRbRect : public CRbClassBase
{
public:
	CRbRect();
	//virtual ~CRbRect() {}

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize

	virtual VALUE	set(int argc, VALUE * argv, VALUE obj);
	virtual VALUE	_dump(VALUE depth);
	virtual VALUE	clone();
	
	virtual VALUE	to_string();

public:
	int				m_rect_data[4];

protected:
	union
	{
		struct
		{
			VALUE	vx, vy, vw, vh;
		};
	};

protected:
	static VALUE	dm_load(VALUE klass, VALUE str);
	static VALUE	dm_set(int argc, VALUE * argv, VALUE obj);

	dm_method(empty)
	
	attr_accessor(x)
	attr_accessor(y)
	attr_accessor(width)
	attr_accessor(height)
};

#endif