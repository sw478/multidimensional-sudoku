#include "error.h"

void invalidSudokuBoard()
{
   fprintf(stderr, "Error: Input file contains invalid sudoku board\n");
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
   fprintf(stderr,
      "Usage: ./a.out [mode: s to solve or g to generate] [sudoku file]\n");
   exit(EXIT_FAILURE);
}

void numArgError()
{
   fprintf(stderr, "Error: Invalid number of arguments\n");
   exit(EXIT_FAILURE);
}

void heurNumError()
{
   fprintf(stderr, "Error: Heur num does not match hcol drow\n");
   exit(EXIT_FAILURE);
}

void invalidN_NQueens()
{
   fprintf(stderr, "Error: Invalid value for n\n");
   exit(EXIT_FAILURE);
}

void error()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}
