#include "main.h"
#include "aux.h"
#include "setup.h"
#include "sew.h"
#include "heuristic.h"
#include "end.h"
#include "error.h"

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
   Sudoku *s = malloc(sizeof(Sudoku));
   Dance *d = malloc(sizeof(Dance));
   srand(time(NULL));

   parseArgs(s, argc, argv);

   if(s->mode == 1)
      printBoard(s->grid, s->x, s->y);
   d->mode = s->mode;

   initMatrixFileSudoku(d, s->x, s->y);
   initDance(d, s->x, s->y);
   initMatrix(d);
   initHeurList(d);
//printHeur(d);
   hideRows(d, s);
   coverRowHeaders(d);
/*
   printf("cmax: %d\n", d->cmax);
   printf("ncol: %d\n", d->root->dcol);
   printf("rmax: %d\n", d->rmax);
   printf("nrow: %d\n", d->root->drow);
*/   printf("finished initializing structure\n");

   if(algorithmX(d) == 1)
      printf("no solutions\n");
   printf("numCalls: %d\n", d->numCalls);

   uncoverRowHeaders(d);
   recoverHiddenRows(d);
   printSolutions(d);
   saveSolution(d, s);

   freeHeur(d);
   free(d->sols);
   freeDance(d);

   fclose(s->in);
   free(s->grid);
   free(s);

   return 0;
}

void parseArgs(Sudoku *s, int argc, char *argv[])
{
   int i, c, test;
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
