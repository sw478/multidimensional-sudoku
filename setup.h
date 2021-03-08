#ifndef SETUP_H
#define SETUP_H
#include "struct.h"

int initDance(Dance *d);
int initMatrixFileSudoku(Dance *d);
void stitch_secondary(Dance *d);
void unstitch_secondary(Dance *d);
void set_secondary_columns(Dance *d, int index);

#endif
