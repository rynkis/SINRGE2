/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** System layer API
*/


#ifndef HGE_H
#define HGE_H


#include <windows.h>

#define HGE_VERSION 0x180

#ifdef HGEDLL
#define EXPORT  __declspec(dllexport)
#else
#define EXPORT
#endif

#define CALL  __stdcall

#ifdef __BORLANDC__
 #define floorf (float)floor
 #define sqrtf (float)sqrt
 #define acosf (float)acos
 #define atan2f (float)atan2
 #define cosf (float)cos
 #define sinf (float)sin
 #define powf (float)pow
 #define fabsf (float)fabs

 #define min(x,y) ((x) < (y)) ? (x) : (y)
 #define max(x,y) ((x) > (y)) ? (x) : (y)
#endif


/*
** Common data types
*/
#ifndef DWORD
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif


/*
** Common math constants
*/
#ifndef M_PI
#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#endif


/*
** HGE Handle types
*/
typedef DWORD HTEXTURE;
typedef DWORD HTARGET;
typedef DWORD HEFFECT;
typedef DWORD HMUSIC;
typedef DWORD HSTREAM;
typedef DWORD HCHANNEL;

/*
** Hardware color macros
*/
//#define ARGB(a,r,g,b)	((DWORD(a)<<24) + (DWORD(r)<<16) + (DWORD(g)<<8) + DWORD(b))
//#define GETA(col)		((col)>>24)
//#define GETR(col)		(((col)>>16) & 0xFF)
//#define GETG(col)		(((col)>>8) & 0xFF)
//#define GETB(col)		((col) & 0xFF)
//#define SETA(col,a)		(((col) & 0x00FFFFFF) + (DWORD(a)<<24))
//#define SETR(col,r)		(((col) & 0xFF00FFFF) + (DWORD(r)<<16))
//#define SETG(col,g)		(((col) & 0xFFFF00FF) + (DWORD(g)<<8))
//#define SETB(col,b)		(((col) & 0xFFFFFF00) + DWORD(b))

#define ARGB(a, r, g, b)	(b | (g << 8) | (r << 16) | (a << 24))
#define GETA(col)			(col >> 24)
#define GETR(col)			((col >> 16) & 0xFF)
#define GETG(col)			((col >> 8) & 0xFF)
#define GETB(col)			(col & 0xFF)
#define SETA(col, a)		((col & 0x00FFFFFF) + (a << 24))
#define SETR(col, r)		((col & 0xFF00FFFF) + (r << 16))
#define SETG(col, g)		((col & 0xFFFF00FF) + (g << 8))
#define SETB(col, b)		((col & 0xFFFFFF00) + b)

//+++
#define QUAD_INIT(quad) {\
	quad.blend = BLEND_DEFAULT;\
	quad.blend_color = 0x00000000;\
	for (int i = 0; i < 4; i++)\
	{\
		quad.v[i].z = 0.5f;\
		quad.v[i].col = 0xffffffff;\
	}\
	quad.v[0].tx = 0; quad.v[0].ty = 0;\
	quad.v[1].tx = 1; quad.v[1].ty = 0;\
	quad.v[2].tx = 1; quad.v[2].ty = 1;\
	quad.v[3].tx = 0; quad.v[3].ty = 1;\
};

#define QUAD_SET_VRECT(quad, x1, y1, x2, y2) {\
	quad.v[0].x = x1; quad.v[0].y = y1;\
	quad.v[1].x = x2; quad.v[1].y = y1;\
	quad.v[2].x = x2; quad.v[2].y = y2;\
	quad.v[3].x = x1; quad.v[3].y = y2;\
};
//+++

/*
** HGE Blending constants
*/
#define	BLEND_COLORADD		1
#define	BLEND_COLORMUL		0
#define	BLEND_ALPHABLEND	2
#define	BLEND_ALPHAADD		0
#define	BLEND_ZWRITE		4
#define	BLEND_NOZWRITE		0

#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)

#define	BLEND_ALPHASUBTRACT	8	//	SINRGE2
#define	BLEND_COLORBLNED	16	//	SINRGE2

/*
** HGE System state constants
*/
enum hgeBoolState
{
	HGE_WINDOWED		= 1,    // bool		run in window?		(default: false)
	HGE_ZBUFFER			= 2,    // bool		use z-buffer?		(default: false)
	HGE_TEXTUREFILTER	= 3,    // bool		texture filtering?	(default: true)
	
	HGE_DONTSUSPEND		= 5,	// bool		focus lost:suspend?	(default: false)

	HGEBOOLSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeFuncState
{
	HGE_RENDERFUNC		= 9,    // bool*()	render function		(default: NULL)
	HGE_FOCUSLOSTFUNC	= 10,   // bool*()	focus lost function	(default: NULL)
	HGE_FOCUSGAINFUNC	= 11,   // bool*()	focus gain function	(default: NULL)
	HGE_GFXRESTOREFUNC	= 12,   // bool*()	exit function		(default: NULL)
	HGE_EXITFUNC		= 13,   // bool*()	exit function		(default: NULL)
	
	HGEFUNCSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeHwndState
{
	HGE_HWND			= 15,	// int		window handle: read only
	
	HGEHWNDSTATE_FORCE_DWORD = 0x7FFFFFFF
};

enum hgeIntState
{
	HGE_SCREENWIDTH		= 17,   // int		screen width		(default: 800)
	HGE_SCREENHEIGHT	= 18,   // int		screen height		(default: 600)
	HGE_SCREENBPP		= 19,   // int		screen bitdepth		(default: 32) (desktop bpp in windowed mode)
	
	HGE_FPS				= 24,	// int		fixed fps			(default: HGEFPS_UNLIMITED)

	HGEINTSTATE_FORCE_DWORD = 0x7FFFFFF
};

enum hgeStringState
{
	HGE_TITLE			= 27,   // char*	window title		(default: "HGE")
	
	HGESTRINGSTATE_FORCE_DWORD = 0x7FFFFFFF
};

/*
** Callback protoype used by HGE
*/
typedef bool (*hgeCallback)();


/*
** HGE_FPS system state special constants
*/
#define HGEFPS_UNLIMITED	0
#define HGEFPS_VSYNC		-1


/*
** HGE_POWERSTATUS system state special constants
*/
//#define HGEPWR_AC			-1
//#define HGEPWR_UNSUPPORTED	-2


/*
** HGE Primitive type constants
*/
#define HGEPRIM_LINES		2
#define HGEPRIM_TRIPLES		3
#define HGEPRIM_QUADS		4


/*
** HGE Vertex structure
*/
struct hgeVertex
{
	float			x, y;		// screen position    
	float			z;			// Z-buffer depth 0..1
	DWORD			col;		// color
	float			tx, ty;		// texture coordinates
};


/*
** HGE Triple structure
*/
struct hgeTriple
{
	hgeVertex		v[3];
	HTEXTURE		tex;
	int				blend;
};


/*
** HGE Quad structure
*/
struct hgeQuad
{
	hgeVertex		v[4];
	HTEXTURE		tex;
	int				blend;
	DWORD			blend_color;	//	SINRGE2
};

/*
** HGE Interface class
*/
class HGE
{
public:
	virtual	void			CALL	Release() = 0;

	virtual bool			CALL	System_Initiate() = 0;
	virtual void			CALL	System_Shutdown() = 0;
	virtual bool			CALL	System_Start() = 0;
	// +++SINRGE2+++
	virtual bool			CALL	System_Update() = 0;
	virtual bool			CALL	System_PeekMessage() = 0;
	virtual void			CALL	System_Resize(int width, int height) = 0;
	virtual void*			CALL	Resource_Load_Without_Suffix(const wchar_t *filename, DWORD *size, wchar_t *suffixs[], int suffixs_size, int *suffix_idx) = 0;
	virtual HTEXTURE		CALL	Texture_CreateFromScreen() = 0;
	virtual bool			CALL	Texture_Save2File(HTEXTURE tex, int width, int height, wchar_t *filename) = 0;
	// +++SINRGE2+++
	virtual wchar_t*		CALL	System_GetErrorMessage() = 0;
	virtual	void			CALL	System_Log(const wchar_t *format, ...) = 0;
	virtual bool			CALL	System_Launch(const wchar_t *url) = 0;
	virtual DWORD*			CALL	System_Snapshot(int& width,int& height) = 0;

private:
	virtual void			CALL	System_SetStateBool  (hgeBoolState   state, bool        value) = 0;
	virtual void			CALL	System_SetStateFunc  (hgeFuncState   state, hgeCallback value) = 0;
	virtual void			CALL	System_SetStateInt   (hgeIntState    state, int         value) = 0;
	virtual void			CALL	System_SetStateString(hgeStringState state, const wchar_t *value) = 0;
	virtual bool			CALL	System_GetStateBool  (hgeBoolState   state) = 0;
	virtual hgeCallback		CALL	System_GetStateFunc  (hgeFuncState   state) = 0;
	virtual HWND			CALL	System_GetStateHwnd  (hgeHwndState   state) = 0;
	virtual int				CALL	System_GetStateInt   (hgeIntState    state) = 0;
	virtual const wchar_t*	CALL	System_GetStateString(hgeStringState state) = 0;

public:
	inline void						System_SetState(hgeBoolState   state, bool        value) { System_SetStateBool  (state, value); }
	inline void						System_SetState(hgeFuncState   state, hgeCallback value) { System_SetStateFunc  (state, value); }
	inline void						System_SetState(hgeIntState    state, int         value) { System_SetStateInt   (state, value); }
	inline void						System_SetState(hgeStringState state, const wchar_t *value) { System_SetStateString(state, value); }
	inline bool						System_GetState(hgeBoolState   state) { return System_GetStateBool  (state); }
	inline hgeCallback				System_GetState(hgeFuncState   state) { return System_GetStateFunc  (state); }
	inline HWND						System_GetState(hgeHwndState   state) { return System_GetStateHwnd  (state); }
	inline int						System_GetState(hgeIntState    state) { return System_GetStateInt   (state); }
	inline const wchar_t*			System_GetState(hgeStringState state) { return System_GetStateString(state); }
	
	virtual void*			CALL	Resource_Load(const wchar_t *filename, DWORD *size=0) = 0;
	virtual void			CALL	Resource_Free(void *res) = 0;
	virtual wchar_t*		CALL	Resource_MakePath(const wchar_t *filename=0) = 0;

	virtual void			CALL	Random_Seed(int seed=0) = 0;
	virtual int				CALL	Random_Int(int min, int max) = 0;
	virtual float			CALL	Random_Float(float min, float max) = 0;

	virtual bool			CALL	Gfx_BeginScene(HTARGET target=0) = 0;
	virtual void			CALL	Gfx_EndScene() = 0;
	virtual void			CALL	Gfx_Clear(DWORD color) = 0;
	//virtual void			CALL	Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color=0xFFFFFFFF, float z=0.5f) = 0;
	//virtual void			CALL	Gfx_RenderTriple(const hgeTriple *triple) = 0;
	virtual void			CALL	Gfx_RenderQuad(const hgeQuad *quad) = 0;
	virtual hgeVertex*		CALL	Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim) = 0;
	virtual void			CALL	Gfx_FinishBatch(int nprim) = 0;
	virtual void			CALL	Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0) = 0;
	virtual void			CALL	Gfx_SetTransform(float x=0, float y=0, float dx=0, float dy=0, float rot=0, float hscale=0, float vscale=0) = 0; 

	virtual HTARGET			CALL	Target_Create(int width, int height, bool zbuffer) = 0;
	virtual void			CALL	Target_Free(HTARGET target) = 0;
	virtual HTEXTURE		CALL	Target_GetTexture(HTARGET target) = 0;

	virtual HTEXTURE		CALL	Texture_Create(int width, int height) = 0;
	virtual HTEXTURE		CALL	Texture_Load(const wchar_t *filename, DWORD size=0, bool bMipmap=false, DWORD dwColorKey = 0) = 0;
	virtual void			CALL	Texture_Free(HTEXTURE tex) = 0;
	virtual int				CALL	Texture_GetWidth(HTEXTURE tex, bool bOriginal=false) = 0;
	virtual int				CALL	Texture_GetHeight(HTEXTURE tex, bool bOriginal=false) = 0;
	virtual DWORD*			CALL	Texture_Lock(HTEXTURE tex, bool bReadOnly=true, int left=0, int top=0, int width=0, int height=0) = 0;
	virtual void			CALL	Texture_Unlock(HTEXTURE tex) = 0;
};

extern "C" { EXPORT HGE * CALL hgeCreate(int ver); }

#endif

