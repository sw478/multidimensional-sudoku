#include "main.h"
#include "auxil.h"
#include "initDance.h"
#include "sew.h"
#include "heuristic.h"
#include "free.h"
#include "error.h"
#include "hide.h"
#include "matrixFile.h"
#include "hrowCover.h"
#include "parseArgs.h"
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
      default: assert(0);
   }
}

void checkConfig()
{
   assert(USE_HEUR >= 0 && USE_HEUR <= 2);
   assert(STARTING_CAP >= 1);
   assert(GROWTH_FACTOR > 1);
   assert(BUFSIZE > 100);
}

/*
 * dim: innermost dimension is first, outermost dimension is last
 */
int runSudoku(Dance *d, int argc, char *argv[])
{
   printSudoku(d->s);
   findMatrixFile(d);

   setMatrixDimensions_Sudoku(d);

   initDance(d);
   initMatrix(d);

   //printMatrix(d);

   HEUR_INIT(d, d->s->containerSize)
   initHide_Sudoku(d);
   fillAllCells(d);
   coverRowHeaders(d);

   printf("starting algX\n");
   algorithmX(d);
   printf("number of calls: %lu\n", d->numCalls);
   printf("numSols: %d\n", d->numSols);
   
   uncoverRowHeaders(d);
   unfillAllCells(d);

   PRINT_ALL_SUDOKU_SOLS

   saveSolution_Sudoku(d);

   freeDance(d);
   
   return 0;
}

int runSudokuGen(Dance *d, int argc, char *argv[])
{
   int res, i;
   findMatrixFile(d);
   setMatrixDimensions_Sudoku(d);

   initDance(d);
   initMatrix(d);

   //printMatrix2(d);

   HEUR_INIT(d, d->s->containerSize)
   
   coverRowHeaders(d);

   printf("starting algX\n");
   for(i = 0; i < THRESHOLD_TRY; i++)
   {
      d->numCalls = 0;
      if(i % 10 == 0)
         printf("algX try: %d\n", i);
      res = algorithmX_Gen_Rand(d);
      if(res == FOUND)
         break;
   }
   printf("number of calls: %lu\n", d->numCalls);

   uncoverRowHeaders(d);
   if(res == NOT_FOUND)
   {
      d->hideList = malloc(0);
      d->hideRoot = malloc(0);
      freeDance(d);
   }
   
   saveSolution_Sudoku(d);
   printf("printing sudoku solved:\n");
   printSudoku(d->s);

   initHide_Sudoku(d);
   
   setMaxNumClues(d->s, d->s->sudokuSize * (1.0/2));
   printf("starting generation\n");
   for(i = 0; i < THRESHOLD_TRY; i++)
   {
      d->genNumCalls = 0;
      printf("gen try: %d\n", i);
      res = generate(d);
      if(res == FOUND)
         break;
   }
   if(res == NOT_FOUND)
      printf("No puzzles found\n");
   printf("\nnumber of gen calls: %d\n", d->genNumCalls);
   
   printf("printing sudoku puzzle:\n");
   printSudoku(d->s);
   printToSudokuFile(d);

   unfillAllCells(d);
   freeDance(d);

   return 0;
}