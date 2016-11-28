#include "GameMaster.h"

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

INFO info; //user information
STATUS current; //cuurent block status

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

void Setblocks(STATUS s, int type, int(*board)[MAP_HEIGHT])
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

int putBlock(STATUS s, int(*board)[MAP_HEIGHT]) {

	if (CheckBlock(s, board) == FALSE)
		return FALSE;

	Setblocks(s, s.type, board);

	return TRUE;
}

int deleteBlock(STATUS s, int(*board)[MAP_HEIGHT]) {

	Setblocks(s, 0, board);

	return TRUE;
}


int CheckBlock(STATUS s, int(*board)[MAP_HEIGHT])
{
	if (board[s.x][s.y] != 0) {
		return FALSE;
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
			return FALSE;
		}

	}

	return TRUE;
}

void AhphaBlending(int(*board)[MAP_HEIGHT]) {
	STATUS temp = current;

	if (temp.type == NULL)
		return;

	deleteBlock(temp, board);
	temp = makeDropResultBlock(temp, board);
	putBlock(current, board);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 120;
	bf.AlphaFormat = 0;

	AlphaBlend(hMemDC, (temp.x - 1) * 24, (20 - temp.y) * 24, 24, 24, hBlockDC, 0, (current.type) * 24, 24, 24, bf);

	for (int i = 0; i < 3; i++) {
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


HDC showBoard(int(*board)[MAP_HEIGHT]) {
	//맵을 그린다
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			BitBlt(hMemDC, (x - 1) * 24, (20 - y) * 24, 24, 24, hBlockDC, 0, board[x][y] * 24, SRCCOPY);
		}
	}

	AhphaBlending(board);

	return hMemDC;
}

STATUS makeDropResultBlock(STATUS s, int(*board)[MAP_HEIGHT]) {
	while (1) {
		s.y--;
		if (CheckBlock(s, board))
			continue;
		else {
			s.y++;
			break;
		}
	}

	return s;
}


int processInput(WPARAM keyValue, int(*board)[MAP_HEIGHT]) {
	if (info.bUpdateStop == TRUE)
		return FALSE;

	int ret = FALSE;
	STATUS n = current;

	switch (keyValue) {
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
		deleteBlock(n, board);
		n = makeDropResultBlock(n, board);
		ret = TRUE;
		break;
	}

	if (n.x != current.x || n.y != current.y || n.rotate != current.rotate) {
		deleteBlock(current, board);

		if (putBlock(n, board)) {
			current = n;
		}

		else {
			putBlock(current, board);
		}
	}

	return ret;
}


void gameOver(int(*board)[MAP_HEIGHT]) {
	info.bUpdateStop = TRUE;

	current.type = NULL;
	//Setblocks(current, 0);

	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			if (board[x][y] != 0) {
				board[x][y] = 1;
			}
		}
	}

	return;
}

void deleteLine(int(*board)[MAP_HEIGHT]) {
	for (int y = 1; y < 23; y++) {
		int flag = TRUE;
		for (int x = 1; x <= 10; x++) {
			if (board[x][y] == 0) {
				flag = FALSE;
			}
		}

		if (flag) {
			info.c_check++;
			info.score += 10;
			info.line++;

			for (int j = y; j < 23; j++) {
				for (int i = 1; i <= 10; i++) {
					board[i][j] = board[i][j + 1];
				}
			}
			y--;
		}
	}

	if (info.c_check > 1) // combo인지 확인
	{
		info.combo = info.c_check;
		info.total += info.c_check; // 콤보를 누적함
		info.score += info.c_check * 100; // 콤보시 보너스 점수
	}

	if (info.c_check == 1) // 한 줄만 지워진 경우 combo=0
	{
		info.combo = 0;
	}
	info.c_check = 0;

	if (info.total > 10) // 누적 콤보 보너스 점수
	{
		info.score += 200;
		info.total -= 10;
	}

}

void blockDown(int(*board)[MAP_HEIGHT]) {
	deleteBlock(current, board);
	current.y--;
	if (!putBlock(current, board)) {
		current.y++;
		putBlock(current, board);

		deleteLine(board);

		current.x = 5;
		current.y = 21;
		current.type = random(7) + 1;
		current.rotate = random(4);
		if (!putBlock(current, board)) {
			gameOver(board);
		}
	}
}

void initialize(int(*board)[MAP_HEIGHT])
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
	putBlock(current, board);

	info.bUpdateStop = FALSE;

	info.combo = info.c_check = info.line = info.score = info.total = 0;
	info.Framecounter = 0;
}

void MakeDCformBitmaps(HDC hdc, HINSTANCE hInstance) {
	hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
	hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);

	hBlockDC = CreateCompatibleDC(hdc);
	hBitmap = LoadBitmap(hInstance, "BLOCKS");
	hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);

}

void Update(int(*board)[MAP_HEIGHT])
{
	if (info.bUpdateStop == TRUE)
		return;

	blockDown(board);
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








//for AI

 /**
 * Get best position block for AI.
 *
 * @param : current block
 * @param : current board
 *
 * @return : best position BLOCK
 */
STATUS getBlock(STATUS s, int(*board)[MAP_HEIGHT]) {
	STATUS highest = s;
	double highest_score = -9999999999999.0;

	for (int i = 1; i <= 10; i++) {
		s.x = i;
		for (int j = 0; j < block[s.type].rotate; j++) {
			++s.rotate;
			double temp = calcAIScore(s, board);
			if (temp > highest_score) {
				highest_score = temp;
				highest = s;
			}
		}
	}

	return makeDropResultBlock(highest, board);
}




/**
* Calculate AI score.
*
* @param block
* @param current board
*
* @return AI score
*/
double calcAIScore(STATUS s, int(*board)[MAP_HEIGHT]) {
	int temp[MAP_WIDTH][MAP_HEIGHT];
	copyBoard(temp, board);

	deleteBlock(current, temp);
	s = makeDropResultBlock(s, temp);

	if (!putBlock(s, temp))
		return -9999999999999.0;



	double EFG = getEFGI(s, temp);
	double D = getD(temp);
	double ABC = getABCH(temp);


	return ABC + D + EFG;

}



/**
* Copy current board to temp board.
*
* @param temp board
* @param current board
*
* @return void
*/
void copyBoard(int(*temp)[MAP_HEIGHT], int(*board)[MAP_HEIGHT]) {
	for (int i = 0; i < MAP_WIDTH; i++)
		for (int j = 0; j < MAP_HEIGHT; j++)
			temp[i][j] = board[i][j];
}



/**
* Calculate AI score of condition A, B, C, H 
*
* @param temp board
*
* @return AI score of condition A, B, C, H
*/
double getABCH(int(*temp)[MAP_HEIGHT]) {
	int A = 0;
	int B = 0;
	int C = 0;
	int H = INT_MAX;

	for (int x = 1; x < MAP_WIDTH - 1; x++) {
		int height = 0;
		int firstEmpty = TRUE;
		for (int y = 1; y < MAP_HEIGHT; y++) {
			if (temp[x][y] != 0)
				height = y;
		}
		for (int y = 1; y <= height; y++) {
			if (temp[x][y] == 0) {
				B++;
				if (firstEmpty) {
					firstEmpty = FALSE;
					for (int i = y; i <= height; i++) {
						if (temp[x][i] != 0)
							C++;
					}
				}

			}
		}
		if (height > A)
			A = height;
		if (height < H)
			H = height;
	}
	H = A - H;
	return A * AI_A + B * AI_B + C * AI_C + H * AI_H;
}


/**
* Calculate AI score of condition D
*
* @param temp board
*
* @return AI score of condition D
*/
double getD(int(*temp)[MAP_HEIGHT]) {
	int cnt = 0;
	for (int y = 1; y < 23; y++) {
		int flag = TRUE;
		for (int x = 1; x <= 10; x++) {
			if (temp[x][y] == 0) {
				flag = FALSE;
			}
		}
		if (flag) {
			cnt++;
			for (int j = y; j < 23; j++) {
				for (int i = 1; i <= 10; i++) {
					temp[i][j] = temp[i][j + 1];
				}
			}
			y--;
		}
	}
	return cnt * AI_D;
}


/**
* Calculate AI score of condition E, F, G, I
*
* @param put block
* @param temp board
*
* @return AI score of condition E, F, G, I
*/
double getEFGI(STATUS s, int(*temp)[MAP_HEIGHT]) {
	int I=0;
	deleteBlock(s, temp);

	int cntE = 0;
	int cntF = 0;
	int cntG = 0;

	if (s.x == 1 || s.x == 11)
		cntF++;
	if (s.y == 1)
		cntG++;

	if (temp[s.x + 1][s.y] != 0 && s.x + 1 != 1 && s.x + 1 != 11)
		cntE++;
	if (temp[s.x - 1][s.y] != 0 && s.x - 1 != 1 && s.x + 1 != 11)
		cntE++;
	if (temp[s.x][s.y + 1] != 0 && s.y + 1 != 1)
		cntE++;
	if (temp[s.x][s.y - 1] != 0 && s.y + 1 != 1)
		cntE++;


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
		if (s.y + dy > I)
			I = s.y + dy;

		if (s.x + dx == 1 || s.x + dx == 11)
			cntF++;
		if (s.y + dy == 1)
			cntG++;

		if (temp[s.x + 1 + dx][s.y + dy] != 0 && s.x + dx + 1 != 1 && s.x + dx + 1 != 11)
			cntE++;
		if (temp[s.x - 1 + dx][s.y + dy] != 0 && s.x + dx - 1 != 1 && s.x + dx + 1 != 11)
			cntE++;
		if (temp[s.x + dx][s.y + 1 + dy] != 0 && s.y + dy + 1 != 1)
			cntE++;
		if (temp[s.x + dx][s.y - 1 + dy] != 0 && s.y + 1 + dy != 1)
			cntE++;

	}
	putBlock(s, temp);

	 

	return cntE * AI_E + cntF * AI_F + cntG * AI_G + I * AI_I;

}
