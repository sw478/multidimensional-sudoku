#ifndef SOLTREE_H
#define SOLTREE_H
#define STARTING_CAP 1
#include "struct.h"

void addLeaf(Dance *d, SolTree *sol);
SolTree* initTree();
void addChild(SolTree *parent, SolTree *child);
void freeTree(SolTree *sol);
void freeSol(SolTree *sol);

#endif
