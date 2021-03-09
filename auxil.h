#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTree.h"

void printMatrix(Dance *d);
void printSingleSol_Matrix(Dance *d, SolTree *sol);

void printSolutions_Sudoku(Dance *d);
void printSingleSol_Sudoku(Dance *d, SolTree *sol);
void printSudokuBoard(Dance *d, int *grid);
void printSudokuBoard_Gen(Dance *d);

void printSolutions_Sudoku2(Dance *d);
void printSingleSol_Sudoku2(Dance *d, SolTree *sol);

void printSolutions_NQueens(Dance *d);
void printSingleSol_NQueens(Dance *d, SolTree *sol);
void printBoard_NQueens(Dance *d, int *board);
void printDistribution_NQueens(Dance *d);

void printHeur(Dance *d);

void checkMatrix(Dance *d);
void checkDoubly(Doubly *doub);

#endif