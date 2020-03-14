#ifndef DANCE_H
#define DANCE_H
#include "sudoku.h"

#define RMAX 10 /*729*/
#define CMAX 15 /*324*/

typedef struct doubly
{
/* 
 * describes the rows and columns of the binary matrix
 * drow = row * 81 + col * 9 + num
 * dcol = constraint * 81 + row * 9 + col
 *
 * if Doubly is a root, drow = d->root->rmax, dcol = d->root->cmax
 * 
 * IMPORTANT: for space saving purposes, if doubly is not header:
 * (drow < d->root->rmax && drow >= 0)
 * if a header: (drow >= d->root->rmax)
 * drow will instead represent: (number of 1's in that column + RMAX)
 * ex. if drow = 745, drow is a header with 16 1's in that column
 *
 * constraint #: 1: row-col, 2: row-num, 3: col-num, 4: box-num
 */

   int drow, dcol;
   struct doubly *up, *down, *left, *right;
} Doubly;

typedef struct
{
   int rmax, cmax;
   Doubly *root;
} Dance;

int initSudokuMatrix(Dance *d);
Doubly *heuristic(Dance *d);

int initDance(Sudoku *s);
int initRoot(Dance *d);
int initHeaders(Dance *d);
int initDoubly(Dance *d, int drow, int dcol);
void freeDance(Dance *d);
void freeColumn(Doubly *col);
void coverDoubly(Doubly *node);

void testAddAllDoubly(Dance *d);
void printMatrix(Dance *d);
void printNodeInfo(Doubly *node);

#endif
