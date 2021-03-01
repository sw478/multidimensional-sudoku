#ifndef HIDE_H
#define HIDE_H
#include "struct.h"

int initHide_Sudoku(Dance *d);
void hideAllCells(Dance *d);
int hideSingleCell(Dance *d, int igrid);
void unhideAllCells(Dance *d);
int unhideSingleCell(Dance *d, int igrid);
void freeHide(Dance *d);
void saveGeneratedPuzzle(Dance *d);

int initHide_Sudoku2(Dance *d);

#endif
