#include "solTree.h"
#include "dance.h"

/**
 * Add end node sol to list of solutions
 */
void addLeaf(Dance *d, SolTree *sol)
{
   if(d->numSols >= d->solCap)
   {
      d->solCap = d->solCap * GROWTH_FACTOR + 1;
      d->sols = realloc(d->sols, d->solCap*sizeof(SolTree*));
   }
   d->sols[d->numSols] = sol;
}

/**
 * Initialize everything except for row,
 * set parent to itself
 */
SolTree* initTree()
{
   SolTree *sol = malloc(sizeof(SolTree));
   sol->ichild = 0;
   sol->cap = STARTING_CAP;
   sol->childList = malloc(sol->cap*sizeof(SolTree*));
   sol->parent = sol;

   return sol;
}

/**
 * Add child to parent's childList,
 * Sets child's parent to this parent
 */
void addChild(SolTree *parent, SolTree *child)
{
   if(parent->ichild >= parent->cap)
   {
      parent->cap = parent->cap * GROWTH_FACTOR + 1;
      parent->childList = realloc(parent->childList, parent->cap*sizeof(SolTree*));
   }

   parent->childList[parent->ichild] = child;
   parent->ichild++;

   child->parent = parent;
}

/**
 * Recursively frees itself and its children
 */
void freeTree(SolTree *sol)
{
   int i;

   for(i = 0; i < sol->ichild; i++)
      freeTree(sol->childList[i]);

   free(sol->childList);
   free(sol);
}

/**
 * Translates solution matrix rows to a filled sudoku board
 */
int saveSolution_Sudoku(Dance *d)
{
   SolTree *cur;
   int num, iSudoku, mrow, cSize = d->s->containerSize, *sudoku = d->s->sudoku;

   assert(d->numSols > 0);
   if(d->numSols > 1)
      printf("more than one solution found, saving first solution\n");

   for(cur = d->sols[0]; cur->parent != cur; cur = cur->parent)
   {
      mrow = cur->row->drow;
      num = mrow % cSize;
      iSudoku = mrow / cSize;
      sudoku[iSudoku] = num + 1;
   }

   return 0;
}