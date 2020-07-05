#ifndef SETUP_H
#define SETUP_H
#include "dance.h"

int initDanceSudoku(Sudoku *s);
int initDance(Dance *d, int x, int y);
int initDoubly(Dance *d, int drow, int dcol);
int initDoubly2(Dance *d, int drow, int dcol);
Doubly *getHRow(Dance *d, int irow);
Doubly *getHCol(Dance *d, int icol);

Doubly *initDoubly3(int drow, int dcol);
int connectRows(Dance *d);
int connectCols(Dance *d);
int compareRows(const void *a, const void *b);
int compareCols(const void *a, const void *b);

void freeDance(Dance *d);
void freeColumn(Doubly *col);
void freeDoubly(Doubly *node);

#endif
