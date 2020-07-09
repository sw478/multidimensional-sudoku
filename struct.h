#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#define BUFSIZE 1000

/*
 * a 2-d linked list of heur headers that holds all heurs mapped to hcol with
 * the same number of elements beneath them together
 */
typedef struct heur
{
   /* used for hcol heurs */
   void *hcol;
   struct heur *head, *prev, *next;

   /* used for heur headers */
   int num;
   struct heur *hprev, *hnext;
} Heur;

/*
 * columns describe the constraints
 * if Doubly is a root, drow = d->root->rmax, dcol = d->root->cmax
 *
 * to reduce redundancy, if doubly is not header:
 * 0 < drow < d->root->rmax and
 * 0 < dcol < d->root->cmax
 * if a header:
 * drow >= d->root->rmax
 * dcol >= d->root->cmax
 * drow/dcol for headers are used to keep track of number of elements
 * in their row or col
 * pointers to adjacent nodes and to header nodes
 */
typedef struct doubly
{
   int drow, dcol;
   struct doubly *up, *down, *left, *right, *hcol, *hrow;
   Heur *heur;
} Doubly;

typedef struct hide
{
   Doubly *xrow;
   struct hide *next, *prev;
} Hide;

typedef struct solTrie
{
   Doubly *row, **minList;
   struct solTrie **child, *parent;
   int ichild, cap, numSols;
   int minSize, minIndex;
} SolTrie;

typedef struct
{
   int rmax, cmax, mode, x, y, ilist, initListCap, numCalls;
   long int numSols, solCap;
   Doubly *root, *xrow, *xcol, **initList;
   SolTrie *solRoot, *csol, **sols;
   Doubly *curRow;
   Hide *hideRoot;
   Heur *heurRoot;
   FILE *init;
} Dance;

typedef struct
{
   int *grid, x, y, xy, z, gridSize;
   int elements, steps, fit, visited, mode;
   FILE *in;
} Sudoku;

#endif
