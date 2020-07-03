#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

#define BUFSIZE 100

typedef struct
{
   int *grid, x, y, xy, z, gridSize;
   int elements, steps, fit, visited, mode;
} Sudoku;

void printBoard(Sudoku *s);
void readIn(Sudoku *s, FILE *in);
void checkArgs(Sudoku *s, int argc, char *argv[], FILE **in);

void invalidInput();
void fileError(char *fileName);
void usage();
void error();
void numArgError();

#endif
