#ifndef TESTDANCE_H
#define TESTDANCE_H
#include "dance.h"

void testX(Dance *d, Sudoku *s);
void testAddAllDoubly(Dance *d);
void testAddWiki(Dance *d);
void printMatrix(Dance *d);
void printSolutions(Dance *d);
void printNodeInfo(Doubly *node);
void printSingleSol(Dance *d, SolTrie *sol);
void printSingleSol2(Dance *d, SolTrie *sol);

#endif
