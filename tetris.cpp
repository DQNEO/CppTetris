#include <windows.h>

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
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

    return 0;
}
