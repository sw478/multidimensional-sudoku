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
        case DLX_GEN_FULL:
            run_DLXGenFull(argc, argv);
            break;
        case DLX_GEN_PARTIAL:
            run_DLXGenPartial(argc, argv);
            break;
        case ENUMERATE:
            run_Enumerate(argc, argv);
            break;
        case ZCHAFF_SOLVE_0:
            run_ZchaffSolve0(argc, argv);
            break;
        case ZCHAFF_SOLVE_1:
            run_ZchaffSolve1(argc, argv);
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
    setMatrixDimensions(d, d->s);
    initDance(d);
    initMatrix(d);

    //printMatrix(d);

    HEUR_INIT(d, d->s->containerSize)
    initHide(d);
    hideAllCells(d);
    coverRowHeaders(d);

    printf("starting algX\n");
    algorithmX(d);
    printf("number of calls: %lu\n", d->numCalls);
    printf("numSols: %d\n", d->numSols);

    uncoverRowHeaders(d);
    unhideAllCells(d);

    printSolutions_Sudoku(d);

    saveSolution_Sudoku(d);
    writeSudokuToFile(d->s, dlx->solutionFile);

    free_DLXSolve(dlx);
}

void run_DLXGenFull(int argc, char *argv[])
{
    int res = NOT_FOUND, i;
    DLX *dlx = malloc(sizeof(DLX));
    Dance *d;
    parseArgs_DLXGenFull(dlx, argc, argv);
    d = dlx->d;
    
    findMatrixFile(d);
    setMatrixDimensions(d, d->s);
    initDance(d);
    initMatrix(d);
    HEUR_INIT(d, d->s->containerSize)

    coverRowHeaders(d);

    printf("starting algX\n");
    for(i = 1; i < THRESHOLD_TRY+1; i++)
    {
        d->numCalls = 0;
        res = algorithmX_Gen_Rand(d);
        if(res == FOUND)
            break;
    }

    uncoverRowHeaders(d);
    if(res == FOUND)
    {
        saveSolution_Sudoku(d);
        printSudoku(d->s);
        writeSudokuToFile(d->s, dlx->solutionFile);
    }
    else if(res == NOT_FOUND)
        printf("No solutions found");

    free_DLXGenFull(dlx);
}

void run_DLXGenPartial(int argc, char *argv[])
{
    int res = NOT_FOUND, i;
    DLX *dlx = malloc(sizeof(DLX));
    Dance *d;
    parseArgs_DLXGenPartial(dlx, argc, argv);
    d = dlx->d;
    
    findMatrixFile(d);
    setMatrixDimensions(d, d->s);
    initDance(d);
    initMatrix(d);
    HEUR_INIT(d, d->s->containerSize)
    
    initHide(d);
    setNumClues(d->s, d->s->sudokuSize * (1.0/2));
    printf("starting generation\n");
    for(i = 1; i < THRESHOLD_TRY+1; i++)
    {
        d->genNumCalls = 0;
        //printf("gen try: %d\n", i);
        res = generate(d);
        if(res == FOUND)
            break;
    }
    if(res == NOT_FOUND)
        printf("No puzzles found\n");

    printSudoku(d->s);
    writeSudokuToFile(d->s, dlx->sudokuFile);

    unhideAllCells(d);

    free_DLXGenPartial(dlx);
}

void run_Enumerate(int argc, char *argv[])
{
    Enum *e = malloc(sizeof(Enum));

    parseArgs_Enumerate(e, argc, argv);

    findMatrixFile(e->d);
    setMatrixDimensions(e->d, e->d->s);
    initDance(e->d);
    initMatrix(e->d);
    HEUR_INIT(e->d, e->d->s->containerSize)
    initHide(e->d);
    hideAllCells(e->d);
    coverRowHeaders(e->d);

    algorithmX_Enumerate(e->d);
    printf("numSols: %d\n", e->d->numSols);

    uncoverRowHeaders(e->d);
    unhideAllCells(e->d);
    writeToEnumerateFile(e->d, e->enumerateFile);

    free_Enum(e);
}

void run_ZchaffSolve0(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffSolve0(z, argc, argv);

    printSudoku(z->s);
    writeToDimacs_Solve(z);
    printf("written to dimacs input\n");

    free_ZChaffSolve0(z);
}

void run_ZchaffSolve1(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffSolve1(z, argc, argv);
    printf("finished zchaff\n");

    readInDimacsOutput(z, z->dimacsOutputFile);
    printSudoku(z->s);

    writeSudokuToFile(z->s, z->solutionFile);

    free_ZChaffSolve1(z);
}

void run_ZchaffGen0(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffGen0(z, argc, argv);

    //testConvertSat(z);
    writeToDimacs_Gen(z);
    printf("written to dimacs input\n");

    free_ZChaffGen0(z);
}

void run_ZchaffGen1(int argc, char *argv[])
{
    ZChaff *z = malloc(sizeof(ZChaff));
    parseArgs_ZChaffGen1(z, argc, argv);

    printf("finished zchaff\n");
    readInDimacsOutput(z, z->dimacsOutputFile);
    printSudoku(z->s);
    writeSudokuToFile(z->s, z->solutionFile);

    free_ZChaffGen1(z);
}