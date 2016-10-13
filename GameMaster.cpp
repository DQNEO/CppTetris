#include "GameMaster.h"


GameMaster::GameMaster()
{
	block[0] = { 1, { { 0,  0 },	{ 0, 0 }	, { 0, 0 } } }; //null
	block[1] = { 2, { { 0, -1 },	{ 0, 1 }	, { 0, 2 } } }; // tetris
	block[2] = { 4, { { 0, -1 },	{ 0, 1 }	, { 1, 1 } } }; //L1
	block[3] = { 4, { { 0, -1 },	{ 0, 1 }	, { -1, 1 } }}; // L2
	block[4] = { 2, { { 0, -1 },	{ 1, 0 }	, { 1, 1 } } }; // key1
	block[5] = { 2, { { 0, -1 },	{ -1,0 }	, { -1, 1 } }}; // key2
	block[6] = { 1, { { 0,  1 },	{ 1, 0 }	, { 1, 1 } } }; // square
	block[7] = { 4, { { 0, -1 },	{ 1, 0 }	, { -1, 0 } }}; // T	

}


GameMaster::~GameMaster()
{
}

int GameMaster::random(int max) {
	return (int)(rand() / (RAND_MAX + 1.0) * max);
}

bool GameMaster::putBlock(STATUS s, bool action) {
	if (board[s.x][s.y] != 0) {
		return false;
	}

	if (action) {
		board[s.x][s.y] = s.type;
	}

	// 회전 관련 
	for (int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for (int j = 0; j < r; j++) {
			int nx = dx, ny = dy;
			dx = ny; dy = -nx;
		}
		if (board[s.x + dx][s.y + dy] != 0) {
			return false;
		}
		if (action) {
			board[s.x + dx][s.y + dy] = s.type;
		}
	}
	if (!action) {
		putBlock(s, true);
	}
	return true;
}

bool GameMaster::deleteBlock(STATUS s) {
	board[s.x][s.y] = 0;

	for (int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for (int j = 0; j < r; j++) {
			int nx = dx, ny = dy;
			dx = ny; dy = -nx;
		}
		board[s.x + dx][s.y + dy] = 0;
	}

	return true;
}



HDC GameMaster::showBoard() {
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			BitBlt(hMemDC, (x - 1) * 24, (20 - y) * 24, 24, 24, hBlockDC, 0, board[x][y] * 24, SRCCOPY);
		}
	}

	return hMemDC;
}


bool GameMaster::processInput() {
	bool ret = false;
	STATUS n = current;
	if (GetAsyncKeyState(VK_LEFT)) {
		n.x--;
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		n.x++;
	}
	else if (GetAsyncKeyState(VK_UP)) {
		n.rotate++;
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		n.y--;
		ret = true;
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


void GameMaster::gameOver() {
	bUpdateStop = true;

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			if (board[x][y] != 0) {
				board[x][y] = 1;
			}
		}
	}
	//InvalidateRect(hMainWindow, NULL, false);
}

void GameMaster::deleteLine() {
	for (int y = 1; y < 23; y++) {
		bool flag = true;
		for (int x = 1; x <= 10; x++) {
			if (board[x][y] == 0) {
				flag = false;
			}
		}

		if (flag) {
			for (int j = y; j < 23; j++) {
				for (int i = 1; i <= 10; i++) {
					board[i][j] = board[i][j + 1];
				}
			}
			y--;
		}
	}
}

void GameMaster::blockDown() {
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

void GameMaster::initialize(HDC hdc, HINSTANCE hInstance)
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

	hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
	hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);

	hBlockDC = CreateCompatibleDC(hdc);
	hBitmap = LoadBitmap(hInstance, "BLOCKS");
	hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);

	bUpdateStop = false;
}

void GameMaster::Update()
{
	if (bUpdateStop == false)
	{
		static int w = 0;
		if (w % 3 == 0) {
			processInput();
		}

		if (w % 8 == 0) {
			blockDown();
		}
		w++;
	}

}

void GameMaster::Destory()
{
	HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hMemPrev);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);

	hBitmap = (HBITMAP)SelectObject(hBlockDC, hBlockPrev);
	DeleteObject(hBitmap);
	DeleteObject(hBlockDC);

}