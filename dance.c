#include "dance.h"
#include "solTree.h"
#include "auxil.h"
#include "heuristic.h"

int algorithmX(Dance *d)
{
   Doubly *hcol, *crow;
   int res = NOT_FOUND, ret;
   int solCreated = 0;
   SolTree *sol;

   if(d->root == d->root->right)
   {
      d->csol = initTree();
      addLeaf(d, d->csol);
      return FOUND;
   }

   d->numCalls++;
   hcol = HEUR_HEURISTIC(d)

   if(hcol == d->root)
      return NOT_FOUND;
   
   for(crow = hcol->down; crow != hcol; crow = crow->down)
   {
      selectCandidateRow(d, crow);
      ret = algorithmX(d);
      if(ret == FOUND)
         res = FOUND;
      unselectCandidateRow(d, crow);

      if(ret == FOUND)
      {
         d->csol->row = crow->hrow;
         if(solCreated == 0)
         {
            solCreated = 1;
            sol = initTree();
         }
         addChild(sol, d->csol);
         d->csol = sol;
      }
   }

   return res;
}

/* stops after finding first random solution */
int algorithmX_Gen_Rand(Dance *d)
{
   Doubly *hcol, *crow;
   int res = NOT_FOUND, ret;
   int solCreated = 0;
   SolTree *sol;
   int listSize, *hitList;

   if(d->root == d->root->right)
   {
      d->csol = initTree();
      addLeaf(d, d->csol);
      return FOUND;
   }

   d->numCalls++;
   hcol = HEUR_HEURISTIC(d)

   if(hcol == d->root)
      return NOT_FOUND;
   
   listSize = hcol->drow - d->rmax;
   hitList = calloc(listSize, sizeof(int));
   crow = nextRow(hcol, &listSize, &hitList);
   
   while(crow != hcol)
   {
      selectCandidateRow(d, crow);
      ret = algorithmX_Gen_Rand(d);
      if(ret == FOUND)
         res = FOUND;
      unselectCandidateRow(d, crow);

      if(ret == FOUND)
      {
         d->csol->row = crow->hrow;
         if(solCreated == 0)
         {
            solCreated = 1;
            sol = initTree();
         }
         addChild(sol, d->csol);
         d->csol = sol;

         break;
      }
      crow = nextRow(hcol, &listSize, &hitList);
   }

   free(hitList);

   return res;
}

/* checks if more than one solution exists */
int algorithmX_Gen_NumSol(Dance *d)
{
   Doubly *hcol, *crow;
   int res = NOT_FOUND, ret;

   if(d->root == d->root->right)
   {
      d->numSols++;
      return FOUND;
   }

   hcol = HEUR_HEURISTIC(d)

   if(hcol == d->root)
      return NOT_FOUND;
   
   for(crow = hcol->down; crow != hcol; crow = crow->down)
   {
      selectCandidateRow(d, crow);
      ret = algorithmX_Gen_NumSol(d);
      if(ret == FOUND)
         res = FOUND;
      unselectCandidateRow(d, crow);

      if(ret == FOUND && d->numSols > 1)
         break;
   }

   return res;
}

/*
 * used to randomize order of candidate rows searched in column chosen
 * in AlgX, otherwise algX will just go down the column
 */
Doubly *nextRow(Doubly *hcol, int *listSize, int **hitList)
{
   Doubly *crow;
   int i, j, randInt;
   if(*listSize == 0)
      return hcol;

   randInt = rand() % *listSize;
   for(crow = hcol->down, i = 0; i < randInt; i++, crow = crow->down);
   for(j = i; (*hitList)[j] == 1; j++, crow = crow->down)
   {
      if(crow == hcol)
         crow = crow->down;
   }

   (*listSize)--;
   (*hitList)[j] = 1;
   return crow;
}

void selectCandidateRow(Dance *d, Doubly *candidateRow)
{
   Doubly *crow;

   for(crow = candidateRow->right; crow != candidateRow; crow = crow->right)
      coverColRows(d, crow);
   coverColRows(d, candidateRow);
}

void coverColRows(Dance *d, Doubly *crow)
{
   Doubly *doub, *hcol = crow->hcol;

   hcol->right->left = hcol->left;
   hcol->left->right = hcol->right;

   for(doub = hcol->down; doub != hcol; doub = doub->down)
   {
      doub->hcol->drow--;
      doub->hrow->dcol--;
      coverRows(d, doub);
   }
}

void coverRows(Dance *d, Doubly *doub)
{
   Doubly *xrow;

   for(xrow = doub->right; xrow != doub; xrow = xrow->right)
   {
      xrow->up->down = xrow->down;
      xrow->down->up = xrow->up;
      xrow->hcol->drow--;
      xrow->hrow->dcol--;

      HEUR_DEC(d, xrow->hcol->dcol, d->sec_hcol_index, xrow->hcol->heur)
   }
   HEUR_DEC(d, xrow->hcol->dcol, d->sec_hcol_index, xrow->hcol->heur)
}

void unselectCandidateRow(Dance *d, Doubly *candidateRow)
{
   Doubly *crow;

   uncoverColRows(d, candidateRow);
   for(crow = candidateRow->left; crow != candidateRow; crow = crow->left)
      uncoverColRows(d, crow);
}

void uncoverColRows(Dance *d, Doubly *crow)
{
   Doubly *doub, *hcol = crow->hcol;

   hcol->right->left = hcol;
   hcol->left->right = hcol;

   for(doub = hcol->down; doub != hcol; doub = doub->down)
   {
      doub->hcol->drow++;
      doub->hrow->dcol++;
      uncoverRows(d, doub);
   }
}

void uncoverRows(Dance *d, Doubly *doub)
{
   Doubly *xrow;
   
   for(xrow = doub->right; xrow != doub; xrow = xrow->right)
   {
      xrow->up->down = xrow;
      xrow->down->up = xrow;
      xrow->hcol->drow++;
      xrow->hrow->dcol++;
      HEUR_INC(d, xrow->hcol->dcol, d->sec_hcol_index, xrow->hcol->heur)
   }
   HEUR_INC(d, xrow->hcol->dcol, d->sec_hcol_index, xrow->hcol->heur)
}