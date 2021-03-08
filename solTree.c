#include "solTree.h"
#include "dance.h"

/* add end node sol to list of solutions */
void addLeaf(Dance *d, SolTree *sol)
{
   if(d->numSols >= d->solCap)
   {
      d->solCap *= 2;
      d->sols = realloc(d->sols, d->solCap*sizeof(SolTree));
   }
   d->sols[d->numSols] = sol;
   d->numSols++;
}

/*
   initialize everything except for row,
   set parent to itself
*/
SolTree* initTree()
{
   SolTree *sol = malloc(sizeof(SolTree));
   sol->ichild = 0;
   sol->cap = STARTING_CAP;
   sol->childList = malloc(sol->cap*sizeof(SolTree));
   sol->parent = sol;

   return sol;
}

/*
   add child to parent's childList and sets child's parent
   to this parent
*/
void addChild(SolTree *parent, SolTree *child)
{
   if(parent->ichild >= parent->cap)
   {
      parent->cap *= 2;
      parent->childList = realloc(parent->childList, parent->cap*sizeof(SolTree));
   }

   parent->childList[parent->ichild] = child;
   parent->ichild++;

   child->parent = parent;
}

void freeTree(SolTree *sol)
{
   int i;

   for(i = 0; i < sol->ichild; i++)
      freeTree(sol->childList[i]);

   free(sol->childList);
   free(sol);
}
