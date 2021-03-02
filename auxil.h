#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTrie.h"

void printBoard(Dance *d, int *grid);
void printMatrix(Dance *d);
void printColHeaders(Dance *d);
void printSolutions_Sudoku(Dance *d);
void printSolutions_Sudoku2(Dance *d);
void printSingleSol_Matrix(Dance *d, SolTrie *sol);
void printSingleSol_Sudoku(Dance *d, SolTrie *sol);
void printSingleSol_Sudoku2(Dance *d, SolTrie *sol);
void printHeur(Dance *d);
int saveSolution_Sudoku(Dance *d);
int saveSolution_Sudoku2(Dance *d);
void checkMatrix(Dance *d);
void checkDoubly(Doubly *doub);

#endif