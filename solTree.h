#ifndef SOLTREE_H
#define SOLTREE_H
#include "struct.h"

void addLeaf(Dance *d, SolTree *sol);
SolTree* initTree();
void addChild(SolTree *parent, SolTree *child);
void freeTree(SolTree *sol);
int saveSolution_Sudoku(Dance *d);

#endif
