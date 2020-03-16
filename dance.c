#include "dance.h"

int algorithmX(Dance *d)
{
   Doubly *hcol, *xrow;
   int x = 1;

   if(d->root == d->root->right)
      return 0;
   hcol = heuristic(d);
   if(hcol->drow == d->rmax)
      return 1;
   for(xrow = hcol->down; xrow != hcol; xrow = xrow->down)
   {
      coverRow(d, xrow);
      //printMatrix(d);
      x = algorithmX(d);
      uncoverRow(d, xrow);
      //printMatrix(d);
      if(x == 0)
         return storeSol(d, xrow);
   }

   return 1;
}

int storeSol(Dance *d, Doubly *xrow)
{
   d->sol[d->isol] = xrow->drow;
   d->isol++;
   return 0;
}

int coverRow(Dance *d, Doubly *node)
{
   Doubly *xrow;

   for(xrow = node->right; xrow != node; xrow = xrow->right)
      coverCol(d, xrow);
   coverCol(d, xrow);

   return 0;
}

int coverCol(Dance *d, Doubly *xrow)
{
   Doubly *xcol, *xrow2, *hcol;

   hcol = xrow->hcol;
   hcol->right->left = hcol->left;
   hcol->left->right = hcol->right;

   for(xcol = hcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
      {
         xrow2->up->down = xrow2->down;
         xrow2->down->up = xrow2->up;
         xrow2->hcol->drow--;
      }
   }

   return 0;
}

int uncoverRow(Dance *d, Doubly *node)
{
   Doubly *xrow;

   for(xrow = node->left; xrow != node; xrow = xrow->left)
      uncoverCol(d, xrow);
   uncoverCol(d, xrow);

   return 0;
}

int uncoverCol(Dance *d, Doubly *xrow)
{
   Doubly *xcol, *xrow2, *hcol;

   hcol = xrow->hcol;
   hcol->right->left = hcol;
   hcol->left->right = hcol;

   for(xcol = hcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
      {
         xrow2->up->down = xrow2;
         xrow2->down->up = xrow2;
         xrow2->hcol->drow++;
      }
   }

   return 0;
}

Doubly *heuristic(Dance *d)
{
   Doubly *hcol = d->root->right, *result;

   result = hcol;
   for(; hcol != d->root; hcol = hcol->right)
   {
      if(hcol->drow < result->drow)
         result = hcol;
   }

   return result;
}
