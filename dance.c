#include "dance.h"
#include "solTrie.h"
#include "auxil.h"
#include "heuristic.h"

/*
 * returns 0 if a solution was found (not leaf)
 * 1 if no solution found
 * 2 if solution was found and is a leaf
 */
int algorithmX(Dance *d)
{
   Doubly *hcol, *xrow;
   int x = 1, ret;
   //int listSize, *hitList;
   SolTrie *sol;

   if(d->root == d->root->right)
   {
      addLeaf(d);
      return 0;
   }
   d->numCalls++;

   hcol = heuristic(d);
   //hcol = heuristic2(d);
   if(hcol == d->root)
      return 1;
   
//printHeur(d);
 
   //listSize = hcol->drow - d->rmax;
   //hitList = calloc(listSize, sizeof(int));

   //xrow = nextRow(hcol, &listSize, &hitList);
   xrow = hcol->down;
   for(; xrow != hcol; xrow = xrow->down)
//nextRow(hcol, &listSize, &hitList))
   {
      sol = initTrie(xrow->hrow);
      addChild(d->csol, sol);
      d->csol = sol;
//printMatrix(d);
      coverRow(d, xrow);
      ret = algorithmX(d);
      if(ret != 1)
         x = 0;
//printMatrix(d);
      uncoverRow(d, xrow);

      d->csol = d->csol->parent;
      if(x == 1)
      {
         d->csol->ichild--;
         freeSol(sol);
      }
      if(x == 0 && d->s->mode == 2)
      {
         /* if you want to stop at the first solution found,
            have it break here */
         break;
      }
   }

   //free(hitList);
   return x;
}

/*
 * used to randomize board
 * keeps track of rows and records which have already been visited
 */
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
      xcol->hcol->drow--;
      xcol->hrow->dcol--;
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
      {
         xrow2->up->down = xrow2->down;
         xrow2->down->up = xrow2->up;
         xrow2->hcol->drow--;
         xrow2->hrow->dcol--;
         decHeur(d, xrow2->hcol->heur, 1);
      }
      decHeur(d, xcol->hcol->heur, 1);
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
      xcol->hcol->drow++;
      xcol->hrow->dcol++;
      for(xrow2 = xcol->right; xrow2 != xcol; xrow2 = xrow2->right)
      {
         xrow2->up->down = xrow2;
         xrow2->down->up = xrow2;
         xrow2->hcol->drow++;
         xrow2->hrow->dcol++;
         incHeur(d, xrow2->hcol->heur, 1);
      }
      incHeur(d, xcol->hcol->heur, 1);
   }

   return 0;
}

/* using heur */
Doubly *heuristic(Dance *d)
{
   Heur *heurHeader;

   if(d->heurRoot->hnext == d->heurRoot)
      return d->root;

   // find the first non-empty heur header
   for(heurHeader = d->heurRoot->hnext; heurHeader != d->heurRoot; heurHeader = heurHeader->hnext)
   {
      if(heurHeader->next != heurHeader)
         return heurHeader->next->hcol;
   }

   // signifying none has been found
   return d->root;
}

/* original setup, O(cmax) time */
Doubly *heuristic2(Dance *d)
{
   Doubly *hcol, *minXs;

   for(hcol = minXs = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      if(hcol->drow < minXs->drow)
         minXs = hcol;
   }

   return minXs;
}