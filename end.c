#include "dance.h"
#include "setup.h"

void freeDance(Dance *d)
{
   Doubly *col, *temp;

   for(col = d->root->right; col != d->root;)
   {
      freeColumn(col);
      temp = col;
      col = col->right;
      freeDoubly(temp);
   }
   freeColumn(col);
   free(d->root);

   freeSol(d->solRoot);
   free(d->hideRoot);
   free(d->initList);
   free(d);
}

void freeColumn(Doubly *col)
{
   Doubly *row, *temp;

   for(row = col->down; row != col;)
   {
      temp = row;
      row = row->down;
      freeDoubly(temp);
   }
}

void freeDoubly(Doubly *node)
{
   node->left->right = node->right;
   node->right->left = node->left;
   node->down->up = node->up;
   node->up->down = node->down;
   free(node);
}
