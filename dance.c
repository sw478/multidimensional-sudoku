#include "dance.h"
#include "solTree.h"
#include "auxil.h"
#include "heuristic.h"

int algorithmX(Dance *d)
{
   Doubly *hcol, *crow, *root = d->root;
   int res = NOT_FOUND, ret;
   int solCreated = 0;
   SolTree *sol;

   if(root == root->right)
   {
      d->csol = initTree();
      addLeaf(d, d->csol);
      d->numSols++;
      return FOUND;
   }

   d->numCalls++;
   hcol = HEUR_HEURISTIC(d)

   if(hcol == root)
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

         ALGX_BREAK
      }
   }

   return res;
}

/* stops after finding first random solution */
int algorithmX_Gen_Rand(Dance *d)
{
   Doubly *hcol, *crow, *root = d->root;
   int ret;
   SolTree *sol;
   int irand;
   Doubly **hitList;

   if(root == root->right)
   {
      d->csol = initTree();
      addLeaf(d, d->csol);
      d->numSols++;
      return FOUND;
   }

   d->numCalls++;
   //if(d->numCalls % CALL_TRACKING_ALGX_GEN_1 == 0)
     // printf("-----algX gen 1 calls: %lu\n", d->numCalls);
   
   if(d->numCalls >= THRESHOLD_ALGX_GEN_1)
      return NOT_FOUND;
   hcol = HEUR_HEURISTIC(d)

   if(hcol == root)
      return NOT_FOUND;
   
   //printHeur(d);
   //printMatrix2(d);
   
   irand = hcol->heur->num;
   hitList = shuffledList(d, hcol, irand);

   for(irand--; irand >= 0; irand--)
   {
      crow = hitList[irand];

      selectCandidateRow(d, crow);
      ret = algorithmX_Gen_Rand(d);
      unselectCandidateRow(d, crow);

      if(d->numCalls >= THRESHOLD_ALGX_GEN_1)
         break;

      if(ret == FOUND)
      {
         d->csol->row = crow->hrow;
         sol = initTree();
         addChild(sol, d->csol);
         d->csol = sol;

         free(hitList);
         return FOUND;
      }
   }

   free(hitList);
   return NOT_FOUND;
}

/* checks if more than one solution exists */
int algorithmX_Gen_NumSol(Dance *d)
{
   Doubly *hcol, *crow, *root = d->root;
   int ret, num, iSudoku;

   if(root == root->right)
   {
      d->numSols++;
      return FOUND;
   }

   d->numCalls++;
   if(d->numCalls >= THRESHOLD_ALGX_GEN_2)
      return FOUND;

   hcol = HEUR_HEURISTIC(d)

   if(hcol == root)
      return NOT_FOUND;
   
   for(crow = hcol->down; crow != hcol; crow = crow->down)
   {
      selectCandidateRow(d, crow);
      ret = algorithmX_Gen_NumSol(d);
      unselectCandidateRow(d, crow);

      if(ret == FOUND)
      {
         if(d->numSols > 1)
            return FOUND;
         num = crow->drow % d->s->containerSize + 1;
         iSudoku = crow->drow / d->s->containerSize;
         if(d->s->sudoku[iSudoku] != num)
         {
            d->numSols = 2;
            return FOUND;
         }
      }
   }

   return NOT_FOUND;
}

/* uses the Fisher-Yates O(n) algorithm */
Doubly **shuffledList(Dance *d, Doubly *hcol, int len)
{
   int i, irand;
   Doubly **hitList = malloc(len*sizeof(Doubly));
   Doubly *doub, *temp;

   /* initialize list */
   for(i = 0, doub = hcol->down; i < len; i++, doub = doub->down)
      hitList[i] = doub;

   for(i = len - 1; i > 0; i--)
   {
      irand = rand() % (i + 1);

      /* swap doubly at indices irand and i */
      temp = hitList[irand];
      hitList[irand] = hitList[i];
      hitList[i] = temp;
   }

   return hitList;
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

   hcol->drow = d->rmax;
   HEUR_DEC(d, hcol->heur, hcol->heur->num)
   for(doub = hcol->down; doub != hcol; doub = doub->down)
   {
      //doub->hrow->dcol--;
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
      //xrow->hrow->dcol--;
      HEUR_DEC(d, xrow->hcol->heur, 1)
   }
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
   int incAmount = 0;

   hcol->right->left = hcol;
   hcol->left->right = hcol;

   for(doub = hcol->down; doub != hcol; doub = doub->down)
   {
      //doub->hrow->dcol++;
      incAmount++;
      uncoverRows(d, doub);
   }
   hcol->drow += incAmount;
   HEUR_INC(d, hcol->heur, incAmount)
}

void uncoverRows(Dance *d, Doubly *doub)
{
   Doubly *xrow;
   
   for(xrow = doub->right; xrow != doub; xrow = xrow->right)
   {
      xrow->up->down = xrow;
      xrow->down->up = xrow;
      xrow->hcol->drow++;
      //xrow->hrow->dcol++;
      HEUR_INC(d, xrow->hcol->heur, 1)
   }
}