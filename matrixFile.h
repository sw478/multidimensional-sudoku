#ifndef MATRIX_FILE_H
#define MATRIX_FILE_H
#include "struct.h"

void setMatrixDimensions(Dance *d, Sudoku *s);
void findMatrixFile(Dance *d);
char *getMatrixFileName(Dance *d);
void initMatrixFile(Dance *d, char *matrixFileName);

#endif