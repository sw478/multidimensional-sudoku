#include "parseArgs.h"
#include "error.h"
#include "fileHandling.h"

/**
 * @file
 * Parses command line arguments.
 * Argument format can be found in project_reqs.md
 * argOffset is the arg index of the first dim.
 */

/**
 * First arg indicates user option
 */
int parseFirstArg(int argc, char *argv[])
{
    if(argc < 1)
        error_numArg();

    if(!strcmp(argv[1], "s"))
        return DLX_SOLVE;
    else if(!strcmp(argv[1], "gf"))
        return DLX_GEN_FULL;
    else if(!strcmp(argv[1], "gp"))
        return DLX_GEN_PARTIAL;
    else if(!strcmp(argv[1], "e"))
        return ENUMERATE;
    else if(!strcmp(argv[1], "zs0"))
        return ZCHAFF_SOLVE_0;
    else if(!strcmp(argv[1], "zs1"))
        return ZCHAFF_SOLVE_1;
    else if(!strcmp(argv[1], "zg0"))
        return ZCHAFF_GEN_0;
    else if(!strcmp(argv[1], "zg1"))
        return ZCHAFF_GEN_1;
    else
        return -1;
}

/**
 * Parses input dims
 * 
 * The dims should always be at the end of the argument
 * list since they are of variable length
 */
void parseArgs_readDims(Sudoku *s, int argc, char *argv[], int argOffset)
{
    int idim;
    s->n = argc - argOffset;

    s->dim = malloc(s->n*sizeof(int));
    s->containerSize = 1;
    
    for(idim = 0; idim < s->n; idim++)
    {
        assert(1 == sscanf(argv[idim+argOffset], "%d", &s->dim[idim]));
        s->containerSize *= s->dim[idim];
    }
    
    s->sudokuSize = round(pow(s->containerSize, s->n));
    s->sudoku = calloc(s->sudokuSize, sizeof(int));
}

/**
 * Gets file handle
 */
void parseArgs_getFile(FILE **f, char *filename, int rw)
{
    if(rw == READ)
        *f = fopen(filename, "r+");
    else if(rw == WRITE)
        *f = fopen(filename, "w+");
    
    if(!*f)
        error_file(filename);

    //assert(fseek(*f, 0, SEEK_SET) == 0);
}

void parseArgs_DLXSolve(DLX *dlx, int argc, char *argv[])
{
    Dance *d = malloc(sizeof(Dance));
    Sudoku *s = malloc(sizeof(Sudoku));
    dlx->d = d;
    dlx->s = s;
    d->s = s;

    if(argc != 4)
        error_numArg();
    
    parseArgs_getFile(&dlx->sudokuFile, argv[2], READ);
    parseArgs_getFile(&dlx->solutionFile, argv[3], WRITE);
    readInSudokuFile(s, dlx->sudokuFile);
}

void parseArgs_DLXGenFull(DLX *dlx, int argc, char *argv[])
{
    const int argOffset = 3;
    Dance *d = malloc(sizeof(Dance));
    Sudoku *s = malloc(sizeof(Sudoku));
    dlx->d = d;
    dlx->s = s;
    d->s = s;

    if(argc < argOffset + 1)
        error_numArg();

    parseArgs_getFile(&dlx->solutionFile, argv[2], WRITE);
    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_DLXGenPartial(DLX *dlx, int argc, char *argv[])
{
    Dance *d = malloc(sizeof(Dance));
    Sudoku *s = malloc(sizeof(Sudoku));
    dlx->d = d;
    dlx->s = s;
    d->s = s;

    if(argc != 4)
        error_numArg();

    parseArgs_getFile(&dlx->solutionFile, argv[2], READ);
    parseArgs_getFile(&dlx->sudokuFile, argv[3], WRITE);
    readInSudokuFile(s, dlx->solutionFile);
}

void parseArgs_Enumerate(Enum *e, int argc, char *argv[])
{
    const int argOffset = 3;
    Sudoku *s = malloc(sizeof(Sudoku));
    Dance *d = malloc(sizeof(Dance));
    e->s = s;
    e->d = d;
    d->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    parseArgs_getFile(&e->enumerateFile, argv[2], WRITE);
    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_ZChaffSolve0(ZChaff *z, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc != 4)
        error_numArg();

    parseArgs_getFile(&z->dimacsInputFile, argv[2], WRITE);
    parseArgs_getFile(&z->sudokuFile, argv[3], READ);
    readInSudokuFile(s, z->sudokuFile);
}

void parseArgs_ZChaffSolve1(ZChaff *z, int argc, char *argv[])
{
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc != 5)
        error_numArg();

    parseArgs_getFile(&z->dimacsOutputFile, argv[2], READ);
    parseArgs_getFile(&z->sudokuFile, argv[3], READ);
    parseArgs_getFile(&z->solutionFile, argv[4], WRITE);
    readInSudokuFile(s, z->sudokuFile);
}

void parseArgs_ZChaffGen0(ZChaff *z, int argc, char *argv[])
{
    const int argOffset = 3;
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    parseArgs_getFile(&z->dimacsInputFile, argv[2], WRITE);
    parseArgs_readDims(s, argc, argv, argOffset);
}

void parseArgs_ZChaffGen1(ZChaff *z, int argc, char *argv[])
{
    const int argOffset = 5;
    Sudoku *s = malloc(sizeof(Sudoku));
    z->s = s;
    
    if(argc < argOffset + 1)
        error_numArg();

    parseArgs_getFile(&z->dimacsOutputFile, argv[2], READ);
    parseArgs_getFile(&z->solutionFile, argv[3], WRITE);
    parseArgs_readDims(s, argc, argv, argOffset);
}