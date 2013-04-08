#include "RbExport.h"
#include "SINRGE2.h"

using namespace Sin;

static VALUE rb_mHge;

//hgeQuad quad;

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

	/*
	quad.tex = GetHgePtr()->Texture_Load(L"Graphics/ÖÐÎÄ/²âÊÔpic.bmp");
	if(!quad.tex)
	{
		MessageBox(NULL, L"Can't load PARTICLES.PNG", L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		GetHgePtr()->System_Shutdown();
		GetHgePtr()->Release();
		return 0;
	}
	quad.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;
	for(int i=0;i<4;i++)
	{
		quad.v[i].z=0.5f;
		quad.v[i].col=0xFFFFFFFF;
	}
	quad.v[0].tx=0; quad.v[0].ty=0;
	quad.v[1].tx=1; quad.v[1].ty=0;
	quad.v[2].tx=1; quad.v[2].ty=1;
	quad.v[3].tx=0; quad.v[3].ty=1;

	quad.v[0].x=0; quad.v[0].y=0;
	quad.v[1].x=800; quad.v[1].y=0;
	quad.v[2].x=800; quad.v[2].y=600;
	quad.v[3].x=0; quad.v[3].y=600;
*/
	return Qtrue;
}

//bool Sin::RenderProc()
//{
//	if(!NIL_P(rb_errinfo()))
//		return true;
//	
//	/*RgeRenderNode*	p1;
//
//	for (p1 = s_pViewportLists; p1; p1 = p1->next)
//	{
//		p1->renderproc(p1->value, p1->id);
//	}*/
//	
//	GetHgePtr()->Gfx_BeginScene();
//	GetHgePtr()->Gfx_Clear(0);
//
//	
//	//GetHgePtr()->Gfx_RenderQuad(&quad);
//	/*for (p1 = s_pRenderHead; p1; p1 = p1->next)
//	{
//		p1->renderproc(p1->value, p1->id);
//	}*/
//
//	GetHgePtr()->Gfx_SetClipping();
//	GetHgePtr()->Gfx_EndScene();
//
//	return false;
//}

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

//static VALUE graphics_update()
//{
//    if(!GetHgePtr()->System_Update())
//		quit();
//
//	return Qnil;
//}

void bind_hge()
{
	rb_mHge = rb_define_module_under(rb_mSin, "HGE");
	
	rb_define_singleton_method(rb_mHge, "init", RbFunc(init), 0);
	rb_define_singleton_method(rb_mHge, "quit", RbFunc(quit), 0);
	
	rb_define_singleton_method(rb_mHge, "begin_scene", RbFunc(begin_scene), -1);
	rb_define_singleton_method(rb_mHge, "end_scene", RbFunc(end_scene), 0);

	//rb_define_module_function(rb_mHge, "update", RbFunc(graphics_update), 0);
}

namespace Sin
{
	void InitRbHge()
	{
		bind_hge();
	}
}