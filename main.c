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
#include "test.h"
#include "generate.h"

int main(int argc, char *argv[])
{
   Dance *d = malloc(sizeof(Dance));
   srand(time(NULL));
   checkConfig();
   parseArgs(d, argc, argv);

   switch(d->problem)
   {
      case SUDOKU: runSudoku(d, argc, argv); break;
      case SUDOKU2: runSudoku2(d, argc, argv); break;
      case NQUEENS: runNQueens(d, argc, argv); break;
      case SGEN: runSudokuGen(d, argc, argv); break;
      default: break;
   }
}

void checkConfig()
{
   assert(USE_HEUR == 0 || USE_HEUR == 1);
   assert(STARTING_CAP >= 1);
   assert(GROWTH_FACTOR > 1);
}

/*
 * Run generator first to create a puzzle in /tests
 * 
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

   printf("size of hide: %d\n", ((int)sizeof(Hide)));
   printf("size of doubly: %d\n", ((int)sizeof(Doubly)));
   printf("size of heur: %d\n", ((int)sizeof(Heur)));
   printf("size of solTree: %d\n", ((int)sizeof(SolTree)));

   /* outdated, use matrixFileCreator.py for same results */
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
   //printf("finished matrix\n");

   HEUR_INIT(d, d->s->xy)

   /* hides the necessary rows in the matrix to define the puzzle, reading from sudoku file */
   initHide_Sudoku(d);
   fillAllCells(d);
   //printf("finished hide\n");
   
   /* covers row headers from rest of matrix */
   coverRowHeaders(d);
   //printf("finished cover\n");

   //printf("starting algX\n");
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d);

   unfillAllCells(d);

   printSolutions_Sudoku(d);

   /* translates solTree matrix rows to sudoku solution */
   saveSolution_Sudoku(d);

   //printMatrixDoublyMemory(d);

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

   HEUR_INIT(d, (d->rmax / d->s->xy))

   //printHeur(d);
   //printMatrix(d);
   initHrowLayout_Sudoku2(d);
   //printHrowLayout(d);

   hide_Sudoku2(d);

   //printMatrix(d);
   
   coverRowHeaders(d);
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);
   uncoverRowHeaders(d);

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
   //printf("finished matrix\n");

   /* declare index of start point of secondary hcols */
   set_secondary_columns(d, 2 * d->nq);
   stitch_secondary(d);

   HEUR_INIT(d, d->nq)

   coverRowHeaders(d);
   //printf("finished cover\n");

   //printf("starting algX\n");
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

int runSudokuGen(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   sprintf(matrixFile, "dance/ds1_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku(d);

   initDance(d);
   initMatrix(d);

   HEUR_INIT(d, d->s->xy)
   
   coverRowHeaders(d);

   algorithmX_SGen1(d);
   printf("number of calls: %d\n", d->numCalls);

   uncoverRowHeaders(d);

   /* setup generating mechanic */
   saveSolution_Sudoku(d);
   printSudokuBoard(d, d->s->grid);

   initHide_Sudoku(d);

   /* maximum number of clues you want for this puzzle */
   d->numClues = d->s->gridSize / 2;
   if(generate(d) == NOT_FOUND)
      printf("No puzzles found\n");
   printSudokuBoard(d, d->s->grid);
   printToSudokuFile(d);

   unfillAllCells(d);
   freeDance(d);

   return 0;
}