#ifndef SUDOKU_H
#define SUDOKU_H
#include "struct.h"

void checkConfig();
int runSudoku(Dance *d, int argc, char *argv[]);
int runSudokuGen(Dance *d, int argc, char *argv[]);
int runSudokuSat(Dance *d, int argc, char *argv[]);

#endif
