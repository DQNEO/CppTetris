#pragma once

class GameMaster
{
public:
	GameMaster();
	~GameMaster();

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

	int random(int max);
	bool putBlock(STATUS s, bool action = false);
	bool deleteBlock(STATUS s);
	void showBoard();
	bool processInput();
	void gameOver();
	void deleteLine();
	void blockDown();
};

