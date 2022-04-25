#ifndef HIDE_H
#define HIDE_H
#include "struct.h"

int initHide(Dance *d);
void fillAllCells(Dance *d);
int fillSingleCell(Dance *d, Hide *h);
void unfillAllCells(Dance *d);
int unfillSingleCell(Dance *d, Hide *h);
void freeHide(Dance *d);

int hide_Sudoku2(Dance *d);
int unhide_Sudoku2(Dance *d);
int hideSingleRow(Dance *d, Doubly *hrow);
int unhideSingleRow(Dance *d, Doubly *hrow);

#endif
