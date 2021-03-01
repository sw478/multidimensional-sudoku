#include "end.h"
#include "solTrie.h"
#include "heuristic.h"
#include "hide.h"

void freeDance(Dance *d)
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

   freeSol(d->solRoot);
   free(d->initList);

   //freeHide(d);
   freeHeur(d);
   free(d->sols);

   fclose(d->s->in);
   free(d->s->grid);
   free(d->s);

   free(d);
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

