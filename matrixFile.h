#ifndef MATRIX_FILE_H
#define MATRIX_FILE_H
#include "struct.h"

void setMatrixDimensions_Sudoku(Dance *d);
void findMatrixFile(Dance *d);
void createMatrixFile(Dance *d, char *matrixFile);
char *getMatrixFileName(Dance *d);

#endif