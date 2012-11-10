#include <windows.h>

HINSTANCE hInstance;
HWND hMainWindow;

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            HDC hdc = GetDC(hWnd);
            
            hMemDC = CreateCompatibleDC(hdc);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
            hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);
            
            
            ReleaseDC(hWnd, hdc);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
    hInstance = hInst;
    WNDCLASSEX wc;
    static LPCTSTR pClassName = "NicoNicoProgramming2";  // �N���X��

    wc.cbSize        = sizeof(WNDCLASSEX);               // �\���̃T�C�Y
    wc.style         = CS_HREDRAW | CS_VREDRAW;          // �N���X�X�^�C��
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;                                // �⑫�������u���b�N
    wc.cbWndExtra    = 0;                                // �@�̃T�C�Y
    wc.hInstance     = hInst;                            // �C���X�^���X
    wc.hIcon         = NULL;                             // �A�C�R��
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);       // �J�[�\��
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);         // �w�i�F
    wc.lpszMenuName  = NULL;                             // ���j���[
    wc.lpszClassName = pClassName;                       // �N���X��
    wc.hIconSm       = NULL;                             // �������A�C�R��

    if (!RegisterClassEx(&wc)) return FALSE;             // �o�^

    RECT r;
    r.left = r.top = 0;
    r.right = 24 * 10;
    r.bottom = 24 * 20;
    AdjustWindowRectEx(&r, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, false, 0);

    hMainWindow = CreateWindow(pClassName, "Nico Nico Programming2", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.left,
        NULL, NULL, hInst, NULL);
    
    ShowWindow(hMainWindow, SW_SHOW);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
