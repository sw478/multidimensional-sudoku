#ifndef FILEHANDLING_H
#define FILEHANDLING_H
#include "struct.h"

void readInSudokuFile(Sudoku *s, FILE *f);

void writeToSudokuFile(Sudoku *s, FILE *f);
void writeToEnumerateFile(Dance *d, FILE *f);

#endif