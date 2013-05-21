/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Color
*/
#ifndef __RB_COLOR_H__
#define __RB_COLOR_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"

class RbColor : public RbClassBase
{
public:
	RbColor();													//	allocate

public:
	static void		InitLibrary();

public:
	const DWORD		GetColor() const { return m_color; }

protected:
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize

	virtual VALUE	_dump(VALUE depth);
	virtual VALUE	clone();

	virtual VALUE	to_string();

protected:
	DWORD			m_color;
	double			m_r;
	double			m_g;
	double			m_b;
	double			m_a;

	union
	{
		struct
		{
			VALUE	m_red;
			VALUE	m_green;
			VALUE	m_blue;
			VALUE	m_alpha;
		};
		VALUE		m_dump_data[4];	//	for _dump and load ...
	};

protected:
	static VALUE	dm_load(VALUE klass, VALUE str);

	dm_method_vargs(set)

	attr_accessor(red)
	attr_accessor(green)
	attr_accessor(blue)
	attr_accessor(alpha)
};

#endif  //__RB_COLOR_H__