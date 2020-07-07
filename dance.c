#include "dance.h"
#include "solTrie.h"
#include "aux.h"

/*
 * returns 0 if a solution was found (not leaf)
 * 1 if no solution found
 * 2 if solution was found and is a leaf
 */
int algorithmX(Dance *d)
{
   Doubly *hcol, *xrow;
   int x = 1, ret, numSol = 0;
   /*int listSize, *hitList;*/
   SolTrie *sol;

   if(d->root == d->root->right)
   {
      d->csol = d->solRoot;
      return 2;
   }
   d->numCalls++;

   /*hcol = d->root->right;*/
   hcol = heuristic(d);
   /*printColHeaders(d);*/

/* 
 * listSize = hcol->drow - d->rmax;
 * hitList = calloc(listSize, sizeof(int));
 */

   xrow = hcol->down;
   /*xrow = nextRow(hcol, &listSize, &hitList);*/
   for(; xrow != hcol; xrow = xrow->down)
   /*nextRow(hcol, &listSize, &hitList))*/
   {
      coverRow(d, xrow);
      ret = algorithmX(d);
      if(ret == 0 || ret == 2)
         x = 0;
      uncoverRow(d, xrow);

      if(x == 0)
      {
         if(numSol == 0)
            sol = initTrie(xrow->hrow);
         if(d->csol != d->solRoot)
            addChild(sol, d->csol);
         d->csol = sol;
         numSol++;
         if(ret == 2)
            addLeaf(d);
         if(d->mode == 2)
            break;
      }
   }

   /*free(hitList);*/
   return x;
}

Doubly *nextRow(Doubly *hcol, int *num, int **hitList)
{
   Doubly *row;
   int i, j, randInt;
   if(*num == 0)
      return hcol;

   randInt = rand() % *num;
   for(row = hcol->down, i = 0; i < randInt; i++, row = row->down);
   for(j = i; (*hitList)[j] == 1; j++, row = row->down)
   {
      if(row == hcol)
         row = row->down;
   }

   (*num)--;
   (*hitList)[j] = 1;
   return row;
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
   d->root->dcol--;

   for(xcol = hcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
      {
         xrow2->up->down = xrow2->down;
         xrow2->down->up = xrow2->up;
         xrow2->hcol->drow--;
         xrow2->hrow->dcol--;
      }
   }

   return 0;
}

int uncoverRow(Dance *d, Doubly *node)
{
   Doubly *xrow;

   uncoverCol(d, node);
   for(xrow = node->left; xrow != node; xrow = xrow->left)
      uncoverCol(d, xrow);

   return 0;
}

int uncoverCol(Dance *d, Doubly *xrow)
{
   Doubly *xcol, *xrow2, *hcol;

   hcol = xrow->hcol;
   hcol->right->left = hcol;
   hcol->left->right = hcol;
   d->root->dcol++;

   for(xcol = hcol->up; xcol != hcol; xcol = xcol->up)
   {
      for(xrow2 = xcol->left; xrow2 != xcol; xrow2 = xrow2->left)
      {
         xrow2->up->down = xrow2;
         xrow2->down->up = xrow2;
         xrow2->hcol->drow++;
         xrow2->hrow->dcol++;
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
