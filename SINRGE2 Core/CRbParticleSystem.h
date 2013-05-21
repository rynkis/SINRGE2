/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Class PartcleSystem
*/
#ifndef __C_RB_PARTICLE_SYS_H__
#define __C_RB_PARTICLE_SYS_H__
#include "CRbDisplayObject.h"
#include "hgeparticle.h"

class hgeSprite;
class hgeParticleSystem;
class CRbBitmap;

class CRbParticleSystem : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	CRbParticleSystem();
	virtual ~CRbParticleSystem();

public:
	static void		InitLibrary();
	
	void			Render(float offset_x, float offset_y);

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE * argv, VALUE obj);

	virtual	void	render(u32 id);

	virtual VALUE	set_z(VALUE z);
	virtual VALUE	set_viewport(VALUE viewport);
	
protected:
	void			check_raise();

protected:
	RbRenderNode *			m_node;
	
	s32						m_ref_bitmap_modify_count;
	int						m_blend_type;

	hgeParticleSystemInfo	info;

	float					fAge;
	float					fEmissionResidue;

	hgeVector				vecPrevLocation;
	hgeVector				vecLocation;
	float					fTx, fTy;
	float					fScale;

	int						nParticlesAlive;
	hgeRect					rectBoundingBox;
	bool					bUpdateBoundingBox;

	hgeParticle				particles[MAX_PARTICLES];

	hgeSprite *				m_pSpr;

	CRbBitmap *				m_bitmap_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	dm_method(update)
	dm_method02(fire_at)
	dm_method(fire)
	dm_method_vargs(stop)
	dm_method_vargs(move_to)

	attr_accessor(bitmap)
	attr_accessor(blend_type)
};

#endif //__C_RB_PARTICLE_SYS_H__