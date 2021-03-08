#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTree.h"

void printBoard(Dance *d, int *grid);
void printMatrix(Dance *d);
void printColHeaders(Dance *d);
void printSolutions_Sudoku(Dance *d);
void printSolutions_Sudoku2(Dance *d);
void printSolutions_NQueens(Dance *d);
void printSingleSol_Matrix(Dance *d, SolTree *sol);
void printSingleSol_Sudoku(Dance *d, SolTree *sol);
void printSingleSol_Sudoku2(Dance *d, SolTree *sol);
void printSingleSol_NQueens(Dance *d, SolTree *sol);
void printBoard_NQueens(Dance *d, int *board);
void printHeur(Dance *d);
int saveSolution_Sudoku(Dance *d);
int saveSolution_Sudoku2(Dance *d);
void checkMatrix(Dance *d);
void checkDoubly(Doubly *doub);

#endif