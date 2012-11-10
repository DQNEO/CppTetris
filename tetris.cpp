#include <windows.h>

HINSTANCE hInstance;
HWND hMainWindow;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
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

    hMainWindow = CreateWindow(pClassName, "Nico Nico Programming2", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT, TODO, TODO,
        NULL, NULL, hInst, NULL);
    
    ShowWindow(SW_SHOW);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
