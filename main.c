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
      default: break;
   }
}

/*
 * right now generate option generates full boards, will change to have
 * empty slot to be solvable
 *
 * a filled text file should only contain positive integers, 0 denoting an
 * unknown space
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra characters will be ignored
 */
int runSudoku(Dance *d, int argc, char *argv[])
{
   int mode;
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   mode = d->s->mode;

   if(mode == 1) /* if solving */
      printSudokuBoard(d, d->s->grid);

   //initMatrixFileSudoku(d); /* using matrixFileCreator.py probably easier */
   sprintf(matrixFile, "dance/ds1_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku(d);

   /* initialize dance struct */
   initDance(d);
   initMatrix(d); /* reads from d->matrixFile and creates the general matrix */
   printf("finished matrix\n"); /* for larger boards everything prior takes a small but noticeable amount of time */

   /* initializes the heuristic structure */
   initHeurList(d, d->s->xy);
   printf("finished heur\n");

   if(mode == 1)
   {
      initHide_Sudoku(d);
      hideAllCells(d); /* if solving, hides the necessary rows in the matrix to define the puzzle, reading from sudoku file */
   }
   printf("finished hide\n");
   
   coverRowHeaders(d); /* cover all row headers, necessary for program to work */
   printf("finished cover\n");

   /* if mode == 2, will stop after one solution is found */
   algorithmX(d);

   printf("number of calls: %d\n", d->numCalls);
   uncoverRowHeaders(d); /* handles memory allocated from coverRowHeaders */

   if(mode == 1)
      unhideAllCells(d); /* handles memory */
   printSolutions_Sudoku(d);
   saveSolution_Sudoku(d); /* translates solTree matrix rows to sudoku solution */

   if(mode == 2)
   {
      initHide_Sudoku(d);
      printf("finished initHide\n");
      hideAllCells(d);

      saveGeneratedPuzzle(d);
      printSudokuBoard(d, d->s->grid);

      shuffle(d);
      printSudokuBoard(d, d->s->grid);
      unhideAllCells(d);
   }

   freeDance(d);

   return 0;
}

/* using layouts */
int runSudoku2(Dance *d, int argc, char *argv[])
{
   char *matrixFile = malloc(BUFSIZE*sizeof(char));

   if(d->s->mode == 2)
   {
      fprintf(stderr, "Can't generate puzzles with layout setup\n");
      error();
   }

   printSudokuBoard(d, d->s->grid);
 
   /* can set to custom matrixFile here */
   sprintf(matrixFile, "dance/ds2_%dx%d.txt", d->s->y, d->s->x);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_Sudoku2(d);

   initDance(d); /* initialize dance struct */
   initMatrix(d); /* reads from d->matrixFile and creates the general matrix for the given dimensions */
   printf("finished matrix\n"); /*for larger boards everything prior takes a small but noticeable amount of time */

   initHeurList(d, d->rmax / d->s->xy); /* initializes the heuristic helper structure */
   printf("finished heur\n");
   
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
 
   /* can set to custom matrixFile here */
   sprintf(matrixFile, "dance/dq_%d.txt", d->nq);
   d->matrixFile = fopen(matrixFile, "r+");
   free(matrixFile);

   setMatrixDimensions_NQueens(d);

   initDance(d); /* initialize dance struct */
   initMatrix(d); /* reads from d->matrixFile and creates the general matrix for the given dimensions */
   printf("finished matrix\n"); /*for larger boards everything prior takes a small but noticeable amount of time */

   /* declare index of start point of secondary hcols */
   set_secondary_columns(d, 2 * d->nq);
   stitch_secondary(d);
   //printMatrix(d);

   if(USE_HEUR)
   {
      initHeurList(d, d->nq); /* initializes the heuristic helper structure */
      printf("finished heur\n");
   }

   coverRowHeaders(d); /* cover all row headers, necessary for program to work */
   printf("finished cover\n");

   algorithmX(d);

   printf("number of calls: %d\n", d->numCalls);
   uncoverRowHeaders(d); /* handles memory allocated from coverRowHeaders */

   unstitch_secondary(d);

   printSolutions_NQueens(d);
   
   /* print distribution of queen placements on board of all solutions */
   //printDistribution_NQueens(d);

   freeDance(d);

   return 0;
}