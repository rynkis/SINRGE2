#include "RbExport.h"
#include "SINRGE2.h"
#include "sin_graphics.h"

using namespace Sin;

static VALUE rb_mGraphics;

static VALUE init()
{
	if (GetHgePtr())
		return Qtrue;

	CreateHge();
	// Set our render proc
	//GetHgePtr()->System_SetState(HGE_RENDERFUNC, Sin::RenderProc);
	GetHgePtr()->System_SetState(HGE_TEXTUREFILTER, false);

	bool	isFullScreen;

	if (GetFrmStructPtr()->m_forbid_fullscreen)
		isFullScreen = false;
	else
	{
		if (GetFrmStructPtr()->m_fullscreen_start)
			isFullScreen = true;
		else
		{
			isFullScreen = int(GetFrmStructPtr()->m_screen_width) >= GetSystemMetrics(SM_CXSCREEN) && 
				int(GetFrmStructPtr()->m_screen_height) >= GetSystemMetrics(SM_CYSCREEN);
		}
	}
	 //Set the window title
	GetHgePtr()->System_SetState(HGE_TITLE, GetTitleW());
	// Set our frame's width
	GetHgePtr()->System_SetState(HGE_SCREENWIDTH, GetFrmStructPtr()->m_screen_width);
	// Set our frame's height
	GetHgePtr()->System_SetState(HGE_SCREENHEIGHT, GetFrmStructPtr()->m_screen_height);
	// Run in windowed mode, Default window size is 800x600
 	GetHgePtr()->System_SetState(HGE_WINDOWED, !isFullScreen);
	// Set default FPS
	GetHgePtr()->System_SetState(HGE_FPS, 60);
	
	if(!GetHgePtr()->System_Initiate())
	{
		return Qfalse;
	}
	//	start HGE system 
	GetHgePtr()->System_Start();
	//	Save the window's hwnd
	GetFrmStructPtr()->m_hwnd = GetHgePtr()->System_GetState(HGE_HWND);
	return Qtrue;
}

static VALUE quit()
{
	rb_exit(EXIT_SUCCESS);
	return Qnil;
}

static VALUE begin_scene(int argc, VALUE* argv)
{
	VALUE clear;
    rb_scan_args(argc, argv, "01", &clear);
	
	GetHgePtr()->Gfx_BeginScene();
	if (!RTEST(clear))
		GetHgePtr()->Gfx_Clear(0);

	return Qnil;
}

static VALUE end_scene()
{
	GetHgePtr()->Gfx_SetClipping();
	GetHgePtr()->Gfx_EndScene();
	return Qnil;
}

static VALUE image_to_screen(int argc, VALUE img, VALUE x, VALUE y)
{
	SafeImageValue(img);
	SafeNumericValue(x);
	SafeNumericValue(y);

	image_p pimage;
	Data_Get_Struct(img, image_t, pimage);
	ImageToScreen(pimage, NUM2DBL(x), NUM2DBL(y));
	return Qnil;
}

void bind_graphics()
{
	rb_define_singleton_method(rb_mNge, "init", RbFunc(init), 0);
	rb_define_singleton_method(rb_mNge, "quit", RbFunc(quit), 0);
	
	rb_define_module_function(rb_mNge, "begin_scene", RbFunc(begin_scene), -1);
	rb_define_module_function(rb_mNge, "end_scene", RbFunc(end_scene), 0);
	rb_define_module_function(rb_mNge, "image_to_screen", RbFunc(image_to_screen), 3);
}

namespace Sin
{
	void InitRbGraphics()
	{
		bind_graphics();
	}
}