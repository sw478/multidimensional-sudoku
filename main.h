#ifndef SUDOKU_H
#define SUDOKU_H
#include "struct.h"

void checkConfig();
void runSudoku(Dance *d, int argc, char *argv[]);
void runSudokuGen(Dance *d, int argc, char *argv[]);
void runSudokuSat(Dance *d, int argc, char *argv[]);
void runSudokuEnumerate(Dance *d, int argc, char *argv[]);

#endif
