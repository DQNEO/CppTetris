#include "GameMaster.h"

const int KIND_OF_BLOCK = 8;
const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 25;

/*Map*/
int board[MAP_WIDTH][MAP_HEIGHT];

/*cuurent block status*/
STATUS current;

bool bUpdateStop;

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

int score = 0, combo = 0, c_check = 0;

const BLOCK block[KIND_OF_BLOCK] = {
	{ 1, { { 0, 0 }, { 0, 0 }, { 0, 0 } } }, //null 
	{ 2, { { 0, -1 }, { 0, 1 }, { 0, 2 } } }, //tetris
	{ 4, { { 0, -1 }, { 0, 1 }, { 1, 1 } } }, //L1
	{ 4, { { 0, -1 }, { 0, 1 }, { -1, 1 } } }, //L2
	{ 2, { { 0, -1 }, { 1, 0 }, { 1, 1 } } }, //KEY1
	{ 2, { { 0, -1 }, { -1, 0 }, { -1, 1 } } }, //KEY2
	{ 1, { { 0, 1 }, { 1, 0 }, { 1, 1 } } }, //SQUARE
	{ 4, { { 0, -1 }, { 1, 0 }, { -1, 0 } } }  //T
};

int random(int max) {
	return (int)(rand() / (RAND_MAX + 1.0) * max);
}

void Setblocks(STATUS& s, int type)
{
	board[s.x][s.y] = type;

	// 회전 관련 
	for (int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for (int j = 0; j < r; j++) {
			int nx = dx;
			int	ny = dy;
			dx = ny;
			dy = -nx;
		}
		board[s.x + dx][s.y + dy] = type;
	}
}

bool putBlock(STATUS s, bool action) {

	if (CheckBlock(s) == false)
		return false;

	Setblocks(s, s.type);

	return true;
}

bool deleteBlock(STATUS s) {

	Setblocks(s, 0);

	return true;
}


bool CheckBlock(STATUS s)
{
	if (board[s.x][s.y] != 0) {
		return false;
	}

	// 회전 관련 
	for (int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for (int j = 0; j < r; j++) {
			int nx = dx;
			int	ny = dy;
			dx = ny;
			dy = -nx;
		}
		if (board[s.x + dx][s.y + dy] != 0) {
			return false;
		}

	}

	return true;
}

void AhphaBlending() {
	STATUS temp = current;

	if (temp.type == NULL)
		return;

	deleteBlock(temp);
	temp = makeDropResultBlock(temp);
	putBlock(current);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 120;
	bf.AlphaFormat = 0;

	AlphaBlend(hMemDC, (temp.x - 1) * 24, (20 - temp.y) * 24, 24, 24, hBlockDC, 0, (current.type) * 24, 24, 24, bf);

	for (int i = 0; i < 3; i++){
		int dx = block[current.type].p[i].x;
		int dy = block[current.type].p[i].y;

		int r = current.rotate % block[current.type].rotate;
		for (int j = 0; j < r; j++) {
			int nx = dx;
			int	ny = dy;
			dx = ny;
			dy = -nx;
		}
		AlphaBlend(hMemDC, (temp.x - 1 + dx) * 24, (20 - (temp.y + dy)) * 24, 24, 24, hBlockDC, 0, (current.type) * 24, 24, 24, bf);
	}
}


HDC showBoard() {


	//맵을 그린다
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			BitBlt(hMemDC, (x - 1) * 24, (20 - y) * 24, 24, 24, hBlockDC, 0, board[x][y] * 24, SRCCOPY);
		}
	}


	AhphaBlending();

	return hMemDC;
}

STATUS makeDropResultBlock(STATUS s){
	while (1){
		s.y--;
		if (CheckBlock(s))
			continue;
		else{			
			s.y++;
			break;
		}
	}

	return s;
}


bool processInput(WPARAM keyValue) {
	if (bUpdateStop == true)
		return false;

	bool ret = false;
	STATUS n = current;

	switch (keyValue){
	case VK_LEFT:
		n.x--;
		break;
	case VK_RIGHT:
		n.x++;
		break;
	case VK_UP:
		n.rotate++;
		break;
	case VK_DOWN:
		n.y--;
		break;
	case VK_SPACE:
		deleteBlock(n);
 		n = makeDropResultBlock(n);
		ret = true;
		break;
	}

	if (n.x != current.x || n.y != current.y || n.rotate != current.rotate) {
		deleteBlock(current);

		if (putBlock(n)) {
			current = n;
		}
		else {
			putBlock(current);
		}
	}

	return ret;
}


void gameOver() {
	bUpdateStop = true;

	current.type = NULL;
	Setblocks(current, 0);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			if (board[x][y] != 0) {
				board[x][y] = 1;
			}
		}
 	}

	bUpdateStop = true;
	
	return;	
}

void deleteLine() {
	for (int y = 1; y < 23; y++) {
		bool flag = true;
		for (int x = 1; x <= 10; x++) {
			if (board[x][y] == 0) {
				flag = false;
			}
		}

		if (flag) {
			c_check++;
			score += 10;

			for (int j = y; j < 23; j++) {
				for (int i = 1; i <= 10; i++) {
					board[i][j] = board[i][j + 1];
				}
			}
			y--;
		}
	}
	
	if (c_check>1)
	{
		combo = c_check;
		score += c_check * 100;//보너스 점수
	}
	c_check = 0;
	
}

void blockDown() {
	deleteBlock(current);
	current.y--;
	if (!putBlock(current)) {
		current.y++;
		putBlock(current);

		deleteLine();

		current.x = 5;
		current.y = 21;
		current.type = random(7) + 1;
		current.rotate = random(4);
		if (!putBlock(current)) {			
			gameOver();
		}
	}
}

void initialize()
{
	for (int x = 0; x < 12; x++) {
		for (int y = 0; y < 25; y++) {
			if (x == 0 || x == 11 || y == 0) {
				board[x][y] = 1;
			}
			else {
				board[x][y] = 0;
			}
		}
	}

	current.x = 5;
	current.y = 21;
	current.type = random(7) + 1;
	current.rotate = random(4);
	putBlock(current);

	bUpdateStop = false;
}

void MakeDCformBitmaps(HDC hdc, HINSTANCE hInstance){
	hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
	hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);

	hBlockDC = CreateCompatibleDC(hdc);
	hBitmap = LoadBitmap(hInstance, "BLOCKS");
	hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);

}

void Update()
{
	if (bUpdateStop == true)
		return;

	blockDown();
}

void Destory()
{
	HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hMemPrev);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);

	hBitmap = (HBITMAP)SelectObject(hBlockDC, hBlockPrev);
	DeleteObject(hBitmap);
	DeleteObject(hBlockDC);
}
