#ifndef GENERATE_H
#define GENERATE_H
#include "struct.h"

int generate(Dance *d);
int generate2(Dance *d);
Hide *nextHideRand(Dance *d, int **hitList);
Hide *nextHideRand2(Dance *d, Hide **hitList, int *irand);
Hide **shuffledHide(Dance *d, int listSize);
void setNumClues(Sudoku *s, int maxNumClues);
void writePuzzleToSudokuFile(Dance *d);
void writeSolToSolFile(Dance *d);
void saveGeneratedPuzzle(Dance *d);

#endif
