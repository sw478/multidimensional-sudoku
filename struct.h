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
#define BUFSIZE 1000
#define F_OK 0

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
 * actually a tree not a trie
 *
 * when solving an exact cover problem, a set of rows constitute a solution,
 * and algorithm X finds these rows one at a time, but searches for them using
 * a depth first approach, so what you get is a tree with rows as the nodes,
 * and in this case, the struct solTrie acts as the node.
 *
 * row: matrix row associated with the solTrie
 * child: pointer array of child nodes
 * parent: each node's children's parent node points to this node
 * ichild: child array's next available index
 * cap: current array capacity
 * numSols: number of leaf nodes including and underneath this current solTrie
 */
typedef struct solTrie
{
   Doubly *row;
   struct solTrie **child, *parent;
   int ichild, cap, numSols;
} SolTrie;

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
   int *grid, x, y, xy, z, gridSize, mode;
   FILE *in;
} Sudoku;

/*
 * rmax, cmax: dimensions of matrix
 * numCalls: # of times algorithm X is called
 * root: entry point for matrix
 * xrow & xcol: used for tracking doubly
 *
 * sewing:
 * initList: list of all the Doubly when initializing the matrix. used for sorting
 * ilist: next available index in initList
 * initListCap: capacity of initList
 *
 * storing solutions:
 * solRoot: root node for solTrie tree
 * csol: current solTrie being tracked
 * sols: list of leaf nodes, # of leaf nodes is # of distinct solutions
 * numSols: next available index in sols
 * solCap: current capacity of sols, doubles when reached
 * 
 * typical case for solving sudokus is to have only one solution
 * which is why solCap usually stays at 1, and would make the tree behave
 * as doubly linked list
 *
 * hideList: list of pointers to Hide structs
 * hidden: number of cells currently hidden
 * heurRoot: dummy node/entry point for heuristic structure
 *
 * init: file pointer for matrix text file (not to be confused with sudoku text file)
 */
typedef struct
{
   Sudoku *s;
   int nq;
   /* 
      0 for sudoku
      1 for sudoku2
      2 for n queens
   */
   int problem;
   
   /* for secondary columns */
   Doubly *hcol_sec;
   int sec_hcol_index;

   int rmax, cmax, numCalls;
   Doubly *root, *xrow, *xcol;

   Doubly **initList;
   int ilist, initListCap;

   SolTrie *solRoot, *csol, **sols;
   long int numSols, solCap; 

   Hide **hideList;
   int hidden, hideCap, ihide;

   Heur *heurRoot;
   FILE *init;

   int max16mult;
} Dance;

#endif
