#ifndef SETUP_H
#define SETUP_H
#include "struct.h"

int initDance(Dance *d);
int initMatrixFileSudoku(Dance *d);
int hideRows(Dance *d);
int hideRow(Dance *d, Doubly *row);
int recoverHiddenRows(Dance *d);
void coverRowHeaders(Dance *d);
void uncoverRowHeaders(Dance *d);

#endif
