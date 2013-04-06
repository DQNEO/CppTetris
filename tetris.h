/* Windows���܂��Ȃ��n�̃O���[�o���ϐ� */
HINSTANCE hInstance;
HWND hMainWindow;

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

/* �\���̂̐錾 */

/**
 * �ʒu��\���\����
 */
typedef struct _TAG_POSITION {
    int x;
    int y;
} POSITION;

/**
 * �u���b�N��\���\����
 */
typedef struct _TAG_BLOCK {
    int rotate;
    POSITION p[3];
} BLOCK;


/**
 * ���ݑ��쒆�̃u���b�N��\���\����
 */
typedef struct _TAG_STATUS {
    int x;
    int y;
    int type;
    int rotate;
} STATUS;

/* �O���[�o���ϐ��̐錾 */

/**
 * �Ֆ�(12x25)�f�[�^�\����
 * 0:��
 * 1-7:�u���b�N�̃^�C�v
 */
int board[12][25];

/** ���ݑ��쒆�̃u���b�N */
STATUS current;

/**
 * �u���b�N�̑S��ނ��`����
 * (������0�͍��w�i��\��)
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

/* �֐��v���g�^�C�v�錾 */
int random(int);
BOOL putBlock(STATUS, BOOL);
BOOL boardDeleteCurrentBlock(STATUS);
void showBoard();
BOOL proccessInput();
void gameOver();
void deleteLine();
void blockDown();

