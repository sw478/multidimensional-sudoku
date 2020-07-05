#ifndef SOLTRIE_H
#define SOLTRIE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define STARTING_CAP 1
#include "struct.h"

SolTrie* initTrie(void *row);
void addChild(SolTrie *sol, SolTrie *child);
int deleteChild(SolTrie *sol, void *row);
int freeSol(SolTrie *sol);
void testSolTrie();

void addLeaf(Dance *d);
void updateTotalChildren(SolTrie *sol, int num);
SolTrie** getLeaves(SolTrie ***leaves, SolTrie *sol, long int *cap);
void getLeavesRecur(SolTrie ***leaves, SolTrie *sol, long int *cap, long int *index);

#endif
