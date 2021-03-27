#include "free.h"
#include "solTree.h"
#include "heuristic.h"
#include "hide.h"

void freeDance(Dance *d)
{
   freeMatrix(d);

   free(d->sols);
   if(d->numSols > 0)
      freeTree(d->csol);

   freeHide(d);

   HEUR_FREE(d)

   fclose(d->s->boardFile);
   fclose(d->s->solFile);
   free(d->s->sudoku);
   free(d->s->dim);
   free(d->s);

   free(d);
}

void freeMatrix(Dance *d)
{
   Doubly *col, *temp;

   for(col = d->root->right; col != d->root;)
   {
      freeColumn(col);
      temp = col;
      col = col->right;
      free(temp);
   }
   freeColumn(col);
   free(d->root);
}

void freeColumn(Doubly *col)
{
   Doubly *row, *temp;

   for(row = col->down; row != col;)
   {
      temp = row;
      row = row->down;
      free(temp);
   }
}

