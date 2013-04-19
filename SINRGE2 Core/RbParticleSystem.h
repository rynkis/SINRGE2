#ifndef __RB_PARTICLE_SYS_H__
#define __RB_PARTICLE_SYS_H__

#include "RbDisplayObject.h"
#include "hgeparticle.h"

class hgeSprite;
class hgeParticleSystem;

class RbBitmap;


class RbParticleSystem : public DisplayObject
{
public:
	typedef DisplayObject	super;

public:
	RbParticleSystem();
	virtual ~RbParticleSystem();

public:
	static void		InitLibrary();

	
	void			Render(float offset_x, float offset_y);

protected:
	virtual void	mark();
	virtual VALUE	initialize(int argc, VALUE *argv, VALUE obj);

	virtual	void	render(u32 id);

	virtual VALUE	set_z(VALUE z);
	virtual VALUE	set_viewport(VALUE viewport);
	
protected:
	void			check_raise();

protected:
	RbRenderNode*			m_node;
	
	s32						m_ref_bitmap_modify_count;
	
	int						mlast;
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

	hgeSprite*				m_pSpr;

	RbBitmap*				m_bitmap_ptr;

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

#endif //__RB_PARTICLE_SYS_H__