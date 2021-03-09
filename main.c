#include "main.h"
#include "auxil.h"
#include "setup.h"
#include "sew.h"
#include "heuristic.h"
#include "free.h"
#include "error.h"
#include "hide.h"
#include "shuffle.h"
#include "setMatrixDimensions.h"
#include "hrowCover.h"
#include "initHrowLayout_Sudoku2.h"
#include "saveSolution.h"
#include "secondaryColumns.h"
#include "parseArgs.h"

int main(int argc, char *argv[])
{
   Dance *d = malloc(sizeof(Dance));
   srand(time(NULL));
   parseArgs(d, argc, argv);

   switch(d->problem)
   {
      case SUDOKU: runSudoku(d, argc, argv); break;
      case SUDOKU2: runSudoku2(d, argc, argv); break;
      case NQUEENS: runNQueens(d, argc, argv); break;
      case SGEN: runSudokuGenerate(d, argc, argv); break;
      default: break;
   }
}

/*
 * A filled sudoku board file should have dimensions on first line
 * and next xy*xy lines should be either 0 (empty) or 1 through xy
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra numbers will be ignored
 */
int runSudoku(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   printSudokuBoard(d, d->s->grid);

   /* outdated, can use matrixFileCreator.py for same results */
   //initMatrixFileSudoku(d);

   /* can set to custom matrixFile here */
   sprintf(matrixFile, "dance/ds1_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku(d);

   /* initialize dance struct */
   initDance(d);

   /* reads from d->matrixFile and creates the general matrix */
   initMatrix(d);
   printf("finished matrix\n");

   if(USE_HEUR)
   {
      /* initializes the heuristic structure */
      initHeurList(d, d->s->xy);
      printf("finished heur\n");
   }

   /* hides the necessary rows in the matrix to define the puzzle, reading from sudoku file */
   initHide_Sudoku(d);
   hideAllCells(d);
   printf("finished hide\n");
   
   /* covers row headers from rest of matrix */
   coverRowHeaders(d);
   printf("finished cover\n");

   printf("starting algX\n");
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d);

   unhideAllCells(d);

   printSolutions_Sudoku(d);

   /* translates solTree matrix rows to sudoku solution */
   saveSolution_Sudoku(d);

   freeDance(d);

   return 0;
}

/* using layouts */
int runSudoku2(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   printSudokuBoard(d, d->s->grid);
 
   sprintf(matrixFile, "dance/ds2_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku2(d);

   initDance(d);
   initMatrix(d);
   printf("finished matrix\n");

   if(USE_HEUR)
   {
      initHeurList(d, d->rmax / d->s->xy);
      printf("finished heur\n");
   }

   //printHeur(d);
   //printMatrix(d);
   initHrowLayout_Sudoku2(d);
   //printHrowLayout(d);
   printf("finished hrow layout\n");

   hide_Sudoku2(d);
   printf("finished hide\n");

   //printMatrix(d);
   
   coverRowHeaders(d); /* cover all row headers, necessary for program to work */
   printf("finished cover\n");

   printf("starting algX\n");
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d); /* handles memory allocated from coverRowHeaders */
   unhide_Sudoku2(d);

   printSolutions_Sudoku2(d);
   saveSolution_Sudoku2(d);

   freeHrowLayout(d);
   freeDance(d);

   return 0;
}

int runNQueens(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));
 
   sprintf(matrixFile, "dance/dq_%d.txt", d->nq);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_NQueens(d);

   initDance(d);
   initMatrix(d);
   printf("finished matrix\n");

   /* declare index of start point of secondary hcols */
   set_secondary_columns(d, 2 * d->nq);
   stitch_secondary(d);

   if(USE_HEUR)
   {
      initHeurList(d, d->nq);
      printf("finished heur\n");
   }

   coverRowHeaders(d);
   printf("finished cover\n");

   printf("starting algX\n");
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d);

   unstitch_secondary(d);

   printSolutions_NQueens(d);
   
   /* print distribution of queen placements on board of all solutions */
   //printDistribution_NQueens(d);

   freeDance(d);

   return 0;
}

int runSudokuGenerate(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   sprintf(matrixFile, "dance/ds1_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku(d);

   initDance(d);
   initMatrix(d);
   printf("finished matrix\n");

   if(USE_HEUR)
   {
      /* initializes the heuristic structure */
      initHeurList(d, d->s->xy);
      printf("finished heur\n");
   }
   
   coverRowHeaders(d);
   printf("finished cover\n");

   printf("starting algX\n");
   /* will stop after one solution is found */
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d);

   printSolutions_Sudoku(d);
   saveSolution_Sudoku(d);

   /* setup generating mechanic */
   initHide_Sudoku(d);
   printf("finished initHide\n");
   hideAllCells(d);

   saveGeneratedPuzzle(d);
   printSudokuBoard(d, d->s->grid);

   shuffle(d);
   printSudokuBoard(d, d->s->grid);
   unhideAllCells(d);

   freeDance(d);

   return 0;
}