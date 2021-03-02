#include "hide.h"
#include "heuristic.h"
#include "auxil.h"

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
 */
int initHide_Sudoku(Dance *d)
{
   Doubly *xrow;
   int *grid = d->s->grid, igrid, xy = d->s->xy, gridSize = d->s->gridSize;
   int ihide, num;
   Hide *h;

   d->hideList = malloc(d->s->gridSize*sizeof(Hide*));

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
   d->ihide = d->s->gridSize;

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
         decHeur(d, xrow->hcol->heur, 1);
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
         incHeur(d, xrow->hcol->heur, 1);
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
   int ihide;

   for(ihide = 0; ihide < d->ihide; ihide++)
   {
      free(d->hideList[ihide]->hrows);
      free(d->hideList[ihide]);
   }

   free(d->hideList);
}

void saveGeneratedPuzzle(Dance *d)
{
   int igrid;

   for(igrid = 0; igrid < d->s->gridSize; igrid++)
   {
      if(d->hideList[igrid]->filled == 0)
         d->s->grid[igrid] = 0;
   }
}

/*
   in this variation, there will only be one hide struct in d->hideList,
   and its hrows will contain all the rows to be hidden.

   in the original, we traversed down the row headers, but in this, we traverse
   the column headers by grid number

   h->num and h->filled aren't used in this
*/
int hide_Sudoku2(Dance *d)
{
   Doubly *doub, *hcol, *hrow;
   int *grid = d->s->grid, igrid, xy = d->s->xy, gridSize = d->s->gridSize;
   int ihide, num, hideCap = 100;
   Hide *h;

   d->hideList = malloc(sizeof(Hide*));
   h = malloc(sizeof(Hide));
   d->hideList[0] = h;
   
   ihide = 0;
   h->hrows = malloc(hideCap*sizeof(Doubly));

   /* skip the first xy columns */
   for(hcol = d->root->right; hcol->dcol < xy; hcol = hcol->right);

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
      hrow->rowIsHidden = 0;

   for(igrid = 0; igrid < gridSize; hcol = hcol->right, igrid++)
   {
      num = grid[igrid];
      if(num == 0)
         continue;
      num--; /* adjust for off-by-one error */

      for(doub = hcol->down; doub != hcol; doub = doub->down)
      {
         if(doub->drow % xy == num)
            continue;
         hrow = doub->hrow;
         if(hrow->rowIsHidden)
            continue;
         hrow->rowIsHidden = 1;
         h->hrows[ihide] = hrow;
         hideSingleRow(d, hrow);

         ihide++;
         if(ihide > hideCap)
         {
            hideCap *= 2;
            h->hrows = realloc(h->hrows, hideCap*sizeof(Doubly));
         }
      }
   }

   h->hrows = realloc(h->hrows, ihide*sizeof(Doubly));
   d->ihide = ihide;

   return 0;
}

int unhide_Sudoku2(Dance *d)
{
   Doubly *hrow;
   int i;
   Hide *h;

   h = d->hideList[0];
   for(i = 0; i < d->ihide; i++)
   {
      hrow = h->hrows[i];
      hrow->rowIsHidden = 0;
      unhideSingleRow(d, hrow);
   }
   d->ihide = 1;

   return 0;
}

int hideSingleRow(Dance *d, Doubly *hrow)
{
   Doubly *doub;

   for(doub = hrow->right; doub != hrow; doub = doub->right)
   {
      doub->hcol->drow--;
      doub->hrow->dcol--;
      doub->up->down = doub->down;
      doub->down->up = doub->up;
      decHeur(d, doub->hcol->heur, 1);
   }

   hrow->up->down = hrow->down;
   hrow->down->up = hrow->up;

   return 0;
}

int unhideSingleRow(Dance *d, Doubly *hrow)
{
   Doubly *doub;

   for(doub = hrow->right; doub != hrow; doub = doub->right)
   {
      doub->hcol->drow++;
      doub->hrow->dcol++;
      doub->up->down = doub;
      doub->down->up = doub;
      incHeur(d, doub->hcol->heur, 1);
   }

   hrow->up->down = hrow;
   hrow->down->up = hrow;

   return 0;
}