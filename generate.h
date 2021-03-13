#ifndef GENERATE_H
#define GENERATE_H
#include "struct.h"

int generate(Dance *d);
int generate2(Dance *d);
Hide *nextHide(Dance *d, int **hitList);
void setMaxNumClues(Sudoku *s, int maxNumClues);
void printToSudokuFile(Dance *d);
void saveGeneratedPuzzle(Dance *d);

#endif
