#ifndef _SIN_COLOR_H_
#define _SIN_COLOR_H_

#define MAKE_ARGB_8888(a, r, g, b)  (b | (g << 8) | (r << 16) | (a << 24))
#define GET_ARGB_8888(col, a, r, g, b)	{ \
	a = (col >> 24) & 0xFF;\
	r = (col >> 16) & 0xFF;\
	g = (col >> 8) & 0xFF;\
	b = (col & 0xFF);\
}
#define GET_RGBA_B(col)	(col&0xFF)
#define GET_RGBA_G(col)	((col>>8)&0xFF)
#define GET_RGBA_R(col)	((col>>16)&0xFF)
#define GET_RGBA_A(col)	((col>>24)&0xFF)

#define BLEND_ARGB_8888(src, des) {\
	BYTE a1, r1, g1, b1, a2, r2, g2, b2;\
	GET_ARGB_8888(src, a1, r1, g1, b1);\
	GET_ARGB_8888(des, a2, r2, g2, b2);\
	float ftmp = (1 + (float)a2 / a1 - (float)a2 / 255);\
	a2 = a1 + a2 - (a1 * a2 / 255);\
	r2 = (BYTE)((r1 - r2) / ftmp + r2);\
	g2 = (BYTE)((g1 - g2) / ftmp + g2);\
	b2 = (BYTE)((b1 - b2) / ftmp + b2);\
	des = MAKE_ARGB_8888(a2, r2, g2, b2);\
}

//#define BLEND_ARGB_8888(src, des) {\
//	BYTE a1, r1, g1, b1, a2, r2, g2, b2;\
//	GET_ARGB_8888(src, a1, r1, g1, b1);\
//	GET_ARGB_8888(des, a2, r2, g2, b2);\
//	float ftmp = (1 + (float)a2 / a1 - (float)a2 / 255);\
//	a2 = a1 + a2 - (a1 * a2 / 255);\
//	r2 = r + r2 - (r * r2 / 255);\
//	g2 = g + g2 - (g * g2 / 255);\
//	b2 = b + b2 - (b * b2 / 255);\
//	des = MAKE_ARGB_8888(a2, r2, g2, b2);\
//}
	
#endif // _SIN_COLOR_H_