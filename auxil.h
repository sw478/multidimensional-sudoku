#ifndef AUX_H
#define AUX_H
#include "dance.h"
#include "solTree.h"

void printMatrix(Dance *d);
void printSingleSol_Matrix(Dance *d, SolTree *sol);

void printSolutions_Sudoku(Dance *d);
void printSingleSol_Sudoku(Dance *d, SolTree *sol);
void printSudokuBoard_Gen(Dance *d);
void printSudoku(Sudoku *s);
void printSudoku_1D(Sudoku *s);
void printSudoku_2D(Sudoku *s, int iStart);
void printSudoku_ND(Sudoku *s, int idim, int printSize, int iStart);

void printHeur(Dance *d);

void checkMatrix(Dance *d);
void checkDoubly(Doubly *doub);

#endif