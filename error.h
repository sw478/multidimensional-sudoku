#ifndef ERROR_H
#define ERROR_H
#include "struct.h"

void error_invalidSudokuBoard();
void error_file(char *fileName);
void error_arg1();
void error_numArg();
void error_heurNum();
void error_randomRowGen();
void error_doubly(int drow, int dcol);

#endif
