#include "dance.h"
#include "setup.h"

int initDanceSudoku(Sudoku *s)
{
   Dance *d = malloc(sizeof(Dance));
   d->mode = s->mode;

   initMatrixFileSudoku(d, s->x, s->y);
   initDance(d, s->x, s->y);
   initMatrix(d);
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

   d->root = malloc(sizeof(Doubly));
   d->root->drow = d->rmax;
   d->root->dcol = d->cmax;
   d->root->up = d->root->down = d->root->left = d->root->right = d->root;
   d->root->hcol = d->root->hrow = d->root;

   d->hideRoot = malloc(sizeof(Doubly));
   d->hideRoot->next = d->hideRoot->prev = d->hideRoot;

   d->solRoot = initTrie(NULL);
   d->csol = d->solRoot;
   d->numSols = 0;
   d->solCap = 1;
   d->sols = malloc(sizeof(SolTrie));

   return 0;
}

/*frees all nodes in matrix and hideList*/
void freeDance(Dance *d)
{
   Doubly *col, *temp;

   for(col = d->root->right; col != d->root;)
   {
      freeColumn(col);
      temp = col;
      col = col->right;
      freeDoubly(temp);
   }
   freeColumn(col);
   free(d->root);

   freeSol(d->solRoot);
   free(d->hideRoot);
   free(d);
}

void freeColumn(Doubly *col)
{
   Doubly *row, *temp;

   for(row = col->down; row != col;)
   {
      temp = row;
      row = row->down;
      freeDoubly(temp);
   }
}

void freeDoubly(Doubly *node)
{
   node->left->right = node->right;
   node->right->left = node->left;
   node->down->up = node->up;
   node->up->down = node->down;
   free(node);
}
