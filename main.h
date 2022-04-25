#ifndef SUDOKU_H
#define SUDOKU_H
#include "struct.h"

void checkConfig();
void run_DLXSolve(int argc, char *argv[]);
void run_DLXGen(int argc, char *argv[]);
void run_ZchaffSolve0(int argc, char *argv[]);
void run_ZchaffSolve1(int argc, char *argv[]);
void run_ZchaffGen0(int argc, char *argv[]);
void run_ZchaffGen1(int argc, char *argv[]);
void run_Enumerate(int argc, char *argv[]);

#endif
