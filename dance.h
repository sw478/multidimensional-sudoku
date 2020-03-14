#ifndef DANCE_H
#define DANCE_H
#include "sudoku.h"

#define RMAX 10 /*729*/
#define CMAX 15 /*324*/

typedef struct doubly
{
/* describes the rows of the binary matrix
 * drow = row * 81 + col * 9 + num
 * if Doubly is a root, drow = RMAX
 * 
 * IMPORTANT: for space saving purposes, if doubly is not header:
 * (drow < RMAX && drow >= 0)
 * if a header: (drow >= RMAX)
 * drow will instead represent: (number of 1's in that column + RMAX)
 * ex. if drow = 745, drow is a header with 16 1's in that column
 */

   int drow;

/* describes the columns of the binary matrix
 * dcol = constraint * 81 + row * 9 + col
 * if Doubly is a root, dcol = CMAX
 *  
 * constraint #: 1: row-col, 2: row-num, 3: col-num, 4: box-num
 */

   int dcol;

/*
 * up/down is traversal of rows
 * left/right is traversal of cols
 */

   struct doubly *up, *down, *left, *right;
} Doubly;

typedef struct
{
   Doubly *root;
} Dance;

int initDance(Sudoku *s);
int initRoot(Dance *d);
int initHeaders(Dance *d);
int initDoubly(Dance *d, int drow, int dcol);
void freeDance(Dance *d);
void freeColumn(Doubly *col);
void coverDoubly(Doubly *temp);

void testAddAllDoubly(Dance *d);
void printMatrix(Dance *d);
void printNodeInfo(Doubly *node);

#endif
