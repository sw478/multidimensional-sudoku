#ifndef TESTDANCE_H
#define TESTDANCE_H
#include "dance.h"

void testX(Dance *d, Sudoku *s);
void testAddAllDoubly(Dance *d);
void testAddWiki(Dance *d);
void printMatrix(Dance *d);
int cmp_int (const void *a, const void *b);
void printSolution(Dance *);
void printNodeInfo(Doubly *node);

#endif
