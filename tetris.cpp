#include <windows.h>

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
    WNDCLASSEX wc;
    static LPCTSTR pClassName = "NicoNicoProgramming2";  // クラス名

    wc.cbSize        = sizeof(WNDCLASSEX);               // 構造体サイズ
    wc.style         = CS_HREDRAW | CS_VREDRAW;          // クラススタイル
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;                                // 補足メモリブロック
    wc.cbWndExtra    = 0;                                // 　のサイズ
    wc.hInstance     = hInst;                            // インスタンス
    wc.hIcon         = NULL;                             // アイコン
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);       // カーソル
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);         // 背景色
    wc.lpszMenuName  = NULL;                             // メニュー
    wc.lpszClassName = pClassName;                       // クラス名
    wc.hIconSm       = NULL;                             // 小さいアイコン

    if (!RegisterClassEx(&wc)) return FALSE;             // 登録

    return 0;
}
