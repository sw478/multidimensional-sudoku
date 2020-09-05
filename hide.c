#include "hide.h"
#include "heuristic.h"
#include "aux.h"

/*
 * initializes hide structure
 *
 * for solving, should be done at the beginning of program
 * for generating, should be done at the end, when all the
 * cell values are known
 * 
 * xy1 is xy-1 since the number of row headers that can be covered
 * for a signle cell is one less than the # of possible cell numbers
 *
 * uses the values from d->s->grid, so for generation must
 * be called after saveSolution
 *
 * doesn't hide any cell rows, just sets everything up
 *
 * d->hideList is allocated memory in initDance
 */
int initHide(Dance *d)
{
   Doubly *xrow;
   int *grid = d->s->grid, igrid, xy = d->s->xy, gridSize = d->s->gridSize;
   int ihide, num;
   Hide *h;

   xrow = d->root->down;
   for(igrid = 0; igrid < gridSize; igrid++)
   {
      h = malloc(sizeof(Hide));
      h->num = grid[igrid];
      d->hideList[igrid] = h;
      h->hrows = malloc((xy-1)*sizeof(Doubly));
      h->filled = 0;

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
   assert(xrow == d->root);

   return 0;
}

int hideSingleCell(Dance *d, int igrid)
{
   Hide *h = d->hideList[igrid];
   Doubly *hrow, *xrow;
   int ihide, xy1 = d->s->xy - 1;

   if(h->num == 0)
      return 0;
   /* check when generating */
   if(h->filled)
      return 1;
   for(ihide = 0; ihide < xy1; ihide++)
   {
      hrow = h->hrows[ihide];
      for(xrow = hrow->right; xrow != hrow; xrow = xrow->right)
      {
         xrow->up->down = xrow->down;
         xrow->down->up = xrow->up;
         xrow->hcol->drow--;
         decHeur(xrow->hcol->heur);
      }
   }
   h->filled = 1;

   return 0;
}

int unhideSingleCell(Dance *d, int igrid)
{
   Hide *h = d->hideList[igrid];
   Doubly *hrow, *xrow;
   int ihide, xy1 = d->s->xy - 1;

   if(h->num == 0)
      return 0;
   /* check when generating */
   if(!h->filled)
      return 1;

   for(ihide = 0; ihide < xy1; ihide++)
   {
      hrow = h->hrows[ihide];
      for(xrow = hrow->right; xrow != hrow; xrow = xrow->right)
      {
         xrow->up->down = xrow;
         xrow->down->up = xrow;
         xrow->hcol->drow++;
         incHeur(xrow->hcol->heur);
      }
   }
   h->filled = 0;

   return 0;
}

void hideAllCells(Dance *d)
{
   int igrid, gridSize = d->s->gridSize;

   for(igrid = 0; igrid < gridSize; igrid++)
      hideSingleCell(d, igrid);
}

void unhideAllCells(Dance *d)
{
   int igrid, gridSize = d->s->gridSize;

   for(igrid = 0; igrid < gridSize; igrid++)
      unhideSingleCell(d, igrid);
}

void freeHide(Dance *d)
{
   int igrid;
   for(igrid = 0; igrid < d->s->gridSize; igrid++)
   {
      free(d->hideList[igrid]->hrows);
      free(d->hideList[igrid]);
   }

   free(d->hideList);
}
