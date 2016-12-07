/******************************************************************************
* Project Name      : CppTetris
* File Name         : Basic.h
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

/*
Basic Header File

define value 
define sturct
define standard func 
*/
#ifndef _BASIC_H_
#define _BASIC_H_

#include <windows.h>
#include <stdio.h>
#include <float.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define DEBUG_MODE FALSE

#define MAP_WIDTH 12
#define MAP_HEIGHT 25
#define KIND_OF_BLOCK 8

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

/*Type of Block*/
const extern BLOCK block[KIND_OF_BLOCK];

RECT MakeRECT(int left, int top, int right, int bottom);
POINT MakePOINT(int x, int y);

int random(int max);

#endif /*_BASIC_H_*/