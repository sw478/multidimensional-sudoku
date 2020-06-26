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
   int elements, steps, fit, visited;
} Sudoku;

typedef struct
{
   Sudoku **gen, *org, *child;
   int *best, size, saved, numGens;
} Genetic;

int initDance(Sudoku *s);

int dfs(Sudoku *s);

void run(Sudoku *org, int (*solve)(Sudoku *s));

void init(Genetic *g, Sudoku *s, int size, int seed, double factor);
void end(Genetic *g);
int genetic(Sudoku *s, int size, int seed, double factor);
void generateRandom(Sudoku *org, Sudoku *rand);
int score(Sudoku *org, Sudoku *fit);
void createChild(Genetic *g, int p1, int p2);
int iterate(Genetic *g);
int nextGen(Genetic *g, int i);
void findBest(Genetic *g);

int place(Sudoku *s, int row, int col, int value);
void printBoard(Sudoku *s);
void readIn(Sudoku *s, FILE *in);
int checkBoard(Sudoku *s, int row, int col, int value);
int checkRow(Sudoku *s, int row, int col, int value);
int checkCol(Sudoku *s, int row, int col, int value);
int checkBox(Sudoku *s, int row, int col, int value);

void checkArgs(Sudoku *s, int argc, char *argv[], FILE **in);
void invalidInput();
void fileError(char *fileName);
void usage();
void error();

#endif
