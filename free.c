#include "free.h"
#include "solTree.h"
#include "heuristic.h"
#include "hide.h"

void free_DLXSolve(Dance *d)
{
   fclose(d->s->boardFile);
   fclose(d->s->solFile);

   free_Sudoku(d->s);
   freeHide(d);
   free_Dance(d);
}

void free_DLXGen(Dance *d)
{
   fclose(d->s->boardFile);
   fclose(d->s->solFile);

   free_Sudoku(d->s);
   free_Dance(d);
}

void free_Enum(Enum *e)
{
   fclose(e->enumerateFile);

   free_Sudoku(e->d->s);
   freeHide(e->d);
   free_Dance(e->d);
   free(e);
}

void free_ZChaffGen(ZChaff *z)
{
   free_Sudoku(z->s);

   fclose(z->dimacsInputFile);
   fclose(z->dimacsOutputFile);

   free(z);
}

void free_Sudoku(Sudoku *s)
{
   free(s->sudoku);
   free(s->dim);
   free(s);
}

void free_Dance(Dance *d)
{
   free_Matrix(d);

   free(d->sols);
   if(d->numSols > 0)
      freeTree(d->csol);

   HEUR_FREE(d)

   free(d);
}

void free_Matrix(Dance *d)
{
   Doubly *col, *temp;

   for(col = d->root->right; col != d->root;)
   {
      free_Column(col);
      temp = col;
      col = col->right;
      free(temp);
   }
   free_Column(col);
   free(d->root);
}

void free_Column(Doubly *col)
{
   Doubly *row, *temp;

   for(row = col->down; row != col;)
   {
      temp = row;
      row = row->down;
      free(temp);
   }
}

