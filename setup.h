#ifndef SETUP_H
#define SETUP_H
#include "dance.h"

int initDance(Sudoku *s);
int initRoot(Dance *d);
int initHeaders(Dance *d);
int initDoubly(Dance *d, int drow, int dcol);
void freeDance(Dance *d);
void freeColumn(Doubly *col);
void coverDoubly(Doubly *node);

#endif
