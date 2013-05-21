/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Table
*/
#ifndef __C_RB_TABLE_H__
#define __C_RB_TABLE_H__

#ifdef WIN32
#pragma once
#endif

#include "CRbClassBase.h"

class CRbTable : public CRbClassBase
{
public:
	CRbTable()
		: m_data(0), m_dims(0), m_size(0)
	{
		memset(m_dim_size, 0, sizeof(m_dim_size));
	}
	virtual ~CRbTable()											//	free
	{
		if (m_data)
		{
			xfree(m_data);
			m_data = 0;
		}
	}

public:
	static void		InitLibrary();

public:
	VALUE *			GetDataPtr() const { return m_data; }

protected:
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);	//	initialize
	virtual VALUE	_dump(VALUE depth);

protected:
	VALUE *			m_data;
	union
	{
		struct
		{
			int		m_dims;
			int		m_dim_size[3];
			int		m_size;
		};
		int			m_dump_data[5];	//	for _dump and load ...
	};

protected:
	static VALUE	dm_load(VALUE klass, VALUE str);

	dm_method_vargs(resize)

	attr_reader(xsize)
	attr_reader(ysize)
	attr_reader(zsize)

	dm_method_vargs(get_element)
	dm_method_vargs(set_element)
};

#endif