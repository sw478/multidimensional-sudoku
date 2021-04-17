#ifndef FREE_H
#define FREE_H
#include "struct.h"

void freeSudoku(Dance *d);
void freeSGen(Dance *d);
void freeSudokuSat(Dance *d);
void freeDance(Dance *d);
void freeMatrix(Dance *d);
void freeColumn(Doubly *col);

#endif
