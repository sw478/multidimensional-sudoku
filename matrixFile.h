#ifndef MATRIX_FILE_H
#define MATRIX_FILE_H
#include "struct.h"

void setMatrixDimensions_Sudoku(Dance *d);
void findMatrixFile(Dance *d);
char *getMatrixFileName(Dance *d);
void initMatrixFile_Sudoku(Dance *d, char *matrixFileName);

#endif