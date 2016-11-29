/******************************************************************************
* Project Name      : CppTetris
* File Name         : Tetris_AI.c
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

#include "Tetris_AI.h"

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

	deleteBlock(s, temp);
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
	int I = 0;
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
