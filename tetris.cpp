#include "GameMaster.h"

HINSTANCE hInstance;
HWND hMainWindow;
#pragma warning(disable: 4996)

/**********************
이건 임시 전역 변수에요
전역 변수 없애야 해요 
static 써야 하나요 
*********************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	static int FrameCount = 0;
	static bool keyboardUpFalg = true;

	switch (msg) {
	case WM_CREATE: {
		HDC hdc = GetDC(hWnd);
		initialize(hdc, hInstance);
		ReleaseDC(hWnd, hdc);
		
		if (DEBUG_MODE == true){
			AllocConsole();
			freopen("CONOUT$", "wt", stdout);
			printf("Hello, World! in Win32 API Application...\n");
		}

		break;     
	}
	case WM_KEYUP:
		switch (wParam){
		case VK_UP:
		case VK_SPACE:
			keyboardUpFalg = true;
		}
		break;

	case WM_KEYDOWN:{
		switch (wParam){
		case VK_UP:
		case VK_SPACE:
			if (keyboardUpFalg == false)
				break;
			keyboardUpFalg = false;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DOWN:
			bool keyDownResult;
			keyDownResult = processInput(wParam);
			if (keyDownResult){
				Update();
				FrameCount = 0;
			}
			break;
		}
		break;
	}

	case WM_TIMER: {
 		FrameCount++;
		if (FrameCount % 8 == 7)
		{
			Update();
			FrameCount = 0;
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	}

	case WM_PAINT: {
		HDC MemDC;
		MemDC = showBoard();

		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, 24 * 10, 24 * 20, MemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);

		break;
	}

	case WM_DESTROY: {
		Destory();
		
		if (DEBUG_MODE == true)
			FreeConsole();

		PostQuitMessage(0);
		break;
	}

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
	hInstance = hInst;
	WNDCLASSEX wc;
	static LPCTSTR pClassName = "NicoNicoProgramming2";

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = pClassName;
	wc.hIconSm = NULL;

	if (!RegisterClassEx(&wc)) return FALSE;

	RECT r;
	r.left = r.top = 0;
	r.right = 24 * 10;
	r.bottom = 24 * 20;
	AdjustWindowRectEx(&r, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, false, 0);

	hMainWindow = CreateWindow(pClassName, "Nico Nico Programming2", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top,
		NULL, NULL, hInst, NULL);

	ShowWindow(hMainWindow, SW_SHOW);

	SetTimer(hMainWindow, 100, 1000 / 30, NULL);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hMainWindow, 100);

	return 0;
}
