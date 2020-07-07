#include "dance.h"
#include "solTrie.h"
#include "aux.h"

int algorithmX(Dance *d)
{
   Doubly *hcol, *xrow;
   int x = 1, ret; /*, listSize, *hitList;*/
   SolTrie *sol;

   if(d->root == d->root->right)
   {
      addLeaf(d);
      return 0;
   }

   hcol = heuristic(d);
   printf("\nheu: %3d\tnum: %3d\n", hcol->dcol, hcol->drow - d->rmax);
Doubly *temp = getMin(d, d->csol);
   printf("min: %3d\tnum: %3d\n", temp->dcol, temp->drow-d->rmax);
   printColHeaders(d);

   if(hcol->drow == d->rmax)
      return 1;
/* 
 * listSize = hcol->drow - d->rmax;
 * hitList = calloc(listSize, sizeof(int));
 */
   xrow = hcol->down; /*nextRow(hcol, &listSize, &hitList);*/
   for(; xrow != hcol; xrow = xrow->down)/*nextRow(hcol, &listSize, &hitList))*/
   {
      sol = initTrie(xrow->hrow);
      addMin(d->csol, d->root);
      addChild(d->csol, sol);
      d->csol = sol;
      coverRow(d, xrow);
      if(0 == (ret = algorithmX(d)))
         x = 0;
      uncoverRow(d, xrow);
      d->csol = d->csol->parent;
      if(ret == 1)
      {
         d->csol->ichild--;
         freeSol(sol);
      }
      if(x == 0 && d->mode == 2){
         /*free(hitList);*/
         return 0;
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

         Doubly *temp = getMin(d, d->csol);//d->csol->minList[d->csol->minIndex];
         if(xrow2->hcol->drow - d->rmax > 0 &&
            xrow2->hcol->drow < temp->drow)
         {
            printf("%d\n", xrow2->dcol);
            addMin(d->csol, xrow2->hcol);
         }
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

   for(xcol = hcol->down; xcol != hcol; xcol = xcol->down)
   {
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
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
