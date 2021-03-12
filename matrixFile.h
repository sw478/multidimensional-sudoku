#ifndef MATRIX_FILE_H
#define MATRIX_FILE_H
#include "struct.h"

void setMatrixDimensions_Sudoku(Dance *d);
int compareInt(const void *a, const void *b);
void findMatrixFile(Dance *d);
void createMatrixFile(Dance *d, char *matrixFile, int *sorted_dim);
char *getMatrixFileName(Dance *d, int *sorted_dim);

#endif