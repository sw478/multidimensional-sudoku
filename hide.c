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
 * xy1 is xy-1 since the number of row headers that can be covered
 * for a single cell is one less than the # of possible cell numbers
 *
 * uses the values from d->s->grid, so for generation must
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
   int *grid = d->s->sudoku, igrid, xy = d->s->xy, gridSize = d->s->gridSize;
   int ihide, num;
   Hide *h;

   d->hideList = malloc(d->s->gridSize*sizeof(Hide*));
   d->hideRoot = malloc(sizeof(Hide));
   d->hideRoot->prev = d->hideRoot->next = d->hideRoot;
   d->hideRoot->num = 0; /* number of cells filled in grid */

   xrow = d->root->down;
   
   for(igrid = 0; igrid < gridSize; igrid++)
   {
      h = malloc(sizeof(Hide));
      h->next = d->hideRoot;
      h->prev = d->hideRoot->prev;
      d->hideRoot->prev->next = h;
      d->hideRoot->prev = h;

      h->num = grid[igrid];
      d->hideList[igrid] = h;
      h->hrows = malloc((xy-1)*sizeof(Doubly));
      h->filled = 0;
      h->igrid = igrid;

      if(grid[igrid] == 0)
      {
         for(ihide = 0; ihide < xy; ihide++, xrow = xrow->down);
         continue;
      }

      for(ihide = 0; ihide < xy-1; xrow = xrow->down)
      {
         num = (xrow->drow % xy) + 1;
         if(grid[igrid] == num)
            continue;
         h->hrows[ihide] = xrow;
         ihide++;
      }
      if(num != xy)
         xrow = xrow->down;
    
   }
   d->ihide = d->s->gridSize;

   return 0;
}

int fillSingleCell(Dance *d, Hide *h)
{
   Doubly *hrow, *xrow;
   int ihide, xy1 = d->s->xy - 1;

   /* check when generating */
   if(h->filled)
      return 1;
   
   /* cover hide */
   h->next->prev = h->prev;
   h->prev->next = h->next;
   d->hideRoot->num++;

   if(h->num == 0)
      return 0;

   for(ihide = 0; ihide < xy1; ihide++)
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
         HEUR_DEC(d, 0, 1, xrow->hcol->heur)
      }
   }
   h->filled = 1;

   return 0;
}

int unfillSingleCell(Dance *d, Hide *h)
{
   Doubly *hrow, *xrow;
   int ihide, xy1 = d->s->xy - 1;

   /* check when generating */
   if(!h->filled)
      return 1;
   
   /* cover hide */
   h->next->prev = h;
   h->prev->next = h;
   d->hideRoot->num--;

   if(h->num == 0)
      return 0;

   for(ihide = 0; ihide < xy1; ihide++)
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
         HEUR_INC(d, 0, 1, xrow->hcol->heur)
      }
   }
   h->filled = 0;

   return 0;
}

void fillAllCells(Dance *d)
{
   int igrid, gridSize = d->s->gridSize;

   for(igrid = 0; igrid < gridSize; igrid++)
      fillSingleCell(d, d->hideList[igrid]);
}

void unfillAllCells(Dance *d)
{
   int igrid, gridSize = d->s->gridSize;

   for(igrid = 0; igrid < gridSize; igrid++)
      unfillSingleCell(d, d->hideList[igrid]);
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

/* translates filled status of cells to d->s */
void saveGeneratedPuzzle(Dance *d)
{
   int igrid;

   for(igrid = 0; igrid < d->s->gridSize; igrid++)
   {
      if(d->hideList[igrid]->filled == 0)
         d->s->sudoku[igrid] = 0;
   }
}