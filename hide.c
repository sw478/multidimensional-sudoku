#include "hide.h"
#include "heuristic.h"

/*
 * hides necessary rows and stores them in hideList
 * row headers are used to
 */
int hideRows(Dance *d)
{
   Doubly *row = d->root->down;
   int *grid = d->s->grid, igrid, num, xy = d->s->xy;

   if(d->s->mode == 2)
      return 1;
   for(row = d->root->down; row != d->root; row = row->down)
   {
      igrid = (int)(row->drow / xy);
      num = row->drow % xy + 1;
      if(0 == grid[igrid] || num == grid[igrid])
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
   //row->up->down = row->down;
   //row->down->up = row->up;
   for(xrow = row->right; xrow != row; xrow = xrow->right)
   {
      xrow->up->down = xrow->down;
      xrow->down->up = xrow->up;
      xrow->hcol->drow--;
      decHeur(xrow->hcol->heur);
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
         incHeur(xrow->hcol->heur);
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
