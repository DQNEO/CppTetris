﻿#ifndef _GAMEMASTER_H_
#define _GAMEMASTER_H_


#include <windows.h>
#include <stdio.h>
#include <float.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define MAP_WIDTH 12
#define MAP_HEIGHT 25
#define KIND_OF_BLOCK 8

#define DEBUG_MODE FALSE

typedef struct _TAG_POSITION {
	int x;
	int y;
} POSITION;

typedef struct _TAG_BLOCK {
	int rotate;
	POSITION p[3];
} BLOCK;

typedef struct _TAG_STATUS {
	int x;
	int y;
	int type;
	int rotate;
} STATUS;

typedef struct _INFO {
	int score;
	int combo;
	int c_check;
	int total;
	int line;
	int bUpdateStop;
	long Framecounter;
}INFO;





//added func
void initialize(int(*board)[MAP_HEIGHT]);
void Update(int(*board)[MAP_HEIGHT]);
void Destory();
int CheckBlock(STATUS s, int(*board)[MAP_HEIGHT]);
void AhphaBlending(int(*board)[MAP_HEIGHT]);;
void Setblocks(STATUS s, int type, int(*board)[MAP_HEIGHT]);
STATUS makeDropResultBlock(STATUS s, int(*board)[MAP_HEIGHT]); // 임시 
void MakeDCformBitmaps(HDC hdc, HINSTANCE hInstance);


//Original
int random(int max);
int putBlock(STATUS s, int(*board)[MAP_HEIGHT]);
int deleteBlock(STATUS s, int(*board)[MAP_HEIGHT]);
HDC showBoard(int(*board)[MAP_HEIGHT]);
int processInput(WPARAM keyValue, int(*board)[MAP_HEIGHT]);
void gameOver(int(*board)[MAP_HEIGHT]);
void deleteLine(int(*board)[MAP_HEIGHT]);
void blockDown(int(*board)[MAP_HEIGHT]);





//for AI
/*************************************************
* A : 블럭 높이의 합
* B : 블럭 사이에 있는 빈 공간의 개수
* C : 빈 공간을 막고 있는 블럭의 개수
* D : 완성된 줄의 개수
* E : 현재 블럭이 기존 블럭들과 닿는 면의 개수
* F : 현재 블럭이 좌우 벽면과 닿는 면의 개수
* G : 현재 블럭이 바닥면과 닿는 면의 개수
* H : 쌓인 블럭의 최고 높이 - 최저높이
* I : 쌓는 블럭의 최고높이
*************************************************/
#define AI_A -3.78
#define AI_B -9.8
#define AI_C -1.59
#define AI_D 20.2
#define AI_E 3.7
#define AI_F 2.5
#define AI_G 4.0
#define AI_H -5
#define AI_I -2


STATUS getBlock(STATUS s, int(*board)[MAP_HEIGHT]); //get Block of Highest AI Score 
double calcAIScore(STATUS s, int(*board)[MAP_HEIGHT]); 
void copyBoard(int(*temp)[MAP_HEIGHT], int(*board)[MAP_HEIGHT]);

double getABCH(int(*temp)[MAP_HEIGHT]); //Calculate A, B, C, H score
double getD(int(*temp)[MAP_HEIGHT]);	//Calculate D score
double getEFGI(STATUS s, int(*temp)[MAP_HEIGHT]); //Calculate E, F, G, I score




#endif /* GAMEMASTER_H */



