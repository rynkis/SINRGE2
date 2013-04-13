#ifndef __SIN_TYPES_H__
#define __SIN_TYPES_H__


typedef	unsigned char	u8;
typedef signed char		s8;
typedef unsigned short	u16;
typedef signed char		s16;
typedef unsigned long	u32;
typedef signed long		s32;
typedef float			f32;

#define SinArrayCount(ary)	(sizeof(ary) / sizeof((ary)[0]))
#define SinMax(a, b)				(((a) > (b)) ? (a) : (b))
#define SinMin(a, b)				(((a) < (b)) ? (a) : (b))
#define SinBound(x, minv, maxv)		((x) < (minv) ? (minv) : ((x) > (maxv) ? (maxv) : (x)))

#define SinRad2Deg(rad)				((rad) * 57.29577951f)
#define SinDeg2Rad(deg)				((deg) * 0.017453292f)


#endif	//	__SIN_TYPES_H__
