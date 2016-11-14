#include "GameMaster.h"

HINSTANCE hInstance;
#pragma warning(disable: 4996)

extern int score;
extern int combo;

void printscore(HDC hwnd);
void ComboPrint(HDC hwnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static int FrameCount = 0;
	static bool keyboardUpFalg = true;

	switch (msg) {
	case WM_CREATE:{
		HDC hdc = GetDC(hWnd);
		initialize();
		MakeDCformBitmaps(hdc, hInstance);
		ReleaseDC(hWnd, hdc);

		if (DEBUG_MODE == true){
			AllocConsole();
			freopen("CONOUT$", "wt", stdout);
			printf("Hello, World! in Win32 API Application...\n");
		}
		break;
	}		

	case WM_KEYUP:{
		switch (wParam){
		case VK_UP:
		case VK_SPACE:
			keyboardUpFalg = true;
		}
		break;
	}

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

	case WM_TIMER:{
		FrameCount++;
		if (FrameCount % 8 == 7){
			Update();
			FrameCount = 0;
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}

	case WM_PAINT:{
		HDC MemDC;
		MemDC = showBoard();



		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);

		printscore(hdc);
		ComboPrint(hdc);

		BitBlt(hdc, 0, 0, 24 * 10, 24 * 20, MemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);

		break;
	}
	case WM_COMMAND:
		if (((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED))
		{
			switch (LOWORD(wParam))
			{
			case 10000:
				KillTimer(hWnd, 100);
				if (MessageBox(hWnd, "게임을 계속 하시겠습니까?", "일시 정지", MB_YESNO) == IDYES)
					SetTimer(hWnd, 100, 1000 / 30, NULL);
				else
					gameOver();
				break;

			case 10001:
				KillTimer(hWnd, 100);
				if (MessageBox(hWnd, "게임을 재시작 하시겠습니까?", "재시작", MB_YESNO) == IDYES){
					initialize();
				}
				SetTimer(hWnd, 100, 1000 / 30, NULL);
				break;
			}
		}
		break;

	case WM_DESTROY:
		Destory();

		if (DEBUG_MODE == true)
			FreeConsole();

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



void printscore(HDC hwnd)
{
	char imsistr[128];
	wsprintf(imsistr, "점수 : %d", score);
	TextOut(hwnd, 260, 180, imsistr, strlen(imsistr));
}

void ComboPrint(HDC hwnd)
{
	char imsistr[128];
	wsprintf(imsistr,"콤보 : %d",combo);
	TextOut(hwnd, 260, 200, imsistr, strlen(imsistr));
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = pClassName;
	wc.hIconSm = NULL;

	if (!RegisterClassEx(&wc)) return FALSE;

	RECT r;
	r.left = r.top = 0;
	r.right = 24 * 17;
	r.bottom = 24 * 20;
	AdjustWindowRectEx(&r, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, false, 0);

	HWND hMainWindow = CreateWindow(pClassName, "Nico Nico Programming2", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, 350, r.bottom - r.top, NULL, NULL, hInst, NULL);

	HWND hMainWindow_button;
	hMainWindow_button = CreateWindow(TEXT("button"), TEXT("▶"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		24 * 10 + 30, 10, 50, 50, hMainWindow, (HMENU)10000, hInstance, NULL);

	HWND hMainWindow_button1;
	hMainWindow_button1 = CreateWindow(TEXT("button"), TEXT("Restart"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		24 * 10 + 20, 70, 70, 50, hMainWindow, (HMENU)10001, hInstance, NULL);

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
