#include "RbTextureEx.h"

bool __SRgeTexture::Create(u32 w, u32 h)
{
	if (w == 0 || h == 0)
		return false;

	width	= w;
	height	= h;

	u32 maxTexW = GetRgeSysPtr()->GetMaxTexW();
	u32 maxTexH = GetRgeSysPtr()->GetMaxTexH();

	rows = cols = 1;

	if (maxTexW < width)
		cols = int((width + maxTexW - 1) / maxTexW);

	if (maxTexH < height)
		rows = int((height + maxTexH - 1) / maxTexH);

	sub_textures = new SRgeTextureAttr[GetSubTextureCount()];
	if (!sub_textures)
		return false;

	memset(sub_textures, 0, sizeof(SRgeTextureAttr) * GetSubTextureCount());

	for (u32 idx = 0; idx < GetSubTextureCount(); ++idx)
	{
		sub_textures[idx].tex = GetRgeSysPtr()->GetHgePtr()->Texture_Create(maxTexW, maxTexH);

		if (!sub_textures[idx].tex)
			return false;

		sub_textures[idx].width = maxTexW;
		sub_textures[idx].height = maxTexH;
	}

	return true;
}

void __SRgeTexture::Dispose()
{
	if (sub_textures)
	{
		for (u32 idx = 0; idx < GetSubTextureCount(); ++idx)
		{
			if (sub_textures[idx].tex)
			{
				GetRgeSysPtr()->GetHgePtr()->Texture_Free(sub_textures[idx].tex);
			}
		}
		delete [] sub_textures;
		sub_textures = 0;

		rows = cols = 0;
		width = height = 0;
	}
}