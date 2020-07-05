#ifndef SETUP_H
#define SETUP_H
#include "struct.h"

int initDance(Dance *d, int x, int y);
int initMatrixFileSudoku(Dance *d, int x, int y);
int hideRows(Dance *d, Sudoku *s);
int hideRow(Dance *d, Doubly *row);
int recoverHiddenRows(Dance *d);

#endif
