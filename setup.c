#include "dance.h"
#include "setup.h"

int initDanceSudoku(Sudoku *s)
{
   Dance *d = malloc(sizeof(Dance));
   d->mode = s->mode;

   initMatrixFileSudoku(d, s->x, s->y);
   initDance(d, s->x, s->y);
   initMatrix(d);
   printf("finished initializing structure\n");
   hideRows(d, s);

   if(algorithmX(d))
      printf("no solutions\n");

   recoverHiddenRows(d);
   printSolutions(d);
   saveSolution(d, s);

   free(d->sols);
   freeDance(d);
   return 0;
}

int initDance(Dance *d, int x, int y)
{
   assert(d != NULL);

   d->x = x;
   d->y = y;
   d->ilist = 0;
   d->initListCap = 1;
   d->initList = malloc(sizeof(Doubly*));

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->xcol = d->xrow = d->root->hcol = d->root->hrow = d->root;

   d->hideRoot = malloc(sizeof(Doubly));
   d->hideRoot->next = d->hideRoot->prev = d->hideRoot;

   d->solRoot = initTrie(NULL);
   d->csol = d->solRoot->parent = d->solRoot;
   d->numSols = 0;
   d->solCap = 1;
   d->sols = malloc(sizeof(SolTrie));

   return 0;
}
