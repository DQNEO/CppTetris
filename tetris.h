/* Windowsおまじない系のグローバル変数 */
HINSTANCE hInstance;
HWND hMainWindow;

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

/* 構造体の宣言 */

/**
 * 位置を表す構造体
 */
typedef struct _TAG_POSITION {
    int x;
    int y;
} POSITION;

/**
 * ブロックを表す構造体
 */
typedef struct _TAG_BLOCK {
    int rotate;
    POSITION p[3];
} BLOCK;


/**
 * 現在操作中のブロックを表す構造体
 */
typedef struct _TAG_STATUS {
    int x;
    int y;
    int type;
    int rotate;
} STATUS;

/* グローバル変数の宣言 */

/**
 * 盤面(12x25)データ構造体
 * 0:黒
 * 1-7:ブロックのタイプ
 */
int board[12][25];

/** 現在操作中のブロック */
STATUS current;

/**
 * ブロックの全種類を定義する
 * (ただし0は黒背景を表す)
 */
BLOCK block[8] = {
    {1, {{0,  0},{0, 0}, {0 ,0}}},  // null
    {2, {{0, -1},{0, 1}, {0 ,2}}},  // tetris
    {4, {{0, -1},{0, 1}, {1 ,1}}},  // L1
    {4, {{0, -1},{0, 1}, {-1,1}}},  // L2
    {2, {{0, -1},{1, 0}, {1 ,1}}},  // key1
    {2, {{0, -1},{-1,0}, {-1,1}}},  // key2
    {1, {{0,  1},{1, 0}, {1 ,1}}},  // square
    {4, {{0, -1},{1, 0}, {-1 ,0}}},  // T
};

/* 関数プロトタイプ宣言 */
int random(int);
BOOL putBlock(STATUS, BOOL);
BOOL boardDeleteCurrentBlock(STATUS);
void showBoard();
BOOL proccessInput();
void gameOver();
void deleteLine();
void blockDown();

