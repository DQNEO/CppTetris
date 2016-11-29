/******************************************************************************
* Project Name      : CppTetris
* File Name         : GameMaster.h
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

#ifndef _GAMEMASTER_H_
#define _GAMEMASTER_H_

#include "Basic.h"


//추가된 함수들 
void initialize(int(*board)[MAP_HEIGHT]); //보드 초기화  
void Update(int(*board)[MAP_HEIGHT]); //타이머에서 게임 업데이트 
void Destory(); // 자원 해제
int CheckBlock(STATUS s, int(*board)[MAP_HEIGHT]); //블럭 s가 보드에 들어갈수 있는가 
void AhphaBlending(int(*board)[MAP_HEIGHT]); // AhplaBlending
void Setblocks(STATUS s, int type, int(*board)[MAP_HEIGHT]); // Board에 s에 해당하는 블럭에  type 값을 넣음   
STATUS makeDropResultBlock(STATUS s, int(*board)[MAP_HEIGHT]); //즉시 내린 블록 만든다  
void MakeDCformBitmaps(HDC hdc, HINSTANCE hInstance); //load bitmaps 


//Original
int putBlock(STATUS s, int(*board)[MAP_HEIGHT]);
int deleteBlock(STATUS s, int(*board)[MAP_HEIGHT]);
HDC showBoard(int(*board)[MAP_HEIGHT]);
int processInput(WPARAM keyValue, int(*board)[MAP_HEIGHT]);
void gameOver(int(*board)[MAP_HEIGHT]);
void deleteLine(int(*board)[MAP_HEIGHT]);
void blockDown(int(*board)[MAP_HEIGHT]);



#endif /* GAMEMASTER_H */



