#include "SINRGE2.h"
#include "sin_graphics.h"

using namespace Sin;

void ImageToScreen(image_p texture,float dx,float dy)
{
	float tempx1, tempy1, tempx2, tempy2;

	tempx1 = dx;
	tempy1 = dy;
	tempx2 = dx + texture->texw;
	tempy2 = dy + texture->texh;

	texture->quad.v[0].x = tempx1; texture->quad.v[0].y = tempy1;
	texture->quad.v[1].x = tempx2; texture->quad.v[1].y = tempy1;
	texture->quad.v[2].x = tempx2; texture->quad.v[2].y = tempy2;
	texture->quad.v[3].x = tempx1; texture->quad.v[3].y = tempy2;

	GetHgePtr()->Gfx_RenderQuad(&texture->quad);
}