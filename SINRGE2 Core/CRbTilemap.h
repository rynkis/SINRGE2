/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Plane
*/
#ifndef __C_RB_TILEMAP_H__
#define __C_RB_TILEMAP_H__

#include "CRbDisplayObject.h"

class SinSprite;

class CRbTable;

class CRbTilemap : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	CRbTilemap();
	virtual ~CRbTilemap();

public:
	static void		InitLibrary();

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);

	virtual	void	render(u32 id);
	
	virtual VALUE	get_z();
	virtual VALUE	set_z(VALUE z);

	virtual VALUE	set_viewport(VALUE viewport);

protected:
	RbRenderNode *	m_node;

	int				m_opacity;
	int				m_blend_type;
	int				m_save_blend_type;


	SinSprite *		m_pSpr;
	CRbTable *		m_table;
	
protected:
	void			check_raise();

protected:
	dm_method(dispose)
	dm_method(is_disposed)

};


#endif // __C_RB_TILEMAP_H__