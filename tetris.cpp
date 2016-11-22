#include <windows.h>

HINSTANCE hInstance;
HWND hMainWindow;

HDC hMemDC, hBlockDC, hdc;
HBITMAP hMemPrev, hBlockPrev;

char imsistr[128];// 문자열 출력을 위한 임시 버퍼
int board[12][25];
int score=0,combo=0,c_check=0,total=0,line=0; //c_check=콤보인지 확인, total=콤보누적횟수

typedef struct _TAG_POSITION {
	int x;
	int y;
} POSITION;

typedef struct _TAG_BLOCK {
	int rotate;
	POSITION p[3];
} BLOCK;

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

typedef struct _TAG_STATUS {
	int x;
	int y;
	int type;
	int rotate;
} STATUS;

STATUS current;

int random(int max) {
	return (int)(rand() / (RAND_MAX + 1.0) * max);
}

bool putBlock(STATUS s, bool action = false) {
	if(board[s.x][s.y] != 0) {
		return false;
	}


	if(action) {
		board[s.x][s.y] = s.type;
	}


	for(int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for(int j = 0; j < r; j++) {
			int nx = dx, ny = dy;
			dx = ny; dy = -nx;
		}
		if(board[s.x + dx][s.y + dy] != 0) {
			return false;
		}
		if(action) {
			board[s.x + dx][s.y + dy] = s.type;
		}
	}
	if(!action) {
		putBlock(s, true);
	}
	return true;
}

bool deleteBlock(STATUS s) {
	board[s.x][s.y] = 0;

	for(int i = 0; i < 3; i++) {
		int dx = block[s.type].p[i].x;
		int dy = block[s.type].p[i].y;
		int r = s.rotate % block[s.type].rotate;
		for(int j = 0; j < r; j++) {
			int nx = dx, ny = dy;
			dx = ny; dy = -nx;
		}
		board[s.x + dx][s.y + dy] = 0;
	}

	return true;
}

//점수
void printScore()
{
	Rectangle(hdc,251,200,335,260);
	wsprintf(imsistr,"SCORE");
	TextOut(hdc,270,210,imsistr,strlen(imsistr));
	wsprintf(imsistr,"%d",score);
	TextOut(hdc,275,235,imsistr,strlen(imsistr));
}

//콤보
void printCombo()
{
	Rectangle(hdc,251,270,335,330);
    wsprintf(imsistr,"COMBO");
	TextOut(hdc,270,280,imsistr,strlen(imsistr));
	wsprintf(imsistr,"%d",combo);
	TextOut(hdc,275,305,imsistr,strlen(imsistr));
 }

//라인
void printLine()
{
	Rectangle(hdc,251,340,335,400);
    wsprintf(imsistr,"LINE");
	TextOut(hdc,270,350,imsistr,strlen(imsistr));
	wsprintf(imsistr,"%d",line);
	TextOut(hdc,275,375,imsistr,strlen(imsistr));
 }
void showBoard() {

	hdc=GetDC(hMainWindow);
	printScore();
	printCombo();
	printLine();

	for(int x = 1; x <= 10; x++) {
		for(int y = 1; y <= 20; y++) {
			BitBlt(hMemDC, (x - 1) * 24, (20 -y) * 24, 24, 24, hBlockDC, 0, board[x][y] * 24, SRCCOPY);
		}
	}
}

bool processInput() {
	bool ret = false;
	STATUS n = current;
	if(GetAsyncKeyState(VK_LEFT)) {
		n.x--;
	} else if(GetAsyncKeyState(VK_RIGHT)) {
		n.x++;
	} else if(GetAsyncKeyState(VK_UP)) {
		n.rotate++;
	} else if(GetAsyncKeyState(VK_DOWN)) {
		n.y--;
		ret = true;
	}

	if(n.x != current.x || n.y != current.y || n.rotate != current.rotate) {
		deleteBlock(current);
		if(putBlock(n)) {
			current = n;
		} else {
			putBlock(current);
		}
	}

	return ret;
}


void gameOver() {
	KillTimer(hMainWindow, 100);
	for(int x = 1; x <= 10;x++) {
		for(int y = 1; y <= 20; y++) {
			if(board[x][y] != 0) {
				board[x][y] = 1;
			}
		}
	}
	InvalidateRect(hMainWindow, NULL, false);
}

void deleteLine() {
	for(int y = 1; y < 23; y++) {
		bool flag = true;
		for(int x = 1;x <= 10; x++) {
			if(board[x][y] == 0) {
				flag = false;
			}
		}

		if(flag) {

			line++;
			c_check++;
			score+=10;

			for(int j = y; j < 23; j++) {
				for(int i = 1; i <= 10; i++) {
					board[i][j] = board[i][j + 1];
				}
			}
			y--;
		}
	}

	if(c_check>1) // combo인지 확인
	{
		combo=c_check;	
		total+=c_check; // 콤보를 누적함
		score+=c_check*100; // 콤보시 보너스 점수
	}

	if(c_check==1) // 한 줄만 지워진 경우 combo=0
	{
		combo=0;
	}
	c_check=0;

	if(total>10) // 누적 콤보 보너스 점수
	{
		score+=200;
		total-=10;
	}
}

void blockDown() {
	deleteBlock(current);
	current.y--;
	if(!putBlock(current)) {
		current.y++;
		putBlock(current);

		deleteLine();

		current.x = 5;
		current.y = 21;
		current.type = random(7) + 1;
		current.rotate = random(4);
		if(!putBlock(current)) {
			gameOver();
		}
	}
}

void createMap()
{
	for(int x = 0; x < 12; x++) {
			for(int y = 0; y < 25; y++) {
				if(x == 0 || x == 11 || y == 0) {
					board[x][y] = 1;
				} else {
					board[x][y] = 0;
				}
			}
		}

		current.x = 5;
		current.y = 21;
		current.type = random(7) + 1;
		current.rotate = random(4);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	
	switch(msg) {
	case WM_CREATE: {
		createMap();
		putBlock(current);

		HDC hdc = GetDC(hWnd);

		hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 24 * 10, 24 * 20);
		hMemPrev = (HBITMAP)SelectObject(hMemDC, hBitmap);

		hBlockDC = CreateCompatibleDC(hdc);
		hBitmap = LoadBitmap(hInstance, "BLOCKS");
		hBlockPrev = (HBITMAP)SelectObject(hBlockDC, hBitmap);


		
			// debug
		BitBlt(hMemDC, 0, 0, 24, 24, hBlockDC, 0, 0, SRCCOPY);

		ReleaseDC(hWnd, hdc);
		break;
					}
	case WM_TIMER: {
		static int w = 0;
		if(w % 2 == 0) {
			if(processInput()) {
				w = 0;
			}
		}
		if(w % 5 == 0) {
			blockDown();
		}
		w++;

		InvalidateRect(hWnd, NULL, false);
		break;
				   }

	case WM_PAINT: {
		showBoard();
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, 24 * 10, 24 * 20, hMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
				   }
	case WM_COMMAND:

		{
			if(((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED))
			{
				int iMID;
				iMID = LOWORD(wParam);
				switch(iMID)
				{
				case 10000:
					{
						KillTimer(hMainWindow, 100);
						if(MessageBox(hWnd, "게임을 계속 하시겠습니까?", "일시 정지", MB_OK))
							SetTimer(hMainWindow, 100, 1000/30, NULL);
						else
							gameOver();
						break;
					}
				case 10001:
					{

						KillTimer(hMainWindow, 100);
						if(MessageBox(hWnd, "게임을 재시작 하시겠습니까?", "재시작", MB_YESNO)==IDYES)
						{
							score=0;
							combo=0;
							c_check=0;
							total=0;
							line=0;
							createMap();
							SetTimer(hMainWindow, 100, 1000/30, NULL);
		
							break;
						}
						else
							SetTimer(hMainWindow, 100, 1000/30, NULL);
						break;
					}

					//AI ON
				case 1 :
					{
						
					}

					//AI OFF
				case 0 :
					{
					
					}
				default:
					break;
				}
			}
			break;
		}

	case WM_DESTROY: {
		HBITMAP hBitmap = (HBITMAP)SelectObject(hMemDC, hMemPrev);
		DeleteObject(hBitmap);
		DeleteObject(hMemDC);

		hBitmap = (HBITMAP)SelectObject(hBlockDC, hBlockPrev);
		DeleteObject(hBitmap);
		DeleteObject(hBlockDC);

		PostQuitMessage(0);
		break;
		return 0;
					 }
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
	hInstance = hInst;
	WNDCLASSEX wc;
	static LPCTSTR pClassName = "NicoNicoProgramming2"; 

	wc.cbSize        = sizeof(WNDCLASSEX);               
	wc.style         = CS_HREDRAW | CS_VREDRAW;         
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.cbClsExtra    = 0;                                
	wc.cbWndExtra    = 0;            
	wc.hInstance     = hInst;                         
	wc.hIcon         = NULL;                         
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW);    
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+0);
	wc.lpszMenuName  = NULL;                
	wc.lpszClassName = pClassName;                    
	wc.hIconSm       = NULL;                            

	if (!RegisterClassEx(&wc)) return FALSE;            

	RECT r;
	r.left = r.top = 0;
	r.right = 24 * 10;
	r.bottom = 24 * 20;
	AdjustWindowRectEx(&r, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, false, 0);

	hMainWindow = CreateWindow(pClassName, "Nico Nico Programming2", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, 350, r.bottom - r.top,
		NULL, NULL, hInst, NULL); //window창 사이즈의 크기를 바꿔 일시정지 버튼과 점수판을 작성하도록 한다.
	HWND hMainWindow_button;
	HWND hMainWindow_button1;
	HWND hMainWindow_check_box;
	HWND hMainWindow_r1;
	HWND hMainWindow_r2;
	
	hMainWindow_button = CreateWindow(TEXT("button"), TEXT("▶|"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		24 * 10 + 30, 10, 50, 50, hMainWindow, (HMENU)10000, hInstance, NULL);
	hMainWindow_button1 = CreateWindow(TEXT("button"), TEXT("Restart"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		24 * 10 + 20, 70, 70, 50, hMainWindow, (HMENU)10001, hInstance, NULL);
	
	//AI check_box
	hMainWindow_check_box = CreateWindow(TEXT("button"), TEXT("AI"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		24 * 10 + 15, 130, 80, 60, hMainWindow, (HMENU)0, hInstance, NULL);
	hMainWindow_r1 = CreateWindow(TEXT("button"), TEXT("ON"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		24 * 10 + 25, 145, 50, 20, hMainWindow, (HMENU)1, hInstance, NULL);
	hMainWindow_r2 = CreateWindow(TEXT("button"), TEXT("OFF"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		24 * 10 + 25, 165, 50, 20, hMainWindow, (HMENU)0, hInstance, NULL);

	ShowWindow(hMainWindow, SW_SHOW);
	SetTimer(hMainWindow, 100, 1000 /30, NULL);
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hMainWindow, 100);

	return 0;
}
