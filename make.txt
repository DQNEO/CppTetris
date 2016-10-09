@echo off

:: リソースファイルをコンパイル
windres -i  block.rc -o block.o

:: テトリス本体をコンパイル
gcc -mwindows tetris.cpp  block.o

