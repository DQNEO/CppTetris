/******************************************************************************
* Project Name      : CppTetris
* File Name         : GameMaster.c
* Version           : 1.0
* Compiler          : Visual Studio 2015
*
********************************************************************************
* Copyright (2016), tkihira.
********************************************************************************
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
* A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
* IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#include "GameMaster.h"

HDC hMemDC, hBlockDC;
HBITMAP hMemPrev, hBlockPrev;

INFO info; //user information
STATUS current; //cuurent block status

/**
* Set values at board as s form
*
* @param current block 
* @param value 
* @param board
*
*/
void Setblocks(STATUS s, int type, int(*board)[MAP_HEIGHT])
{
	board[s.x][s.y] = type;

	//loop other 3 piece
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

/**
* Put blocks to board
*
* @param block
* @param board
*
* @return success or failure of putblock 
*/
int putBlock(STATUS s, int(*board)[MAP_HEIGHT]) {

	if (CheckBlock(s, board) == FALSE)
		return FALSE;

	Setblocks(s, s.type, board);

	return TRUE;
}

/**
* delete blocks at board
*
* @param block
* @param board
*
*/
int deleteBlock(STATUS s, int(*board)[MAP_HEIGHT]) {

	Setblocks(s, 0, board);

	return TRUE;
}

/**
* check the block can put at board
*
* @param new block
* @param board
*
* @return success or failure of checkblock
*/
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

/**
* preview dropdown block
* 1. make dropResultblock
* 2. alphablend dropResultblock;
*
* @param board
*
* @return success or failure of checkblock
*/
void AhphaBlending(int(*board)[MAP_HEIGHT]) {
	STATUS temp = current;

	if (temp.type == 0)
		return;

	deleteBlock(temp, board);
	//즉시 내린 블록 생성 
	temp = makeDropResultBlock(temp, board);
	putBlock(current, board);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 120;
	bf.AlphaFormat = 0;

	//코어 블록 만 그리고 
	AlphaBlend(hMemDC, (temp.x - 1) * 24, (20 - temp.y) * 24, 24, 24, hBlockDC, 0, (current.type) * 24, 24, 24, bf);

	//나머지 3개 블록 그린다.
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

/**
* show Map
*
* @param board
*/
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

/**
* makeDropResult 
*
* @param block
* @param board
*
* @return DropResult block
*/
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


/**
* when current block is changed as input /current block and put it 
*
* @param keyValue
* @param board
*
* @return isSpacebar down
*/
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

	//when current block is changed
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


/**
* Gameover after work
*
* @param board
*
* @return isSpacebar down
*/
void gameOver(int(*board)[MAP_HEIGHT]) {
	info.bUpdateStop = TRUE;

	current.type = 0;
	//Setblocks(current, 0);

	//block set red
	for (int x = 1; x <= 10; x++) {
		for (int y = 1; y <= 20; y++) {
			if (board[x][y] != 0) {
				board[x][y] = 1;
			}
		}
	}

	return;
}

/**
* check Horizen line is full block
* claer line
* Update Socre
*
* @param board
*
*/
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


/**
* current block down;
* when current block conflict at ground, fix that and make new block
* if makeNewblock is failure  the Game is Over  
*
* @param board
*
*/
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


/**
* initialize board and make first block
*
* @param board
*
*/
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

	//Make new 
	current.x = 5;
	current.y = 21;
	current.type = random(7) + 1;
	current.rotate = random(4);
	putBlock(current, board);

	info.bUpdateStop = FALSE;

	info.combo = info.c_check = info.line = info.score = info.total = 0;
	info.Framecounter = 0;
}

/**
* Load Bitmap Resources
*
* @param hdc
* @param hinstance
*/
void MakeDCformBitmaps(HDC hdc, HINSTANCE hInstance) {
	hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
	hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);

	hBlockDC = CreateCompatibleDC(hdc);
	hBitmap = LoadBitmap(hInstance, "BLOCKS");
	hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);

	
}

/**
* if Game is not over 
* block down
*
*/
void Update(int(*board)[MAP_HEIGHT])
{
	if (info.bUpdateStop == TRUE)
		return;

	blockDown(board);
}   


/**
*  Release resources
*/
void Destory()
{	
	HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hMemPrev);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);

	hBitmap = (HBITMAP)SelectObject(hBlockDC, hBlockPrev);
	DeleteObject(hBitmap);
	DeleteObject(hBlockDC);
}