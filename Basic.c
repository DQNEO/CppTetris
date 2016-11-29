/******************************************************************************
* Project Name      : CppTetris
* File Name         : Basic.c
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

#include "Basic.h"

RECT MakeRECT(int left, int top, int right, int bottom){
	RECT temp;
	temp.left = left;
	temp.top = top;
	temp.right = right;
	temp.bottom = bottom;
	return temp;
}

POINT MakePOINT(int x, int y){
	POINT temp;
	temp.x = x;
	temp.y = y;
	return temp;
}

int random(int max){
	return (int)(rand() / (RAND_MAX + 1.0) * max);
}

const BLOCK block[KIND_OF_BLOCK] = {
	{ 1, { { 0, 0 }, { 0, 0 }, { 0, 0 } } }, //null 
	{ 2, { { 0, -1 }, { 0, 1 }, { 0, 2 } } }, //tetris
	{ 4, { { 0, -1 }, { 0, 1 }, { 1, 1 } } }, //L1
	{ 4, { { 0, -1 }, { 0, 1 }, { -1, 1 } } }, //L2
	{ 2, { { 0, -1 }, { 1, 0 }, { 1, 1 } } }, //KEY1
	{ 2, { { 0, -1 }, { -1, 0 }, { -1, 1 } } }, //KEY2
	{ 1, { { 0, 1 }, { 1, 0 }, { 1, 1 } } }, //SQUARE
	{ 4, { { 0, -1 }, { 1, 0 }, { -1, 0 } } }  //T
};

