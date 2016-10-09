#pragma once
#include <windows.h>

class GameMaster
{
public:
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

private:
	BLOCK block[8];
	int board[12][25];
	STATUS current;

public:
	GameMaster();
	~GameMaster();

	int random(int max);
	bool putBlock(STATUS s, bool action = false);
	bool deleteBlock(STATUS s);
	void showBoard();
	bool processInput();
	void gameOver();
	void deleteLine();
	void blockDown();

};

