#ifndef __RGE_TEXTURE_EX_H__
#define __RGE_TEXTURE_EX_H__

#include "RbClassBase.h"
#include "System.h"

using namespace Sin;

typedef struct __SRgeTextureAttr
{
	HTEXTURE	tex;
	u32			width;
	u32			height;
} SRgeTextureAttr;

typedef struct __SRgeTexture
{
	SRgeTextureAttr*	sub_textures;
	u32					rows;
	u32					cols;

	u32					width;
	u32					height;

	__SRgeTexture() : sub_textures(0), rows(0), cols(0), width(0), height(0) {}

	inline bool	IsSingle()				const { return ((rows * cols) == 1); }
	inline bool	IsMulti()				const { return !IsSingle(); }
	inline u32	GetSubTextureCount()	const { return (rows * cols); }

	bool		Create(u32 w, u32 h);
	void		Dispose();

} SRgeTexture;

#endif	//	__RGE_TEXTURE_EX_H__
