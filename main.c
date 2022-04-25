#include "main.h"
#include "auxil.h"
#include "initDance.h"
#include "sew.h"
#include "heuristic.h"
#include "free.h"
#include "error.h"
#include "hide.h"
#include "matrixFile.h"
#include "hrowCover.h"
#include "parseArgs.h"
#include "generate.h"
#include "dimacs.h"
#include "fileHandling.h"

int main(int argc, char *argv[])
{
    int userOption = parseFirstArg(argc, argv);
    srand(time(NULL));
    checkConfig();

    switch(userOption)
    {
        case DLX_SOLVE:
            run_DLXSolve(argc, argv);
            break;
        case DLX_GEN:
            run_DLXGen(argc, argv);
            break;
        case ENUMERATE:
            run_Enumerate(argc, argv);
            break;
        case ZCHAFF_GEN_0:
            run_ZchaffGen0(argc, argv);
            break;
        case ZCHAFF_GEN_1:
            run_ZchaffGen1(argc, argv);
            break;
        default: assert(0);
    }

    return 0;
}

void checkConfig()
{
    assert(USE_HEUR >= 0 && USE_HEUR <= 2);
    assert(STARTING_CAP >= 1);
    assert(GROWTH_FACTOR > 1);
    assert((int)(STARTING_CAP * GROWTH_FACTOR) > STARTING_CAP);
    assert(BUFSIZE > 100);
}

void run_DLXSolve(int argc, char *argv[])
{
    Dance *d;
    DLX *dlx = malloc(sizeof(DLX));
    parseArgs_DLXSolve(dlx, argc, argv);
    d = dlx->d;

    printSudoku(d->s);

    findMatrixFile(d);
    setMatrixDimensions_Sudoku(d, d->s);
    initDance(d);
    initMatrix(d);

    //printMatrix(d);

    HEUR_INIT(d, d->s->containerSize)
    initHide(d);
    fillAllCells(d);
    coverRowHeaders(d);

    printf("starting algX\n");
    algorithmX(d);
    printf("number of calls: %lu\n", d->numCalls);
    printf("numSols: %d\n", d->numSols);

    uncoverRowHeaders(d);
    unfillAllCells(d);

    PRINT_ALL_SUDOKU_SOLS

    saveSolution_Sudoku(d);
    writeToSudokuFile(d, dlx->solutionFile);

    free_DLXSolve(dlx);
}

void run_DLXGen(int argc, char *argv[])
{
    Dance *d;
    int res = NOT_FOUND, i;

    DLX *dlx = malloc(sizeof(DLX));
    parseArgs_DLXGen(dlx, argc, argv);
    d = dlx->d;
    
    findMatrixFile(d);
    setMatrixDimensions_Sudoku(d, d->s);
    initDance(d);
    initMatrix(d);

    //printMatrix2(d);

    HEUR_INIT(d, d->s->containerSize)

    coverRowHeaders(d);

    printf("starting algX\n");
    for(i = 1; i < THRESHOLD_TRY+1; i++)
    {
        d->numCalls = 0;
        if(i % 10 == 0)
            printf("algX try: %d\n", i);
        res = algorithmX_Gen_Rand(d);
        if(res == FOUND)
            break;
    }
    printf("algX number of tries: %d\n", i-1);
    printf("number of calls: %lu\n", d->numCalls);

    uncoverRowHeaders(d);
    if(res == NOT_FOUND)
    {
        free_DLXGen(dlx);
        return;
    }

    saveSolution_Sudoku(d);
    printf("printing sudoku solved:\n");
    printSudoku(d->s);
    writeToSudokuFile(d, dlx->solutionFile);

    initHide(d);

    setNumClues(d->s, d->s->sudokuSize * (1.0/2));
    //setNumClues(d->s, d->s->sudokuSize * (30.0/81));
    printf("starting generation\n");
    for(i = 1; i < THRESHOLD_TRY+1; i++)
    {
        d->genNumCalls = 0;
        printf("gen try: %d\n", i);
        res = generate(d);
        if(res == FOUND)
            break;
    }
    if(res == NOT_FOUND)
        printf("No puzzles found\n");
    printf("\nnumber of gen calls: %d\n", d->genNumCalls);

    printf("printing sudoku puzzle:\n");
    printSudoku(d->s);
    writeToSudokuFile(d, dlx->sudokuFile);

    unfillAllCells(d);

    freeHide(d);
    free_DLXGen(dlx);
}

void run_Enumerate(int argc, char *argv[])
{
    Enum *e = malloc(sizeof(Enum));

    parseArgs_Enumerate(e, argc, argv);

    //printSudoku(d->s);
    findMatrixFile(e->d);

    setMatrixDimensions_Sudoku(e->d, e->d->s);

    initDance(e->d);
    initMatrix(e->d);

    //printMatrix(e->d);

    HEUR_INIT(e->d, e->d->s->containerSize)
    initHide(e->d);
    fillAllCells(e->d);
    coverRowHeaders(e->d);

    printf("starting algX\n");
    algorithmX_Enumerate(e->d);
    printf("number of calls: %lu\n", e->d->numCalls);
    printf("numSols: %d\n", e->d->numSols);

    uncoverRowHeaders(e->d);
    unfillAllCells(e->d);

    writeToEnumerateFile(e->d, e->enumerateFile);

    free_Enum(e);
}

void run_ZchaffGen0(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffGen0(z, argc, argv);

    //testConvertSat(z);
    writeToDimacs(z);

    free_ZChaffGen0(z);
}

void run_ZchaffGen1(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffGen1(z, argc, argv);

    free_ZChaffGen1(z);
}