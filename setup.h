#ifndef SETUP_H
#define SETUP_H
#include "struct.h"

int initDance(Dance *d);
int initMatrixFileSudoku(Dance *d);
void stitch_secondary(Dance *d, int dcol_sec);
void unstitch_secondary(Dance *d);

#endif
