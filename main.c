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
 * dim: innermost dimension is first, outermost dimension is last
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra numbers will be ignored
 */
int runSudoku(Dance *d, int argc, char *argv[])
{

   printSudoku(d->s);

/*
   findMatrixFile(d);
   setMatrixDimensions_Sudoku(d);

   initDance(d);
   initMatrix(d);
   HEUR_INIT(d, d->s->xy)
   initHide_Sudoku(d);
   fillAllCells(d);
   coverRowHeaders(d);
   algorithmX(d);
   printf("number of calls: %d\n", d->numCalls);
   uncoverRowHeaders(d);
   unfillAllCells(d);

   printSolutions_Sudoku(d);

   saveSolution_Sudoku(d);

   freeDance(d);
*/

   fclose(d->s->boardFile);
   free(d->s->sudoku);
   free(d->s);
   free(d);
   
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
   printSudoku(d->s);

   initHide_Sudoku(d);

   /* maximum number of clues you want for this puzzle */
   d->numClues = d->s->gridSize / 2;
   if(generate(d) == NOT_FOUND)
      printf("No puzzles found\n");
   printSudoku(d->s);
   printToSudokuFile(d);

   unfillAllCells(d);
   freeDance(d);

   return 0;
}