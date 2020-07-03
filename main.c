#include "sudoku.h"
#include "dance.h"
#include "setup.h"

/*
 * argument format: a.out [mode: 1 for gen, 2 for solve] [file: empty to be
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
   FILE *in;
   srand(time(NULL));

   checkArgs(s, argc, argv, &in);
   readIn(s, in);

   if(s->mode == 1)
      printBoard(s);
   initDanceSudoku(s);
   printBoard(s);

   fclose(in);
   free(s->grid);
   free(s);

   return 0;
}

//for obvious debugging purposes
void printBoard(Sudoku *s)
{
   int row, col;

   printf("\n");
   for(row = 0; row < s->xy; row++)
   {
      if(row % s->y == 0)
         printf("\n");
      for(col = 0; col < s->xy; col++)
      {
         if(col % s->x == 0)
            printf(" ");
         if(s->grid[row*s->xy + col] != 0)
            printf(" %2d", s->grid[row*s->xy + col]);
         else
            printf(" __");
      }
      printf("\n");
   }
}

void readIn(Sudoku *s, FILE *in)
{
   int x, y, length, gridSize;
   int i, c, test;
   char buf[BUFSIZE];

   fgets(buf, sizeof(buf), in);
   test = sscanf(buf, "%d %d", &s->y, &s->x);
   if(test != 2)
      invalidInput();
   x = s->x;
   y = s->y;
   s->xy = length = x*y;
   s->gridSize = gridSize = length*length;
   s->grid = calloc(gridSize, sizeof(int));

   if(s->mode == 2)
      return;
   for(i = 0; i < gridSize; i++)
   {
      fgets(buf, BUFSIZE, in);
      test = sscanf(buf, "%d", &c);
      if(test < 1)
         invalidInput();
      if(c < 0 || c > length)
         invalidInput();
      s->grid[i] = c;
   }
}

void checkArgs(Sudoku *s, int argc, char *argv[], FILE **in)
{
   if(argc != 3)
      numArgError();
   if(!strcmp(argv[1], "1")) // solve
      s->mode = 1;
   else if(!strcmp(argv[1], "2")) // generate
      s->mode = 2;
   else
      usage();
   *in = fopen(argv[2], "r");
   if(!*in)
      fileError(argv[2]);
}

void invalidInput()
{
   fprintf(stderr, "Input file contains invalid game board\n");
   exit(EXIT_FAILURE);
}

void fileError(char *fileName)
{
   fprintf(stderr, "FileError: ");
   perror(fileName);
   exit(EXIT_FAILURE);
}

void usage()
{
   fprintf(stderr, "Usage: ./a.out [mode: 1 to solve or 2 to generate] [sudoku file]\n");
   exit(EXIT_FAILURE);
}

void numArgError()
{
   fprintf(stderr, "Num Arg Error");
   exit(EXIT_FAILURE);
}

void error()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}
