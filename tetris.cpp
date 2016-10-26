#include "GameMaster.h"

HINSTANCE hInstance;
HWND hMainWindow;

GameMaster GM;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {            
            HDC hdc = GetDC(hWnd);            
			GM.initialize(hdc,hInstance);	
            ReleaseDC(hWnd, hdc);
            break;
        }

        case WM_TIMER: {			
			GM.Update();            
            InvalidateRect(hWnd, NULL, false);
            break;
			
        }

        case WM_PAINT: {			
			HDC MemDC;
			MemDC = GM.showBoard();

            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc, 0, 0, 24 * 10, 24 * 20, MemDC, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
			
            break;
        }

        case WM_DESTROY: {			
			GM.Destory();
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

    wc.cbSize        = sizeof(WNDCLASSEX);              
    wc.style         = CS_HREDRAW | CS_VREDRAW;          
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;                                
    wc.cbWndExtra    = 0;                                
    wc.hInstance     = hInst;                            
    wc.hIcon         = NULL;                             
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);      
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);         
    wc.lpszMenuName  = NULL;                             
    wc.lpszClassName = pClassName;                      
    wc.hIconSm       = NULL;                            

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

    SetTimer(hMainWindow, 100, 1000 /30, NULL);
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    KillTimer(hMainWindow, 100);

    return 0;
}
