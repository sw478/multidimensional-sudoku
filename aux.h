#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTrie.h"

void printBoard(Dance *d, int *grid);
void printMatrix(Dance *d);
void printColHeaders(Dance *d);
void printSolutions(Dance *d);
void printSingleSol(Dance *d, SolTrie *sol);
void printSingleSol2(Dance *d, SolTrie *sol);
void printHeur(Dance *d);
int saveSolution(Dance *d);
void checkMatrix(Dance *d);
void checkDoubly(Doubly *doub);

#endif
