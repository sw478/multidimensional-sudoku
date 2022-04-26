#include "hrowCover.h"

void coverRowHeaders(Dance *d)
{
   Doubly *hrow;

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      hrow->right->left = hrow->left;
      hrow->left->right = hrow->right;
   }
}

void uncoverRowHeaders(Dance *d)
{
   Doubly *hrow;

   for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
   {
      hrow->right->left = hrow;
      hrow->left->right = hrow;
   }
}