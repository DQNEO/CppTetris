/******************************************************************************
* Project Name      : CppTetris
* File Name         : Tetris_AI.h
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

#ifndef _TETRIS_AI_H_
#define _TETRIS_AI_H_

#include "Basic.h"
#include "GameMaster.h"


//for AI
/*************************************************
* A : �� ������ ��
* B : �� ���̿� �ִ� �� ������ ����
* C : �� ������ ���� �ִ� ���� ����
* D : �ϼ��� ���� ����
* E : ���� ���� ���� ����� ��� ���� ����
* F : ���� ���� �¿� ����� ��� ���� ����
* G : ���� ���� �ٴڸ�� ��� ���� ����
* H : ���� ���� �ְ� ���� - ��������
* I : �״� ���� �ְ����
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


#endif /*_TETRIS_AI_H_*/
