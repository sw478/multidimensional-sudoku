#include "error.h"

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
   fprintf(stderr,
      "Usage: ./a.out [mode: s to solve or g to generate] [sudoku file]\n");
   exit(EXIT_FAILURE);
}

void numArgError()
{
   fprintf(stderr, "Num Arg Error\n");
   exit(EXIT_FAILURE);
}

void heurNumError()
{
   fprintf(stderr, "Heur Num Error\n");
   exit(EXIT_FAILURE);
}

void invalidN()
{
   fprintf(stderr, "Invalid value for n\n");
   exit(EXIT_FAILURE);
}

void error()
{
   perror(NULL);
   exit(EXIT_FAILURE);
}
