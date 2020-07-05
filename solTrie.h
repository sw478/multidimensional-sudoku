#ifndef SOLTRIE_H
#define SOLTRIE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define STARTING_CAP 1

typedef struct solTrie
{
   void *row;
   struct solTrie **child, *parent;
   int ichild, cap, numSols;
} SolTrie;

SolTrie* initTrie(void *row);
void addChild(SolTrie *sol, SolTrie *child);
int deleteChild(SolTrie *sol, void *row);
int freeSol(SolTrie *sol);
void testSolTrie();

void updateTotalChildren(SolTrie *sol, int num);
SolTrie** getLeaves(SolTrie ***leaves, SolTrie *sol, long int *cap);
void getLeavesRecur(SolTrie ***leaves, SolTrie *sol, long int *cap, long int *index);

#endif
