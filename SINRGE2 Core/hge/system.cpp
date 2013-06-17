/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/

//+++SINRGE2+++
#include "CRbBitmap.h"
#include "MRbSinCore.h"
#include "MRbInput.h"
//+++SINRGE2+++

#include "hge_impl.h"

//+++SINRGE2+++
#include "sin_color.h"
#include "sin_app.h"
//+++SINRGE2+++


#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))


const wchar_t * WINDOW_CLASS_NAME = L"SINRGE2_WNDCLASS";		// SINRGE2

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int			nRef=0;
HGE_Impl*	pHGE=0;


HGE* CALL hgeCreate(int ver)
{
	if(ver==HGE_VERSION)
		return (HGE*)HGE_Impl::_Interface_Get();
	else
		return 0;
}


HGE_Impl* HGE_Impl::_Interface_Get()
{
	if(!pHGE) pHGE=new HGE_Impl();

	nRef++;

	return pHGE;
}


void CALL HGE_Impl::Release()
{
	nRef--;

	if(!nRef)
	{
		if(pHGE->hwnd) pHGE->System_Shutdown();
		delete pHGE;
		pHGE=0;
	}
}


bool CALL HGE_Impl::System_Initiate()
{
	WNDCLASS		winclass;
	int				width, height;

	// Register window class
	
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	winclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(101));
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	
	if (!RegisterClass(&winclass)) {
		_PostError(L"Can't register window class");
		return false;
	}

	// Create window

	width = nScreenWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	height = nScreenHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	rectW.left = (GetSystemMetrics(SM_CXSCREEN)-width) / 2;
	rectW.top = (GetSystemMetrics(SM_CYMAXIMIZED) - nScreenHeight) / 2 - GetSystemMetrics(SM_CYCAPTION);//(GetSystemMetrics(SM_CYSCREEN)-height) / 2 - GetSystemMetrics(SM_CYCAPTION);
	rectW.right = rectW.left + width;
	rectW.bottom = rectW.top + height;
	styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

	rectFS.left = 0;
	rectFS.top = 0;
	rectFS.right = nScreenWidth;
	rectFS.bottom = nScreenHeight;
	styleFS = WS_POPUP | WS_VISIBLE; //WS_POPUP

	if (bWindowed)
		hwnd = CreateWindowExW(0, WINDOW_CLASS_NAME, szWinTitle, styleW,
				rectW.left, rectW.top, rectW.right - rectW.left, rectW.bottom - rectW.top,
				NULL, NULL, hInstance, NULL);
	else
		hwnd = CreateWindowEx(WS_EX_TOPMOST, WINDOW_CLASS_NAME, szWinTitle, styleFS,
				0, 0, 0, 0,
				NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		_PostError(L"Can't create window");
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);

	// Init subsystems

	timeBeginPeriod(1);
	Random_Seed();
	if(!_GfxInit()) { System_Shutdown(); return false; }

	// Done

	return true;
}

void CALL HGE_Impl::System_Shutdown()
{
	timeEndPeriod(1);
	_GfxDone();

	if(hwnd)
	{
		DestroyWindow(hwnd);
		hwnd=0;
	}

	if(hInstance) UnregisterClass(WINDOW_CLASS_NAME, hInstance);
}

bool CALL HGE_Impl::System_Start()
{
	if(!hwnd)
	{
		_PostError(__TEXT("System_Start: System_Initiate wasn't called"));
		return false;
	}

	bActive=true;

	return true;
}

void CALL HGE_Impl::System_SetStateBool(hgeBoolState state, bool value)
{
	switch(state)
	{
		case HGE_WINDOWED:		if(VertArray) break;
								if(pD3DDevice && bWindowed != value)
								{
									if(d3dppW.BackBufferFormat==D3DFMT_UNKNOWN || d3dppFS.BackBufferFormat==D3DFMT_UNKNOWN) break;

									if(bWindowed) GetWindowRect(hwnd, &rectW);
									bWindowed=value;
									if(bWindowed) d3dpp=&d3dppW;
									else d3dpp=&d3dppFS;

									if(_format_id(d3dpp->BackBufferFormat) < 4) nScreenBPP=16;
									else nScreenBPP=32;

									_GfxRestore();
									_AdjustWindow();
								}
								else bWindowed=value;
								break;

		case HGE_ZBUFFER:		if(!pD3DDevice)	bZBuffer=value;
								break;

		case HGE_TEXTUREFILTER: bTextureFilter=value;
								if(pD3DDevice)
								{
									_render_batch();
									if(bTextureFilter)
									{
										pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
										pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
									}
									else
									{
										pD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
										pD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
									}
								}
								break;

		case HGE_DONTSUSPEND:	bDontSuspend=value; break;

	}
}

void CALL HGE_Impl::System_SetStateFunc(hgeFuncState state, hgeCallback value)
{
	switch(state)
	{
		case HGE_RENDERFUNC:	 procRenderFunc=value; break;
		case HGE_FOCUSLOSTFUNC:	 procFocusLostFunc=value; break;
		case HGE_FOCUSGAINFUNC:	 procFocusGainFunc=value; break;
		case HGE_GFXRESTOREFUNC: procGfxRestoreFunc=value; break;
		case HGE_EXITFUNC:		 procExitFunc=value; break;
	}
}

void CALL HGE_Impl::System_SetStateInt(hgeIntState state, int value)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	if(!pD3DDevice) nScreenWidth=value; break;

		case HGE_SCREENHEIGHT:	if(!pD3DDevice) nScreenHeight=value; break;

		case HGE_SCREENBPP:		if(!pD3DDevice) nScreenBPP=value; break;

		case HGE_FPS:			if(VertArray) break;

								if(pD3DDevice)
								{
									if((nHGEFPS>=0 && value <0) || (nHGEFPS<0 && value>=0))
									{
										if(value==HGEFPS_VSYNC)
										{
											d3dppW.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
											d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
										}
										else
										{
											d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
											d3dppFS.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
										}
										//if(procFocusLostFunc) procFocusLostFunc();
										_GfxRestore();
										//if(procFocusGainFunc) procFocusGainFunc();
									}
								}
								nHGEFPS=value;
								break;
	}
}

void CALL HGE_Impl::System_SetStateString(hgeStringState state, const wchar_t *value)
{
	if (state == HGE_TITLE)
	{
		wcscpy_s(szWinTitle,value);
		if(pHGE->hwnd)
			SetWindowText(pHGE->hwnd, szWinTitle);
	}
}

bool CALL HGE_Impl::System_GetStateBool(hgeBoolState state)
{
	switch(state)
	{
		case HGE_WINDOWED:		return bWindowed;
		case HGE_ZBUFFER:		return bZBuffer;
		case HGE_TEXTUREFILTER:	return bTextureFilter;
		case HGE_DONTSUSPEND:	return bDontSuspend;
	}

	return false;
}

hgeCallback CALL HGE_Impl::System_GetStateFunc(hgeFuncState state)
{
	switch(state)
	{
		case HGE_RENDERFUNC:	return procRenderFunc;
		case HGE_FOCUSLOSTFUNC:	return procFocusLostFunc;
		case HGE_FOCUSGAINFUNC:	return procFocusGainFunc;
		case HGE_EXITFUNC:		return procExitFunc;
	}

	return NULL;
}

HWND CALL HGE_Impl::System_GetStateHwnd(hgeHwndState state)
{
	switch(state)
	{
		case HGE_HWND:			return hwnd;
	}

	return 0;
}

int CALL HGE_Impl::System_GetStateInt(hgeIntState state)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	return nScreenWidth;
		case HGE_SCREENHEIGHT:	return nScreenHeight;
		case HGE_SCREENBPP:		return nScreenBPP;
		case HGE_FPS:			return nHGEFPS;
	}

	return 0;
}

const wchar_t* CALL HGE_Impl::System_GetStateString(hgeStringState state) {
	if (state == HGE_TITLE)
		return szWinTitle;
	return NULL;
}

wchar_t* CALL HGE_Impl::System_GetErrorMessage()
{
	return szError;
}

void CALL HGE_Impl::System_Log(const wchar_t *szFormat, ...)
{
	static wchar_t szError[1024];
	va_list ap;

	va_start(ap, szFormat);
	vswprintf_s(szError, szFormat, ap);
	va_end(ap);
	
	wprintf(szError);
	printf("\n");
	return;
}

bool CALL HGE_Impl::System_Launch(const wchar_t *url)
{
	if((DWORD)ShellExecute(pHGE->hwnd, NULL, url, NULL, NULL, SW_SHOWMAXIMIZED)>32) return true;
	else return false;
}

DWORD* CALL HGE_Impl::System_Snapshot(int& width, int& height)
{
	if(!pD3DDevice) return 0;

	if (!bFreeze)
	{
		bFreeze = true;
		procRenderFunc();
		bFreeze = false;
	}
	else procRenderFunc();

	LPDIRECT3DSURFACE8 pSurf;
	if (FAILED(pD3DDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf)))
		goto __failed_return;

	D3DSURFACE_DESC surfaceDesc;
	if (FAILED(pSurf->GetDesc(&surfaceDesc)))
		goto __failed_return;
	
	width = surfaceDesc.Width;
	height = surfaceDesc.Height;
	
	//D3DXSaveSurfaceToFile(L"snap.bmp", D3DXIFF_BMP, pSurf, NULL, NULL);

	D3DLOCKED_RECT lockedRect;
	if (FAILED(pSurf->LockRect(&lockedRect, 0, D3DLOCK_READONLY)))
		goto __failed_return;
	width = lockedRect.Pitch / 4;
	DWORD * pData = (DWORD *)malloc(width * height * sizeof(DWORD));
	memcpy(pData, lockedRect.pBits, width * height * sizeof(DWORD));
	pSurf->UnlockRect();
	pSurf->Release();

	return pData;

__failed_return:
	if (pSurf)
		pSurf->Release();
	return 0;
}

//////// Implementation ////////


HGE_Impl::HGE_Impl()
{
	hInstance=GetModuleHandle(0);
	hwnd=0;
	bActive=false;
	szError[0]=0;

	pD3D=0;
	pD3DDevice=0;
	d3dpp=0;
	pTargets=0;
	pCurTarget=0;
	pScreenSurf=0;
	pScreenDepth=0;
	pVB=0;
	pIB=0;
	VertArray=0;
	textures=0;

	nHGEFPS=60;
	
	procRenderFunc=0;
	procFocusLostFunc=0;
	procFocusGainFunc=0;
	procGfxRestoreFunc=0;
	procExitFunc=0;
	wcscpy_s(szWinTitle,L"HGE");
	nScreenWidth=800;
	nScreenHeight=600;
	nScreenBPP=32;
	bWindowed=false;
	bZBuffer=false;
	bTextureFilter=true;
	
	bHideMouse=true;
	bDontSuspend=false;
	
	GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
	int i;
	for(i=wcslen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
	szAppPath[i+1]=0;
	
	// +++SINRGE2+++
	szTitleFps[0] = 0;
	mouseButton = 0;
	mouseWheel = 0;
	mouseMove = false;
	bShowFps = false;
	bOnFocus = true;
	bFreeze = false;
	freezeTex = 0;
	// +++SINRGE2+++
}

void HGE_Impl::_PostError(wchar_t *error)
{
	System_Log(error);
	wcscpy_s(szError,error);
}

void HGE_Impl::_FocusChange(bool bAct)
{
	bActive=bAct;

	if(bActive)
	{
		if(procFocusGainFunc) procFocusGainFunc();
	}
	else
	{
		if(procFocusLostFunc) procFocusLostFunc();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool bActivating;

	pHGE->mouseMove = false;

	switch(msg)
	{
	case WM_KILLFOCUS:
		pHGE->bOnFocus = false;
		return FALSE;
	case WM_SETFOCUS:
		pHGE->bOnFocus = true;
		return FALSE;

	case WM_CREATE: 
		return FALSE;
		
	case WM_PAINT:
		if(pHGE->pD3D && pHGE->procRenderFunc && pHGE->bWindowed) pHGE->procRenderFunc();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return FALSE;

/*
		case WM_ACTIVATEAPP:
			bActivating = (wparam == TRUE);
			if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
			return FALSE;
*/
	case WM_ACTIVATE:
		// tricky: we should catch WA_ACTIVE and WA_CLICKACTIVE,
		// but only if HIWORD(wParam) (fMinimized) == FALSE (0)
		bActivating = (LOWORD(wparam) != WA_INACTIVE) && (HIWORD(wparam) == 0);
		if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
		return FALSE;


	case WM_SETCURSOR:
		if(pHGE->bActive && LOWORD(lparam)==HTCLIENT && pHGE->bHideMouse) SetCursor(NULL);
		else SetCursor(LoadCursor(NULL, IDC_ARROW));
		return FALSE;

	case WM_SYSKEYDOWN:
		if (wparam == VK_F1)
		{
			pHGE->bShowFps = !pHGE->bShowFps;
			if (!pHGE->bShowFps)
				SetWindowText(pHGE->hwnd, pHGE->szWinTitle);
			return FALSE;
		}
		else if(wparam == VK_F4)
		{
			if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		else if(wparam == VK_RETURN)
		{
			pHGE->System_SetState(HGE_WINDOWED, !pHGE->System_GetState(HGE_WINDOWED));
				return FALSE;
		}
		return FALSE;
	case WM_KEYDOWN:
		return FALSE;
	case WM_SYSKEYUP:
		return FALSE;
	case WM_KEYUP:
		return FALSE;

	case WM_LBUTTONDBLCLK:
		pHGE->mouseButton += 64;
		return FALSE;
	case WM_RBUTTONDBLCLK:
		pHGE->mouseButton += 128;
		return FALSE;
	case WM_MBUTTONDBLCLK:
		pHGE->mouseButton += 256;
		return FALSE;
			
	case WM_MOUSEMOVE:
		pHGE->mouseMove = true;
		return FALSE;
	case WM_MOUSEWHEEL:
		pHGE->mouseWheel = (short)HIWORD(wparam);
		return FALSE;

	case WM_SYSCOMMAND:
		if (wparam == SC_KEYMENU)
			return TRUE;
		else if(wparam==SC_CLOSE)
		{
			if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
			pHGE->bActive=false;
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** Ruby Moudle SINRGE2
*/

bool CALL HGE_Impl::System_Update()
{
	if (!System_PeekMessage())
		return false;
	// If HGE window is focused or we have the "don't suspend" state - process the main loop
	if(bActive || bDontSuspend)
	{
		if(procRenderFunc)
			procRenderFunc();

		if (bShowFps)
		{
			wsprintfW(szTitleFps, L"%s - %d FPS", szWinTitle, GetAppPtr()->GetRealFps());
			SetWindowText(hwnd, szTitleFps);
		}
	}
	GetAppPtr()->LimitFps(nHGEFPS);
	return true;
}

bool CALL HGE_Impl::System_PeekMessage()
{
	// Process window messages
	if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		if (m_msg.message == WM_QUIT)
		{
			bActive=false;
			return false;
		}
		DispatchMessage(&m_msg);
	}
	return true;
}

void CALL HGE_Impl::System_Resize(int width, int height)
{
	nScreenWidth = width;
	nScreenHeight = height;

	width = nScreenWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	height = nScreenHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	rectW.left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	rectW.top = (GetSystemMetrics(SM_CYMAXIMIZED) - nScreenHeight) / 2 - GetSystemMetrics(SM_CYCAPTION);
	rectW.right = rectW.left + width;
	rectW.bottom = rectW.top + height;

	::SetWindowPos(hwnd, 0, rectW.left, rectW.top, rectW.right - rectW.left, rectW.bottom - rectW.top, SWP_NOZORDER);

	d3dppW.BackBufferWidth = nScreenWidth;
	d3dppW.BackBufferHeight = nScreenHeight;

	_SetProjectionMatrix(nScreenWidth, nScreenHeight);
	_GfxRestore();
}

HTEXTURE CALL HGE_Impl::Texture_CreateFromScreen()
{
	int width, height;
	DWORD * pTexData = System_Snapshot(width, height);
	if (!pTexData) return 0;
	HTEXTURE tex = Texture_Create(nScreenWidth, height);

	DWORD * pDesData = Texture_Lock(tex, false);
	for (int ly = 0; ly < height; ++ly)
	{
		int tv = ly * width;
		int ti = GetAppPtr()->GetFrameWidth() * ly;
		for (int lx = 0; lx < GetAppPtr()->GetFrameWidth(); ++lx)
		{
			GetAppPtr()->SystemUpdate();
			pDesData[ti + lx] = SETA(pTexData[tv + lx], 255);
		}
	}
	Texture_Unlock(tex);
	free(pTexData);
	return tex;
}

bool CALL HGE_Impl::Texture_Save2File(HTEXTURE tex, int width, int height, wchar_t *filename)
{
	LPDIRECT3DSURFACE8 pDst = NULL;
	LPDIRECT3DSURFACE8 pSrc = NULL;

	if (FAILED(pD3DDevice->CreateImageSurface(width, height, D3DFMT_A8R8G8B8, &pDst)))
		return false;

	POINT	pt = {0, };
	RECT	rt = {0, };

	pt.x = 0;
	pt.y = 0;
	rt.right = width;
	rt.bottom = height;

	if (FAILED(((LPDIRECT3DTEXTURE8)tex)->GetSurfaceLevel(0, &pSrc)))
		goto failed_return;

	if (FAILED(pD3DDevice->CopyRects(pSrc, &rt, 1, pDst, &pt)))
		goto failed_return;

	pSrc->Release();
	pSrc = NULL;

	//	This function supports the following file formats: .bmp and .dds.
	if (SUCCEEDED(D3DXSaveSurfaceToFileW(filename, D3DXIFF_BMP, pDst, NULL, NULL)))
	{
		pDst->Release();
		pDst = NULL;
		return true;
	}
	
failed_return:
	if (pDst)
	{
		pDst->Release();
		pDst = NULL;
	}
	if (pSrc)
	{
		pSrc->Release();
		pSrc = NULL;
	}
	return false;
}

bool MRbInput::OnFocus()
{
	return pHGE->bOnFocus;
}

int MRbInput::MouseWheel()
{   
	return pHGE->mouseWheel;
}

int MRbInput::MouseDblClk(int iKey)
{
	return pHGE->mouseButton & (iKey<<6);
}

//int MRbInput::GetMouseMove()
//{
//	return pHGE->mouseMove;
//}

void MRbSinCore::Freeze()
{
	if (pHGE->freezeTex) pHGE->Texture_Free(pHGE->freezeTex);
	pHGE->freezeTex = 0;
	pHGE->freezeTex = pHGE->Texture_CreateFromScreen();
	pHGE->bFreeze = true;
}

void MRbSinCore::Transition(int duration, const wchar_t * filename, float vague)
{
	if (!pHGE->freezeTex || !pHGE->bFreeze) return;

	if (duration <= 0)
	{
		pHGE->bFreeze = false;
		GetAppPtr()->GraphicsUpdate();
		return;
	}
	
	HTEXTURE newTex = pHGE->Texture_CreateFromScreen();
	if (!newTex) return;
	float tempx1, tempy1, tempx2, tempy2;

	tempx1 = 0;
	tempy1 = 0;
	tempx2 = pHGE->nScreenWidth;
	tempy2 = pHGE->nScreenHeight;

	hgeQuad frzQuad, newQuad;
	frzQuad.tex = pHGE->freezeTex;
	QUAD_INIT(frzQuad);
	QUAD_SET_VRECT(frzQuad, tempx1, tempy1, tempx2, tempy2);

	newQuad.tex = newTex;
	QUAD_INIT(newQuad);
	QUAD_SET_VRECT(newQuad, tempx1, tempy1, tempx2, tempy2);

	if (filename[0])
	{
		int suffix_idx;
		HTEXTURE midTex = CRbBitmap::LoadTexture(filename, 0, suffix_idx);

		int mw = pHGE->Texture_GetWidth(midTex);
		int mh = pHGE->Texture_GetHeight(midTex);
		float a2, gray = 0, rate = (float)(255.0 / duration);
		int dw = pHGE->nScreenWidth < mw ? pHGE->nScreenWidth : mw;
		int dh = pHGE->nScreenHeight < mh ? pHGE->nScreenHeight : mh;
		int wrate = pHGE->nScreenWidth / mw;
		int hrate = pHGE->nScreenHeight / mh;
		
		BYTE a1, r1, g1, b1, r2;
		DWORD * pMidTexData = pHGE->Texture_Lock(midTex, true);
		pHGE->bFreeze = false;
		if (vague > 1)
		{
			do
			{
				DWORD* pNewTexData = pHGE->Texture_Lock(newTex, false);
				for (int ly = 0; ly < dh; ++ly)
				{
					int tempW1 = ly * mw;
					for (int lx = 0; lx < dw; ++lx)
					{
						r2 = GET_ARGB_R(pMidTexData[tempW1 + lx]);
						if ((BYTE)gray < r2)
						{
							// 计算透明度，这算法我自己都不知道如何吐槽了 = =b
							a2 = 255.0 - 255.0 / vague * (r2 - gray);
							if (vague < 24) a2 += (12 + rate * rate) * (16 - vague);
							else a2 += 12 + rate * rate;
							a2 = SinBound(a2, 0, 255);
							
							if (wrate && hrate)
							{
								for (int j = 0; j < hrate + 1; ++j)
								{
									if ((pHGE->nScreenHeight - mh * j) > ly)
									{
										int tempH2 = (ly + mh * j) * pHGE->nScreenWidth;
										GET_ARGB_8888(pNewTexData[tempH2 + lx], a1, r1, g1, b1);
										pNewTexData[tempH2 + lx] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);

										for (int i = 1; i < wrate + 1; ++i)
										{
											int tempW3 = mw * i;
											if ((pHGE->nScreenWidth - tempW3) > lx)
											{
												GET_ARGB_8888(pNewTexData[tempH2 + lx + tempW3], a1, r1, g1, b1);
												pNewTexData[tempH2 + lx + tempW3] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);
											}
										}
									}
								}
							}
							else if (wrate)
							{
								int tempH2 = ly * pHGE->nScreenWidth;
								GET_ARGB_8888(pNewTexData[tempH2 + lx], a1, r1, g1, b1);
								pNewTexData[tempH2 + lx] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);

								for (int i = 1; i < wrate + 1; ++i)
								{
									int tempW3 = mw * i;
									if ((pHGE->nScreenWidth - tempW3) > lx)
									{
										GET_ARGB_8888(pNewTexData[tempH2 + lx + tempW3], a1, r1, g1, b1);
										pNewTexData[tempH2 + lx + tempW3] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);
									}
								}
							}
							else if (hrate)
							{
								for (int j = 0; j < hrate + 1; ++j)
								{
									if ((pHGE->nScreenHeight - mh * j) > ly)
									{
										int tempH2 = (ly + mh * j) * pHGE->nScreenWidth;
										GET_ARGB_8888(pNewTexData[tempH2 + lx], a1, r1, g1, b1);
										pNewTexData[tempH2 + lx] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);

										GET_ARGB_8888(pNewTexData[tempH2 + lx], a1, r1, g1, b1);
										pNewTexData[tempH2 + lx] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);
									}
								}
							}
							else
							{
								int tempH2 = ly * pHGE->nScreenWidth;
								GET_ARGB_8888(pNewTexData[tempH2 + lx], a1, r1, g1, b1);
								pNewTexData[tempH2 + lx] = MAKE_ARGB_8888((BYTE)a2, r1, g1, b1);
							}
						}
					}
				}
				pHGE->Texture_Unlock(newTex);

				GetAppPtr()->SystemUpdate();
				if(pHGE->bActive || pHGE->bDontSuspend)
				{
					pHGE->Gfx_BeginScene();
					pHGE->Gfx_RenderQuad(&frzQuad);
					pHGE->Gfx_RenderQuad(&newQuad);
					pHGE->Gfx_EndScene();

					if (pHGE->bShowFps)
					{
						wsprintfW(pHGE->szTitleFps, L"%s - %d FPS", pHGE->szWinTitle, GetAppPtr()->GetRealFps());
						SetWindowText(pHGE->hwnd, pHGE->szTitleFps);
					}
				}
				GetAppPtr()->LimitFps(pHGE->nHGEFPS);
				duration--;
				gray += rate;
			} while (duration);
		}
		else
		{
			do
			{
				gray += rate;

				DWORD * pFrzTexData = pHGE->Texture_Lock(pHGE->freezeTex, false);
				for (int ly = 0; ly < dh; ++ly)
				{
					int tempW1 = ly * mw;
					int tempW2 = ly * pHGE->nScreenWidth;
					for (int lx = 0; lx < mw; ++lx)
					{
						r2 = GET_ARGB_R(pMidTexData[tempW1 + lx]);
						if ((BYTE)gray > r2)
						{
							pFrzTexData[tempW2 + lx] = 0;

							if ((pHGE->nScreenWidth - mw) > lx)
							{
								pFrzTexData[tempW2 + lx + mw] = 0;
							}
						
							if ((pHGE->nScreenHeight - mh) > ly)
							{
								pFrzTexData[(ly + mh) * pHGE->nScreenWidth + lx] = 0;

								if ((pHGE->nScreenWidth - mw) > lx)
									pFrzTexData[(ly + mh) * pHGE->nScreenWidth + lx + mw] = 0;
							}
						}
					}
				}
				pHGE->Texture_Unlock(pHGE->freezeTex);

				GetAppPtr()->SystemUpdate();
				if(pHGE->bActive || pHGE->bDontSuspend)
				{
					pHGE->Gfx_BeginScene();
					pHGE->Gfx_RenderQuad(&newQuad);
					pHGE->Gfx_RenderQuad(&frzQuad);
					pHGE->Gfx_EndScene();

					if (pHGE->bShowFps)
					{
						wsprintfW(pHGE->szTitleFps, L"%s - %d FPS", pHGE->szWinTitle, GetAppPtr()->GetRealFps());
						SetWindowText(pHGE->hwnd, pHGE->szTitleFps);
					}
				}
				GetAppPtr()->LimitFps(pHGE->nHGEFPS);
				duration--;
			} while (duration);
		}
		pHGE->Texture_Free(midTex);
	}
	else
	{
		float rate = 255.0 / duration;
		float al = 0;
		pHGE->bFreeze = false;
		do
		{
			al += rate;
			newQuad.v[0].col =
			newQuad.v[1].col =
			newQuad.v[2].col =
			newQuad.v[3].col = 0x00ffffff + ((BYTE)al << 24);

			GetAppPtr()->SystemUpdate();
			if(pHGE->bActive || pHGE->bDontSuspend)
			{
				pHGE->Gfx_BeginScene();
				//pHGE->Gfx_Clear(0);
				pHGE->Gfx_RenderQuad(&frzQuad);
				pHGE->Gfx_RenderQuad(&newQuad);
				pHGE->Gfx_EndScene();

				if (pHGE->bShowFps)
				{
					wsprintfW(pHGE->szTitleFps, L"%s - %d FPS", pHGE->szWinTitle, GetAppPtr()->GetRealFps());
					SetWindowText(pHGE->hwnd, pHGE->szTitleFps);
				}
			}
			GetAppPtr()->LimitFps(pHGE->nHGEFPS);
			duration--;
		} while (duration);
	}
	pHGE->Texture_Free(newTex);
	newTex = 0;
	pHGE->Texture_Free(pHGE->freezeTex);
	pHGE->freezeTex = 0;
}

void MRbSinCore::HideMouse(bool hide)
{
	pHGE->bHideMouse = hide;
}

VALUE MRbSinCore::get_show_mouse()
{
	if (!GetAppPtr()->IsInited())
		rb_raise(rb_eSinError, "SINRGE2 has not ninited.");

	return C2RbBool(!pHGE->bHideMouse);
}
