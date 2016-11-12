#ifndef _GAMEMASTER_H_
#define _GAMEMASTER_H_

const bool DEBUG_MODE = false;

#include <windows.h>
#include <stdio.h>

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


//added func
void initialize(HDC hdc,HINSTANCE hInstance);
void Update();
void Destory();
bool CheckBlock(STATUS s);
void AhphaBlending();;
void Setblocks(STATUS& s, int type);
STATUS makeDropResultBlock(STATUS s); // 임시 


//Original
int random(int max);
bool putBlock(STATUS s, bool action = false);
bool deleteBlock(STATUS s);
HDC showBoard();
bool processInput(WPARAM keyValue);
void gameOver();
void deleteLine();
void blockDown();


#endif /* GAMEMASTER_H */



