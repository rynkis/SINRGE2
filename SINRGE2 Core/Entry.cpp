/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Entry
*/
#include "ruby.h"
//#include "sin_app.h"
typedef VALUE	(*RbFunc)(...);

#include "hge.h"
#include "hgesprite.h"
#include "hgeparticle.h"

HGE *hge=0;

hgeSprite*			spr;
hgeSprite*			spt;
hgeParticleSystem*	par;

HTEXTURE			tex;

float x=100.0f, y=100.0f;
float dx=0.0f, dy=0.0f;

const float speed=90;
const float friction=0.98f;

bool FrameFunc()
{
	float dt=hge->Timer_GetDelta();
	//hge->Input_GetMousePos(&x, &y);
	// Process keys
	//if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	//if (hge->Input_GetKeyState(HGEK_LEFT)) dx-=speed*dt;
	//if (hge->Input_GetKeyState(HGEK_RIGHT)) dx+=speed*dt;
	//if (hge->Input_GetKeyState(HGEK_UP)) dy-=speed*dt;
	//if (hge->Input_GetKeyState(HGEK_DOWN)) dy+=speed*dt;

	//// Do some movement calculations and collision detection	
	//dx*=friction; dy*=friction; x+=dx; y+=dy;
	//if(x>784) {x=784-(x-784);dx=-dx;boom();}
	//if(x<16) {x=16+16-x;dx=-dx;boom();}
	//if(y>584) {y=584-(y-584);dy=-dy;boom();}
	//if(y<16) {y=16+16-y;dy=-dy;boom();}

	// Update particle system
	//par->info.nEmission=(int)(dx*dx+dy*dy)*2;
	//par->MoveTo(x,y);
	par->Update(dt);

	return false;
}


bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	par->Render();
	spr->RenderEx(400, 300, 0.0f, 2.0f, 2.0f);
	//fnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	hge->Gfx_EndScene();

	return false;
}


int Run()
{
	hge = hgeCreate(HGE_VERSION);

	//hge->System_SetState(HGE_LOGFILE, "hge_tut03.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Test");
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate()) {

		// Load sound and texture
		//snd=hge->Effect_Load("menu.wav");
		tex=hge->Texture_Load("PARTICLES.PNG");
		//tex2=hge->Texture_Load("PARTICLES.PNG");
		if(!tex)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nMENU.WAV, PARTICLES.PNG, FONT1.FNT, FONT1.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
			return 0;
		}

		// Create and set up a sprite
		spr=new hgeSprite(tex, 0, 32, 32, 32);
		spr->SetColor(0xFFFFFFFF);
		spr->SetHotSpot(16, 16);

		// Create and set up a particle system
		spt=new hgeSprite(tex, 0, 32, 32, 32);
		spt->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		spt->SetHotSpot(16,16);
		par=new hgeParticleSystem("particle9.psi",spt);
		par->FireAt(400, 128);
		//par->Fire();

		// Let's rock now!
		hge->System_Start();

		// Delete created objects and free loaded resources
		delete par;
		//delete fnt;
		delete spt;
		delete spr;
		hge->Texture_Free(tex);
		//hge->Effect_Free(snd);
	}

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}

//static VALUE move_to(int argc, VALUE *argv)
//{
//	x = FIX2INT(argv[0]);
//	y = FIX2INT(argv[1]);
//	par->MoveTo(x,y);
//}

int SINRGE2Entry()
{
	///<	³õÊ¼»¯½âÊÍÆ÷
	int		argc = 0;
	char **	argv = 0;

	ruby_sysinit(&argc, &argv);
	{
		RUBY_INIT_STACK
		ruby_init();
		ruby_set_argv(argc - 1, argv + 1);
		//rb_set_kcode("utf8");
		//ruby_init_loadpath();
		ruby_incpush("./");
		ruby_script("SINRGE2");
	}

	//rb_define_global_function("move_to", (RbFunc)move_to, 2);

	VALUE rt = rb_eval_string("puts :ok");

	return Run();
	//return 0;//CApplication().Run();
}