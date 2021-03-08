#include "free.h"
#include "solTrie.h"
#include "heuristic.h"
#include "hide.h"
#include "initHrowLayout.h"

void freeDance(Dance *d)
{
   freeMatrix(d);

   freeSol(d->solRoot);

   if(d->problem == 0 || d->problem == 1)
      freeHide(d);

   if(USE_HEUR)
      freeHeur(d);
   free(d->sols);

   if(d->problem == 0 || d->problem == 1)
   {
      fclose(d->s->in);
      free(d->s->grid);
      free(d->s);
   }

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

