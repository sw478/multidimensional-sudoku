#ifndef PARSE_H
#define PARSE_H
#include "struct.h"

int parseFirstArg(int argc, char *argv[]);
void parseArgs_readDims(Sudoku *s, int argc, char *argv[], int argOffset);
void parseArgs_getFile(FILE **f, char *filename, int rw);

void parseArgs_DLXSolve(DLX *dlx, int argc, char *argv[]);
void parseArgs_DLXGenFull(DLX *dlx, int argc, char *argv[]);
void parseArgs_DLXGenPartial(DLX *dlx, int argc, char *argv[]);
void parseArgs_Enumerate(Enum *e, int argc, char *argv[]);
void parseArgs_ZChaffSolve0(ZChaff *z, int argc, char *argv[]);
void parseArgs_ZChaffSolve1(ZChaff *z, int argc, char *argv[]);
void parseArgs_ZChaffGen0(ZChaff *z, int argc, char *argv[]);
void parseArgs_ZChaffGen1(ZChaff *z, int argc, char *argv[]);

#define READ 0
#define WRITE 1

#endif