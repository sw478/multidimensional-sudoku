#include "dance.h"
#include "setup.h"

int initDanceSudoku(Sudoku *s)
{
   Dance *d = malloc(sizeof(Dance));

   initMatrixFileSudoku(d, s->x, s->y);
   initMatrix(d);
   initSudokuMatrix(d, s);

   if(algorithmX(d))
      printf("no solutions\n");

   recoverHiddenRows(d);
   //printSolutions(d);
   saveSolution(d, s);

   free(d->sols);
   freeDance(d);
   return 0;
}

int initDance(Dance *d)
{
   assert(d != NULL);

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

int initDoubly(Dance *d, int irow, int icol)
{
   Doubly *hrow, *hcol, *xhrow, *xhcol, *temp, *new;           
   assert(d != NULL);
   assert(d->root != NULL);
   assert(irow < d->rmax && icol < d->cmax);

   hcol = d->root->right;
   for(; hcol != d->root && hcol->dcol < icol; hcol = hcol->right);
   if(hcol->dcol != icol)
   {
      temp = malloc(sizeof(Doubly));
      temp->dcol = icol;
      temp->drow = d->rmax;
      temp->up = temp->down = temp;
      temp->left = hcol->left;
      temp->right = hcol;
      hcol->left->right = temp;
      hcol->left = temp;
      hcol = temp;
      hcol->hcol = hcol;
   }
   xhcol = hcol;

   hcol = hcol->down;
   for(; hcol->drow != d->rmax && hcol->drow < irow; hcol = hcol->down);
   if(hcol->drow == irow)
      return 1;

   hrow = d->root->down;
   for(; hrow != d->root && hrow->drow < irow; hrow = hrow->down);
   if(hrow->drow != irow)
   {
      temp = malloc(sizeof(Doubly));
      temp->dcol = d->cmax;
      temp->drow = irow;
      temp->left = temp->right = temp;
      temp->down = hrow;
      temp->up = hrow->up;
      hrow->up->down= temp;
      hrow->up = temp;
      hrow = temp;
      hrow->hcol = d->root;
   }
   xhrow = hrow;

   hrow = hrow->right;
   for(; hrow->dcol != d->cmax && hrow->dcol < icol; hrow = hrow->right);
   if(hrow->dcol == icol)
      return 2;

   new = malloc(sizeof(Doubly));
   new->dcol = icol;
   new->drow = irow;

   new->right = hrow;
   new->left = hrow->left;
   new->down = hcol;
   new->up = hcol->up;
   new->hcol = xhcol;
   new->hrow = xhrow;
   new->hcol->drow++;

   hrow->left->right = new;
   hrow->left = new;
   hcol->up->down = new;
   hcol->up = new;

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
