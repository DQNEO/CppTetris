@echo off

:: ���\�[�X�t�@�C�����R���p�C��
windres -i  block.rc -o block.o

:: �e�g���X�{�̂��R���p�C��
gcc -mwindows tetris.cpp  block.o

