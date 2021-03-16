#include "hide.h"
#include "heuristic.h"
#include "auxil.h"

/* TODO */

/*
 * initializes hide structure
 *
 * for solving, should be done at the beginning of program
 * for generating, should be done at the end, when all the
 * cell values are known
 * 
 * cs1 is xy-1 since the number of row headers that can be covered
 * for a single cell is one less than the # of possible cell numbers
 *
 * uses the values from d->s->sudoku, so for generation must
 * be called after saveSolution
 *
 * doesn't hide any cell rows, just sets everything up
 *
 * d->hideList is allocated memory in initDance
 * 
 * all cells are initialized unfilled
 */
int initHide_Sudoku(Dance *d)
{
   Doubly *xrow;
   int *sudoku = d->s->sudoku, iSudoku, containerSize = d->s->containerSize;
   int sudokuSize = d->s->sudokuSize, ihide, num;
   Hide *h;

   d->hideList = malloc(sudokuSize*sizeof(Hide*));
   d->hideRoot = malloc(sizeof(Hide));
   d->hideRoot->prev = d->hideRoot->next = d->hideRoot;
   d->hideRoot->num = 0; /* number of cells filled in sudoku */

   xrow = d->root->down;
   
   for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
   {
      h = malloc(sizeof(Hide));
      h->next = d->hideRoot;
      h->prev = d->hideRoot->prev;
      d->hideRoot->prev->next = h;
      d->hideRoot->prev = h;

      h->num = sudoku[iSudoku];
      d->hideList[iSudoku] = h;
      h->hrows = malloc((containerSize-1)*sizeof(Doubly));
      h->filled = 0;
      h->iSudoku = iSudoku;

      if(sudoku[iSudoku] == 0)
      {
         for(ihide = 0; ihide < containerSize; ihide++, xrow = xrow->down);
         continue;
      }

      for(ihide = 0; ihide < containerSize-1; xrow = xrow->down)
      {
         num = (xrow->drow % containerSize) + 1;
         if(sudoku[iSudoku] == num)
            continue;
         h->hrows[ihide] = xrow;
         ihide++;
      }
      if(num != containerSize)
         xrow = xrow->down;
    
   }
   d->ihide = sudokuSize;

   return 0;
}

int fillSingleCell(Dance *d, Hide *h)
{
   Doubly *hrow, *xrow;
   int ihide, cs1 = d->s->containerSize - 1;

   /* check when generating */
   if(h->filled)
      return 1;
   
   /* cover hide */
   h->next->prev = h->prev;
   h->prev->next = h->next;
   d->hideRoot->num++;

   if(h->num == 0)
      return 0;

   for(ihide = 0; ihide < cs1; ihide++)
   {
      hrow = h->hrows[ihide];
      hrow->up->down = hrow->down;
      hrow->down->up = hrow->up;
      for(xrow = hrow->right; xrow != hrow; xrow = xrow->right)
      {
         xrow->up->down = xrow->down;
         xrow->down->up = xrow->up;
         xrow->hcol->drow--;
         xrow->hrow->dcol--;
         HEUR_DEC(d, xrow->hcol->heur, 1)
      }
   }
   h->filled = 1;

   return 0;
}

int unfillSingleCell(Dance *d, Hide *h)
{
   Doubly *hrow, *xrow;
   int ihide, cs1 = d->s->containerSize - 1;

   /* check when generating */
   if(!h->filled)
      return 1;
   
   /* cover hide */
   h->next->prev = h;
   h->prev->next = h;
   d->hideRoot->num--;

   if(h->num == 0)
      return 0;

   for(ihide = 0; ihide < cs1; ihide++)
   {
      hrow = h->hrows[ihide];
      hrow->up->down = hrow;
      hrow->down->up = hrow;
      for(xrow = hrow->right; xrow != hrow; xrow = xrow->right)
      {
         xrow->up->down = xrow;
         xrow->down->up = xrow;
         xrow->hcol->drow++;
         xrow->hrow->dcol++;
         HEUR_INC(d, xrow->hcol->heur, 1)
      }
   }
   h->filled = 0;

   return 0;
}

void fillAllCells(Dance *d)
{
   int iSudoku, sudokuSize = d->s->sudokuSize;

   for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
      fillSingleCell(d, d->hideList[iSudoku]);
}

void unfillAllCells(Dance *d)
{
   int iSudoku, sudokuSize = d->s->sudokuSize;

   for(iSudoku = 0; iSudoku < sudokuSize; iSudoku++)
      unfillSingleCell(d, d->hideList[iSudoku]);
}

void freeHide(Dance *d)
{
   int ihide;

   for(ihide = 0; ihide < d->ihide; ihide++)
   {
      free(d->hideList[ihide]->hrows);
      free(d->hideList[ihide]);
   }

   free(d->hideList);
   free(d->hideRoot);
}