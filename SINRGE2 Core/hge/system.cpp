/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/

#include "RbInput.h"
#include "nge_timer.h"
#include "hge_impl.h"


#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))


const wchar_t *WINDOW_CLASS_NAME = L"SINRGE2_WNDCLASS";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int			nRef=0;
HGE_Impl*	pHGE=0;

static int		onFocus				= 1;
static int		mouseButton			= 0;
static short	mouseWheel			= 0;
static bool		mouseMove			= false;


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
	//OSVERSIONINFO	os_ver;
	//SYSTEMTIME		tm;
	//MEMORYSTATUS	mem_st;
	WNDCLASS		winclass;
	int				width, height;

	// Log system info

	//System_Log(L"HGE Started..\n");

	//System_Log(L"HGE version: %X.%X", HGE_VERSION>>8, HGE_VERSION & 0xFF);
	//GetLocalTime(&tm);
	//System_Log(L"Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond);

	//System_Log(L"Application: %s",szWinTitle);
	//os_ver.dwOSVersionInfoSize=sizeof(os_ver);
	//GetVersionEx(&os_ver);
	//System_Log(L"OS: Windows %ld.%ld.%ld",os_ver.dwMajorVersion,os_ver.dwMinorVersion,os_ver.dwBuildNumber);

	//GlobalMemoryStatus(&mem_st);
	//System_Log(L"Memory: %ldK total, %ldK free\n",mem_st.dwTotalPhys/1024L,mem_st.dwAvailPhys/1024L);


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
	/*if(szIcon) winclass.hIcon = LoadIcon(hInstance, szIcon);
	else winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);*/
	
	if (!RegisterClass(&winclass)) {
		_PostError(L"Can't register window class");
		return false;
	}

	// Create window

	width = nScreenWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	height = nScreenHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	rectW.left = (GetSystemMetrics(SM_CXSCREEN)-width) / 2;
	rectW.top = (GetSystemMetrics(SM_CYMAXIMIZED) - nScreenHeight) / 2 - GetSystemMetrics(SM_CYCAPTION);;//(GetSystemMetrics(SM_CYSCREEN)-height) / 2 - GetSystemMetrics(SM_CYCAPTION);
	rectW.right = rectW.left + width;
	rectW.bottom = rectW.top +height;
	styleW = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

	rectFS.left = 0;
	rectFS.top = 0;
	rectFS.right = nScreenWidth;
	rectFS.bottom = nScreenHeight;
	styleFS = WS_POPUP | WS_VISIBLE; //WS_POPUP

	/*if (hwndParent)
	{
		rectW.left = 0;
		rectW.top = 0;
		rectW.right = nScreenWidth;
		rectW.bottom = nScreenHeight;
		styleW = WS_CHILD | WS_VISIBLE; 
		bWindowed = true;
	}*/

	if (bWindowed)
		hwnd = CreateWindowEx(WS_EX_WINDOWEDGE, WINDOW_CLASS_NAME, szWinTitle, styleW,
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
	_InitPowerStatus();
	if(!_GfxInit()) { System_Shutdown(); return false; }

	//System_Log(L"Init done.\n");

	fTime=0.0f;
	t0=t0fps=timeGetTime();
	dt=cfps=0;
	nFPS=0;

	// Done

	return true;
}

void CALL HGE_Impl::System_Shutdown()
{
	//System_Log(L"\nFinishing..");

	timeEndPeriod(1);
	_GfxDone();
	_DonePowerStatus();

	if(hwnd)
	{
		DestroyWindow(hwnd);
		hwnd=0;
	}

	if(hInstance) UnregisterClass(WINDOW_CLASS_NAME, hInstance);

	//System_Log(L"The End.");
}

bool CALL HGE_Impl::System_Update()
{
	// Process window messages
	if(PeekMessage(&m_msg,NULL,0,0,PM_REMOVE)){
		if(m_msg.message == WM_QUIT){
			bActive=false;
			return false;
		}
		DispatchMessage(&m_msg);
	}
	// If HGE window is focused or we have the "don't suspend" state - process the main loop
	if(bActive || bDontSuspend)
	{
		if(procRenderFunc)
			procRenderFunc();

		LimitFps(nHGEFPS);
	}
	return true;
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
		case HGE_WINDOWED:		if(VertArray || hwndParent) break;
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

void CALL HGE_Impl::System_SetStateHwnd(hgeHwndState state, HWND value)
{
	switch(state)
	{
		case HGE_HWNDPARENT:	if(!hwnd) hwndParent=value; break;
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
								if(nHGEFPS>0) nFixedDelta=int(1000.0f/value);
								else nFixedDelta=0;
								break;
	}
}

void CALL HGE_Impl::System_SetStateString(hgeStringState state, const wchar_t *value)
{
	FILE *hf;
	
	switch(state)
	{
		case HGE_TITLE:			wcscpy_s(szWinTitle,value);
								if(pHGE->hwnd) SetWindowText(pHGE->hwnd, szWinTitle);
								break;
		case HGE_INIFILE:		if(value)
									wcscpy_s(szIniFile,Resource_MakePath(value));
									//wcscpy_s(szIniFile, ResManager::Instance()->Resource_MakePath(value));
								else szIniFile[0]=0;
								break;
		case HGE_LOGFILE:		if(value)
								{
									wcscpy_s(szLogFile,Resource_MakePath(value));
									//wcscpy_s(szIniFile, ResManager::Instance()->Resource_MakePath(value));
									if(_wfopen_s(&hf, szLogFile, L"w")) szLogFile[0]=0;
									else fclose(hf);
								}
								else szLogFile[0]=0;
								break;
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
		case HGE_HWNDPARENT:	return hwndParent;
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
		case HGE_POWERSTATUS:	return nPowerStatus;
	}

	return 0;
}

const wchar_t* CALL HGE_Impl::System_GetStateString(hgeStringState state) {
	switch(state) {
		case HGE_TITLE:			return szWinTitle;
		case HGE_INIFILE:		if(szIniFile[0]) return szIniFile;
								else return 0;
		case HGE_LOGFILE:		if(szLogFile[0]) return szLogFile;
								else return 0;
	}

	return NULL;
}

wchar_t* CALL HGE_Impl::System_GetErrorMessage()
{
	return szError;
}

void CALL HGE_Impl::System_Log(const wchar_t *szFormat, ...)
{
	FILE *hf = NULL;
	va_list ap;
	
	if(!szLogFile[0])
	{
		static wchar_t szError[1024];

		va_start(ap, szFormat);
		vswprintf_s(szError, szFormat, ap);
		va_end(ap);

		wprintf(szError);
		return;
	}

	if(_wfopen_s(&hf, szLogFile, L"a")) return;

	va_start(ap, szFormat);
	vfwprintf(hf, szFormat, ap);
	va_end(ap);

	fprintf(hf, "\n");

	fclose(hf);
}

bool CALL HGE_Impl::System_Launch(const wchar_t *url)
{
	if((DWORD)ShellExecute(pHGE->hwnd, NULL, url, NULL, NULL, SW_SHOWMAXIMIZED)>32) return true;
	else return false;
}

bool CALL HGE_Impl::System_Snapshot(const DWORD* pData, int& width, int& height)
{
	HDC hScreenDC = GetDC(NULL);
    HDC hCaptureDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hScreenDC, nScreenWidth, nScreenHeight); 
    SelectObject(hCaptureDC, hCaptureBitmap);
    BitBlt(hCaptureDC, 0, 0, nScreenWidth, nScreenHeight, hScreenDC, 0, 0, SRCCOPY); 
	
	BITMAP bitmap;
	GetObject(hCaptureBitmap, sizeof(BITMAP), &bitmap);
	if (!bitmap.bmBits) goto failed_return;

	//if (bitmap.bmBitsPixel = 32)
	printf("bitmap.bmBitsPixel %d", bitmap.bmBitsPixel);

	width = bitmap.bmWidth;
	height = bitmap.bmHeight;

	//pData = (DWORD*)malloc(width * height * sizeof(DWORD));
	memcpy(&pData, bitmap.bmBits, width * height * sizeof(DWORD));
	/*for (int i = 0 ; i < height ; ++i)
	{
		pData[]*/
		/*memcpy( (void*)(pData + (nScreenHeight - i) * nScreenWidth * sizeof(DWORD)),
			(&lockedRect.pBits + i * lockedRect.Pitch), nScreenWidth * sizeof(DWORD));*/
	//}

    ReleaseDC(NULL, hScreenDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);

	return true;

failed_return:
	if (hScreenDC)
		ReleaseDC(NULL, hScreenDC);
	if (hCaptureDC)
		DeleteDC(hCaptureDC);
	if (hCaptureBitmap)
		DeleteObject(hCaptureBitmap);

	return false;
//	if(!pD3DDevice)
//		return false;
//
//	LPDIRECT3DSURFACE8 pSurf;
//	//pD3DDevice->CreateImageSurface(nScreenWidth, nScreenHeight, D3DFMT_A8R8G8B8, &pSurf);
//	if (FAILED(pD3DDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurf)))
//	//if (FAILED(pD3DDevice->GetFrontBuffer(pSurf)))
//		goto __failed_return;
//
//	D3DSURFACE_DESC surfaceDesc;
//	if (FAILED(pSurf->GetDesc(&surfaceDesc)))
//		goto __failed_return;
//	
//	width = surfaceDesc.Width;
//	height = surfaceDesc.Height;
//	
//
//	//D3DXSaveSurfaceToFile(L"snap.bmp", D3DXIFF_BMP, pSurf, NULL, NULL);
//
//	D3DLOCKED_RECT lockedRect;
//	if (FAILED(pSurf->LockRect(&lockedRect, 0, D3DLOCK_READONLY)))
//		goto __failed_return;
//	
//	pData = (DWORD*)malloc(width * height * sizeof(DWORD));
//	/*for (int i = 0 ; i < nScreenHeight ; ++i)
//	{
//		memcpy( (void*)(pData + (nScreenHeight - i) * nScreenWidth * sizeof(DWORD)),
//			(&lockedRect.pBits + i * lockedRect.Pitch), nScreenWidth * sizeof(DWORD));
//	}*/
//	
//	pSurf->UnlockRect();
//	pSurf->Release();
//	return true;
//
//__failed_return:
//	if (pSurf)
//		pSurf->Release();
//	return false;
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

	nHGEFPS=HGEFPS_UNLIMITED;
	fTime=0.0f;
	fDeltaTime=0.0f;
	nFPS=0;
	
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
	szLogFile[0]=0;
	szIniFile[0]=0;
	
	nFixedDelta=0;
	bHideMouse=true;
	bDontSuspend=false;
	hwndParent=0;

	nPowerStatus=HGEPWR_UNSUPPORTED;
	hKrnl32 = NULL;
	lpfnGetSystemPowerStatus = NULL;
	
	GetModuleFileName(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
	int i;
	for(i=wcslen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
	szAppPath[i+1]=0;
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

	mouseMove = false;

	switch(msg)
	{	
		
        case WM_KILLFOCUS:
			onFocus = 0;
			return FALSE;
        case WM_SETFOCUS:
			onFocus = 1;
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
			if(wparam == VK_F4)
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

        case WM_LBUTTONDBLCLK:
			mouseButton += 64;
			return FALSE;
        case WM_RBUTTONDBLCLK:
			mouseButton += 128;
			return FALSE;
        case WM_MBUTTONDBLCLK:
			mouseButton += 256;
			return FALSE;
			
		case WM_MOUSEMOVE:
			mouseMove = true;
			return FALSE;
        case WM_MOUSEWHEEL:
            mouseWheel = (short)HIWORD(wparam);
			return FALSE;

		case WM_SIZE:
			if(pHGE->pD3D && wparam==SIZE_RESTORED) pHGE->_Resize(LOWORD(lparam), HIWORD(lparam));
			//return FALSE;
			break;

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

int OnFocus()
{
	return onFocus;
}

int MouseWheel()
{   
	return mouseWheel;
}

int MouseDblClk(int iKey)
{
	return mouseButton & (iKey<<6);
}

int GetMouseMove()
{
	return mouseMove;
}

void HideMouse(bool hide)
{
	pHGE->bHideMouse = hide;
}

