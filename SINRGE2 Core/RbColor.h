#ifndef __C_RGE_COLOR_H__
#define __C_RGE_COLOR_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"

using namespace Sin;

#define PLATFORM_LITTLE_ENDIAN

typedef union __UColor
{
#ifdef PLATFORM_LITTLE_ENDIAN	//	LITTLE_ENDIAN
	struct //__LSB
	{
		BYTE	b;
		BYTE	g;
		BYTE	r;
		BYTE	a;
	};
#else							//	BIG-ENDIAN
	struct //__MSB
	{
		BYTE	a;
		BYTE	r;
		BYTE	g;
		BYTE	b;
	};
#endif	//	PLATFORM_LITTLE_ENDIAN

	DWORD	value;

	__UColor(DWORD v) : value(v) {}
	__UColor(BYTE _a, BYTE _r, BYTE _g, BYTE _b) : a(_a), r(_r), g(_g), b(_b) {}

} UColor;

class RbColor : public RbClassBase
{
public:
	RbColor();													//	allocate

public:
	static void		InitLibrary();

public:
	const UColor&	GetColor() const { return m_color; }

protected:
	virtual VALUE	initialize(int argc, VALUE *argv, VALUE obj);	//	initialize

	virtual VALUE	set(int argc, VALUE *argv, VALUE obj);
	virtual VALUE	_dump(VALUE depth);
	virtual VALUE	clone();

	virtual VALUE	to_string();

protected:
	UColor			m_color;
	int				m_r;
	int				m_g;
	int				m_b;
	int				m_a;

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
	static VALUE	dm_set(int argc, VALUE *argv, VALUE obj);

	attr_accessor(red)
	attr_accessor(green)
	attr_accessor(blue)
	attr_accessor(alpha)
};

#endif