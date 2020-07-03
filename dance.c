#include "dance.h"

int algorithmX(Dance *d)
{
   Doubly *hcol, *xrow;
   int x = 1, ret;
   SolTrie *sol;

   if(d->root == d->root->right)
   {
      addLeaf(&d->sols, d->csol, &d->solCap, &d->numSols);
      //printSingleSol(d->csol);
      return 0;
   }
   //hcol = randHCol(d);
   hcol = heuristic(d);
   if(hcol->drow == d->rmax)
      return 1;
   for(xrow = hcol->down; xrow != hcol; xrow = xrow->down)
   {
      sol = initTrie((void*)(xrow->hrow));
      addChild(d->csol, sol);
      d->csol = sol;
      coverRow(d, xrow);
      //printMatrix(d);
      if(0 == (ret = algorithmX(d)))
         x = 0;
      uncoverRow(d, xrow);
      //printMatrix(d);
      d->csol = d->csol->parent;
      if(ret == 1)
         deleteChild(d->csol, (void*)(xrow->hrow));
      if(x == 0)
         return 0;
   }

   return x;
}

int coverRow(Dance *d, Doubly *node)
{
   Doubly *xrow;

   for(xrow = node->right; xrow != node; xrow = xrow->right)
   {
      if(xrow->hcol == d->root)
         continue;
      coverCol(d, xrow);
   }
   coverCol(d, xrow);

   return 0;
}

int coverCol(Dance *d, Doubly *xrow)
{
   Doubly *xcol, *xrow2, *hcol;

   hcol = xrow->hcol;
   hcol->right->left = hcol->left;
   hcol->left->right = hcol->right;
   d->root->dcol--;

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

   uncoverCol(d, node);
   for(xrow = node->left; xrow != node; xrow = xrow->left)
   {
      if(xrow->hcol == d->root)
         continue;
      uncoverCol(d, xrow);
   }

   return 0;
}

int uncoverCol(Dance *d, Doubly *xrow)
{
   Doubly *xcol, *xrow2, *hcol;

   hcol = xrow->hcol;
   hcol->right->left = hcol;
   hcol->left->right = hcol;
   d->root->dcol++;

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
   Doubly *hcol, *minXs;

   for(hcol = minXs = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      if(hcol->drow < minXs->drow)
         minXs = hcol;
   }

   return minXs;
}

Doubly *randHCol(Dance *d)
{
   int i, randInt = rand() % d->root->dcol;
   Doubly *hcol;

   for(i = 0, hcol = d->root->right; i < randInt; i++, hcol = hcol->right);

   return hcol;
}

