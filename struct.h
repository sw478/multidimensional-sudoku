#ifndef STRUCT_H
#define STRUCT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include "unistd_io.h" /* unistd for linux, io.h for windows */
#include "config.h"
#define F_OK 0

/* for d->problem */
#define SUDOKU 0
#define SUDOKU2 1
#define NQUEENS 2
#define SGEN 3

/*
 * a two dimensional linked list of heur headers
 * 
 * heur headers are heads of lists that holds all
 * heurs with the same number of elements beneath them together
 *
 * heurs are doubly linked to each column header
 *
 * incHeur and decHeur are the main operations
 * when a doubly under a column header is covered or uncovered, the
 * column header's element count is appropriately incremented or
 * decremented, the heur it's linked to is moved from one heur list
 * to another, and a heur headers are created and destroyed when a
 * new column is needed or when a heur column no longer has elements
 *
 * probably plan to refactor later since hcol heurs and heur headers'
 * data can be separated into two distinct structs
 */
typedef struct heur
{
   /* used for hcol heurs */
   void *hcol;
   struct heur *heurHeader, *prev, *next;

   /* used for heur headers */
   struct heur *hprev, *hnext;

   /* used for both */
   int num;
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

   /* misc. data */
   /* could probably move to a struct made for headers that contain doubly */
   Heur *heur;
   int rowIsHidden;
   uint16_t *rowLayout;
} Doubly;

/*
 * each hide is associated with a sudoku cell
 * hrows: each cell is associated with a group of matrix rows, formatted
 * as a list of pointers
 * num: the number for the cell (must be known, so only for given numbers when
 * solving a board), and shouldn't change throughout the course of the program
 * filled: whether or not the cell has been filled with the number, and
 * whether or not the rows have been hidden (should be hidden if filled)
 * 1 - filled/hidden, 0 - empty/unhidden
 */
typedef struct hide
{
   Doubly **hrows;
   int num, filled;
} Hide;

/*
 * when solving an exact cover problem, a set of rows constitute a solution,
 * and algorithm X finds these rows one at a time, but searches for them using
 * a depth first approach, so what you get is a tree with rows as the nodes,
 * and in this case, the struct solTree acts as the node.
 *
 * row: matrix row associated with the solTree
 * child: pointer array of child nodes
 * parent: each node's children's parent node points to this node
 * ichild: child array's next available index
 * cap: current array capacity
 * numSols: number of leaf nodes including and underneath this current solTree
 */
typedef struct solTree
{
   Doubly *row;
   struct solTree **childList, *parent;
   int ichild, cap;
} SolTree;

/*
 * grid: numbers of the sudoku board, indexed on the board from left to right
 * x & y: dimensions of a single sudoku box
 * xy: x*y, max number on the sudoku board
 * z: in the future, planning on expanding the program's capacbilities to solve
 * boards of higher dimensions
 * gridSize: xy*xy, total size of sudoku board
 * mode: 0 - generating, 1 - solving
 * filled: number of cells filled, used for unhideRandom
 *
 * in: file pointer for sudoku text file, not to be confused with matrix text file
 */
typedef struct
{
   int *grid, x, y, xy, gridSize;
   FILE *boardFile;
} Sudoku;

typedef struct
{
   /* root of matrix */
   Doubly *root;
   /* matrix dimensions */
   int rmax, cmax;
   /* number of times AlgX is called */
   int numCalls;
   FILE *matrixFile;

   /* problem specific data */
   int problem;
   Sudoku *s;
   int nq;
   int max16mult;
   int singleSolFound;

   /*
      set to 1 if you want algX to stop after finding
      the first solution
   */
   int stopAfterFirstSol;
   
   /* for secondary columns */
   Doubly *hcol_sec;
   int sec_hcol_index;
   
   /* for initializing matrix */
   Doubly **initList;
   int ilist, initListCap;

   /* for storing solutions */
   SolTree *csol, **sols;
   int numSols, solCap;

   /* for optional pre processed row covering */
   Hide **hideList;
   int hidden, hideCap, ihide;

   /* for column heuristic */
   Heur *heurRoot;

} Dance;

#endif
