#include "free.h"
#include "solTree.h"
#include "heuristic.h"
#include "hide.h"

void free_DLXSolve(DLX *dlx)
{
   fclose(dlx->sudokuFile);
   fclose(dlx->solutionFile);

   free_Sudoku(dlx->d->s);
   free_Hide(dlx->d);
   if(dlx->d->numSols > 0)
      freeTree(dlx->d->csol);
   free_Dance(dlx->d);
   free(dlx);
}

void free_DLXGenFull(DLX *dlx)
{
   fclose(dlx->solutionFile);

   free_Sudoku(dlx->d->s);
   if(dlx->d->numSols > 0)
      freeTree(dlx->d->csol);
   free_Dance(dlx->d);
   free(dlx);
}

void free_DLXGenPartial(DLX *dlx)
{
   fclose(dlx->sudokuFile);
   fclose(dlx->solutionFile);

   free_Sudoku(dlx->d->s);
   free_Hide(dlx->d);
   free_Dance(dlx->d);
   free(dlx);
}

void free_Enum(Enum *e)
{
   fclose(e->enumerateFile);

   free_Sudoku(e->d->s);
   free_Hide(e->d);
   if(e->d->numSols > 0)
      freeTree(e->d->csol);
   free_Dance(e->d);
   free(e);
}

void free_ZChaffSolve0(ZChaff *z)
{
   fclose(z->sudokuFile);
   fclose(z->dimacsInputFile);
   
   free_Sudoku(z->s);
   free(z);
}

void free_ZChaffSolve1(ZChaff *z)
{
   fclose(z->sudokuFile);
   fclose(z->solutionFile);
   fclose(z->dimacsOutputFile);
   
   free_Sudoku(z->s);
   free(z);
}

void free_ZChaffGen0(ZChaff *z)
{
   fclose(z->dimacsInputFile);

   free_Sudoku(z->s);
   free(z);
}

void free_ZChaffGen1(ZChaff *z)
{
   fclose(z->dimacsOutputFile);
   fclose(z->solutionFile);

   free_Sudoku(z->s);
   free(z);
}

void free_Hide(Dance *d)
{
   int ihide;

   for(ihide = 0; ihide < d->ihide; ihide++)
   {
      free(d->hideList[ihide]->hrows);
      free(d->hideList[ihide]);
   }

   free(d->hideList);
   free(d->hideRoot);
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

