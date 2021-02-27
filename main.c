#include "main.h"
#include "aux.h"
#include "setup.h"
#include "sew.h"
#include "heuristic.h"
#include "end.h"
#include "error.h"
#include "hide.h"
#include "shuffle.h"

/*
 * argument format: a.out [mode: 1 for solve, 2 for gen] [file: empty to be
 * filled with only dimensions if generating, filled if solving]
 *
 * right now generate option generates full boards, will change to have
 * empty slot to be solvable
 *
 * a filled text file should only contain positive integers, 0 denoting an
 * unknown space
 *
 * If a textfile contains less characters than required, a message will
 * display. Any extra characters will be ignored
 *
 */
int main(int argc, char *argv[])
{
   Dance *d = malloc(sizeof(Dance));
   int mode;
   d->s = malloc(sizeof(Sudoku));
   srand(time(NULL));

   parseArgs(d, argc, argv); /* gets file pointer of sudoku file*/
   mode = d->s->mode;

   if(mode == 1) /* if solving */
      printBoard(d, d->s->grid);

   initMatrixFileSudoku(d); /* writes to matrix file if it doesn't exist*/
   initDance(d); /* initialize dance struct */
   initMatrix(d); /* reads from matrix file and creates the general matrix for the given dimensions */
   initHeurList(d); /* initializes the heuristic helper structure */

   if(mode == 1)
   {
      initHide(d);
      printf("finished initHide\n");
      hideAllCells(d); /* if solving, hides the necessary rows in the matrix to define the puzzle, reading from sudoku file */
   }
   coverRowHeaders(d); /* cover all row headers, necessary for program to work */
   printf("finished initializing structure\n"); /*for larger boards everything prior takes a small but noticeable amount of time */

   if(algorithmX(d) == 1)
      printf("no solutions\n");

   printf("number of calls: %d\n", d->numCalls);
   uncoverRowHeaders(d); /* handles memory allocated from coverRowHeaders */

   if(mode == 1)
      unhideAllCells(d); /* handles memory */
   printSolutions(d);
   saveSolution(d); /* translates solTrie matrix rows to sudoku solution */

   if(mode == 2)
   {
      initHide(d);
      printf("finished initHide\n");
      hideAllCells(d);

      savePuzzle(d);
      printBoard(d, d->s->grid);

      shuffle(d);
      printBoard(d, d->s->grid);
      unhideAllCells(d);
   }

   freeDance(d);

   return 0;
}

void parseArgs(Dance *d, int argc, char *argv[])
{
   int i, c, test;
   Sudoku *s = d->s;
   char *buf = malloc(BUFSIZE*sizeof(char)), temp;

   memset(buf, 0, BUFSIZE*sizeof(char));
   if(argc != 3)
      numArgError();
   if(!strcmp(argv[1], "s")) /* solve */
      s->mode = 1;
   else if(!strcmp(argv[1], "g")) /* generate */
      s->mode = 2;
   else
      usage();
   if(s->mode == 1)
   {
      s->in = fopen(argv[2], "r+");
      if(!s->in)
         fileError(argv[2]);
   }
   if(s->mode == 2)
      s->in = fopen(argv[2], "w+");

   assert(3 == sscanf(argv[2], "tests/%c/%dx%d.in", &temp, &s->x, &s->y));
   s->xy = s->x*s->y;
   s->gridSize = s->xy*s->xy;
   s->grid = calloc(s->gridSize, sizeof(int));

   assert(fseek(s->in, 0, SEEK_SET) == 0);
   if(s->mode == 2)
   {
      sprintf(buf, "%d %d\n", s->x, s->y);
      fwrite(buf, 1, BUFSIZE*sizeof(char), s->in);
      free(buf);
      return;
   }

   fgets(buf, BUFSIZE*sizeof(char), s->in);
   for(i = 0; i < s->gridSize; i++)
   {
      fgets(buf, BUFSIZE, s->in);
      test = sscanf(buf, "%d", &c);
      if(test < 1 || c < 0 || c > s->xy)
         invalidInput();
      s->grid[i] = c;
   }
   free(buf);
}
