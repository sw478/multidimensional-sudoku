#include "setup.h"
#include "solTrie.h"

int initDance(Dance *d, int x, int y)
{
   assert(d != NULL);

   d->x = x;
   d->y = y;
   d->ilist = 0;
   d->initListCap = 1;
   d->initList = malloc(sizeof(Doubly*));

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->xcol = d->xrow = d->root->hcol = d->root->hrow = d->root;

   d->hideRoot = malloc(sizeof(Doubly));
   d->hideRoot->next = d->hideRoot->prev = d->hideRoot;

   d->solRoot = initTrie(NULL);
   d->csol = d->solRoot->parent = d->solRoot;
   d->csol->minCol = d->root;
   d->numSols = 0;
   d->solCap = 1;
   d->sols = malloc(sizeof(SolTrie));

   return 0;
}

/*
 * initializes text file containing coordinates specific to the constrainsts
 * of a general n x m sudoku board with box, row, col constrainsts
 *
 * modular so that this function can be substituted by any other
 * function writing constraint coordinates for other constraint problems
 * you might want to solve, such as diagonal sudoku boards that have
 * extra diagonal constraints or for an n queens problems or pentomino tiling
 */

/*
 * for sudoku boards:
 * each board size is defined by x an y, where total board size is
 * gridSize = (x*y)^2
 * each row describes a specific position on the board filled with
 * a specific number (0 < num < x*y)
 * drow = (sudoku rows) * (sudoku cols * max num) + col * (max num) + num
 * dcol = constraint * (sudoku rows * sudoku cols) + row * (sudoku cols) + col
 *
 * constraints:
 *    0: row-col
 *    1: row-num
 *    2: col-num
 *    3: box-num
 */
int initMatrixFileSudoku(Dance *d, int x, int y)
{
   int igrid = 0, inum = 0, sr = 0, sc = 0, sb = 0, xy = x*y, gridSize = xy*xy;
   int irow, icol[4], i;
   char *fileName = malloc(BUFSIZE*sizeof(char));

   sprintf(fileName, "dance/d%dx%d.txt", y, x);

   d->rmax = xy*gridSize;
   d->cmax = 4*gridSize;

   if(access(fileName, F_OK) != -1)
   {
      d->init = fopen(fileName, "r+");
      free(fileName);
      return 1;
   }
   d->init = fopen(fileName, "w+");

   for(igrid = 0; igrid < gridSize; igrid++)
   {
      sr = igrid / xy;
      sc = igrid % xy;
      sb = (sr / y)*y + sc / x;

      for(inum = 0; inum < xy; inum++)
      {
         irow = igrid * xy + inum;
         icol[0] = igrid;
         icol[1] = inum + sr*xy + gridSize;
         icol[2] = inum + sc*xy + gridSize * 2;
         icol[3] = inum + sb*xy + gridSize * 3;

         for(i = 0; i < 4; i++)
            fprintf(d->init, "%d %d\n", irow, icol[i]);
      }
   }

   free(fileName);
   return 0;
}

/*
 * hides necessary rows and stores them in hideList
 * row headers are used to
 */
int hideRows(Dance *d, Sudoku *s)
{
   Doubly *row = d->root->down;
   int igrid, num;

   if(d->mode == 2)
      return 1;
   for(row = d->root->down; row != d->root; row = row->down)
   {
      igrid = (int)(row->drow / s->xy);
      num = row->drow % s->xy + 1;
      if(0 == s->grid[igrid] || num == s->grid[igrid])
         continue;
      hideRow(d, row);
   }
   return 0;
}

/*
 * covers/hides a row in a matrix given a Doubly and stores in hideList
 * added to front of hideList
 *
 * done one row at a time
 *
 * 1 Hide is malloced here
 */
int hideRow(Dance *d, Doubly *row)
{
   Hide *hideRow = malloc(sizeof(Hide));
   Doubly *xrow;

   hideRow->next = d->hideRoot->next;
   hideRow->prev = d->hideRoot;
   d->hideRoot->next->prev = hideRow;
   d->hideRoot->next = hideRow;

   hideRow->xrow = row;
   row->up->down = row->down;
   row->down->up = row->up;
   for(xrow = row->right; xrow != row; xrow = xrow->right)
   {
      xrow->up->down = xrow->down;
      xrow->down->up = xrow->up;
      xrow->hcol->drow--;
   }

   return 0;
}

/* 
 * recovers and frees all rows in hideList
 */
int recoverHiddenRows(Dance *d)
{
   Hide *temp, *hideRow = d->hideRoot->next;
   Doubly *xrow;

   while(hideRow != d->hideRoot)
   {
      xrow = hideRow->xrow->right;
      while(xrow != hideRow->xrow)
      {
         xrow->up->down = xrow;
         xrow->down->up = xrow;
         xrow->hcol->drow++;
         xrow = xrow->right;
      }
      xrow->up->down = xrow;
      xrow->down->up = xrow;

      temp = hideRow;
      hideRow->prev->next = hideRow->next;
      hideRow->next->prev = hideRow->prev;
      hideRow = hideRow->next;
      free(temp);
   }
   assert(hideRow->next == hideRow);
   assert(hideRow->prev == hideRow);

   return 0;
}
