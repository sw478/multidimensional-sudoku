#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTrie.h"

void printBoard(int *grid, int x, int y);
void printMatrix(Dance *d);
void printSolutions(Dance *d);
void printSingleSol(Dance *d, SolTrie *sol);
void printSingleSol2(Dance *d, SolTrie *sol);
int saveSolution(Dance *d, Sudoku *s);

#endif
