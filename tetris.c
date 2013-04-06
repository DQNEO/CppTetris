#include <windows.h>
#include "tetris.h"

/* ���������֐� */
int random(int max) {
    return (int)(rand() / (RAND_MAX + 1.0) * max);
}

/* �u���b�N��u���Ă݂鏈�� */
BOOL putBlock(STATUS s, BOOL action) {
    int i,j;

    if(board[s.x][s.y] != 0) {
        return FALSE;
    }


    if(action) {
        board[s.x][s.y] = s.type;
    }


    for(i = 0; i < 3; i++) {
        int dx = block[s.type].p[i].x;
        int dy = block[s.type].p[i].y;
        int r = s.rotate % block[s.type].rotate;
        for(j = 0; j < r; j++) {
            int nx = dx, ny = dy;
            dx = ny; dy = -nx;
        }
        if(board[s.x + dx][s.y + dy] != 0) {
            return FALSE;
        }
        if(action) {
            board[s.x + dx][s.y + dy] = s.type;
        }
    }
    if(!action) {
        putBlock(s, TRUE);
    }
    return TRUE;
}

/* �J�����g�u���b�N����x���� */
BOOL deleteBlock(STATUS s) {
    int i,j;
    board[s.x][s.y] = 0;

    for(i = 0; i < 3; i++) {
        int dx = block[s.type].p[i].x;
        int dy = block[s.type].p[i].y;
        int r = s.rotate % block[s.type].rotate;
        for(j = 0; j < r; j++) {
            int nx = dx, ny = dy;
            dx = ny; dy = -nx;
        }
        board[s.x + dx][s.y + dy] = 0;
    }

    return TRUE;
}

/* ��������ɒi������������ɍ폜���鏈�� */
void deleteLine() {
    int y,x,i,j;
    for(y = 1; y < 23; y++) {
        BOOL flag = TRUE;
        for(x = 1;x <= 10; x++) {
            if(board[x][y] == 0) {
                flag = FALSE;
            }
        }
        
        if(flag) {
            for(j = y; j < 23; j++) {
                for(i = 1; i <= 10; i++) {
                    board[i][j] = board[i][j + 1];
                }
            }
            y--;
        }
    }
}

/* �J�����g�u���b�N�̂P�}�X���R���� */
void blockDown() {
    //���݈ʒu�̃u���b�N����x������
    deleteBlock(current);
    //Y���W���P������
    current.y--;

    //�u���b�N��u���Ă݂�B
    //�u������P�}�X���R�������������Ƃ������ƁB
    if(putBlock(current, FALSE)) {
        return;
    }

    current.y++;
    putBlock(current, FALSE);
    
    deleteLine();
    
    current.x = 5;
    current.y = 21;
    current.type = random(7) + 1;
    current.rotate = random(4);
    if(!putBlock(current, FALSE)) {
        gameOver();
    }
    
}

/**
 * �Q�[���I�[�o�[�ɂȂ�����Ă΂�鏈��
 * 
 * �E�^�C�}�[������
 * �E�S�u���b�N��Ԃ��h��
 * �E��ʂ��ĕ`��
 */
void gameOver() {
    int x,y;
    KillTimer(hMainWindow, 100);
    for(x = 1; x <= 10;x++) {
        for(y = 1; y <= 20; y++) {
            if(board[x][y] != 0) {
                board[x][y] = 1;
            }
        }
    }
    InvalidateRect(hMainWindow, NULL, FALSE);
}

/* ���[�U����̃L�[���͂���舵�� */
BOOL processInput() {
    BOOL ret = FALSE;
    STATUS n = current;
    if(GetAsyncKeyState(VK_LEFT)) {
        n.x--;
    } else if(GetAsyncKeyState(VK_RIGHT)) {
        n.x++;
    } else if(GetAsyncKeyState(VK_UP)) {
        n.y++;
    } else if(GetAsyncKeyState(VK_SPACE)) {
        n.rotate++;
    } else if(GetAsyncKeyState(VK_DOWN)) {
        n.y--;
        ret = TRUE;
    }

    if(n.x != current.x || n.y != current.y || n.rotate != current.rotate) {
        deleteBlock(current);
        if(putBlock(n, FALSE)) {
            current = n;
        } else {
            putBlock(current, FALSE);
        }
    }
    
    return ret;
}


/* �Ֆʃf�[�^����ʂɕ`�悷�� */
void showBoard() {
    int x,y;
    for(x = 1; x <= 10; x++) {
        for(y = 1; y <= 20; y++) {
            BitBlt(hMemDC, (x - 1) * 24, (20 -y) * 24, 24, 24, hBlockDC, 0, board[x][y] * 24, SRCCOPY);
        }
    }
}

/**
 * �C�x���g�������ɌĂ΂��R�[���o�b�N�֐�
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            int x,y;
            for(x = 0; x < 12; x++) {
                for(y = 0; y < 25; y++) {
                    if(x == 0 || x == 11 || y == 0) {
                        board[x][y] = 1;
                    } else {
                        board[x][y] = 0;
                    }
                }
            }
            
            HDC hdc = GetDC(hWnd);
            
            hMemDC = CreateCompatibleDC(hdc);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
            hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);
            
            hBlockDC = CreateCompatibleDC(hdc);
            hBitmap = LoadBitmap(hInstance, "BLOCKS");
            hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);
            
            // debug
            BitBlt(hMemDC, 0, 0, 24, 24, hBlockDC, 0, 0, SRCCOPY);
            
            ReleaseDC(hWnd, hdc);
            break;
        }
        case WM_TIMER: {
            static int w = 0;
            if(w % 2 == 0) {
                if(processInput()) {
                    w = 0;
                }
            }
            if(w % 5 == 0) {
                blockDown();
            }
            w++;
            
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }

        case WM_PAINT: {
            showBoard();
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            BitBlt(hdc, 0, 0, 24 * 10, 24 * 20, hMemDC, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_DESTROY: {
            HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hMemPrev);
            DeleteObject(hBitmap);
            DeleteObject(hMemDC);
            
            hBitmap = (HBITMAP)SelectObject(hBlockDC, hBlockPrev);
            DeleteObject(hBitmap);
            DeleteObject(hBlockDC);
            
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

/**
 * �A�v���P�[�V�����̋N������
 * ���g�͂قƂ�ǂ��܂��Ȃ��B
 */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
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
    AdjustWindowRectEx(&r, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, FALSE, 0);

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

