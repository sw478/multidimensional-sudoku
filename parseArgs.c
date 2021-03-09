#include "parseArgs.h"
#include "error.h"

void parseArgs(Dance *d, int argc, char *argv[])
{

    if(argc < 1)
        numArgError();

    if(!strcmp(argv[1], "s"))
        d->problem = SUDOKU;
    else if(!strcmp(argv[1], "s2"))
        d->problem = SUDOKU2;
    else if(!strcmp(argv[1], "q"))
        d->problem = NQUEENS;
    else if(!strcmp(argv[1], "sg"))
        d->problem = SGEN;
    else
        arg1Error();

    switch(d->problem)
    {
        case SUDOKU:
        case SUDOKU2: parseArgs_Sudoku(d, argc, argv); break;
        case NQUEENS: parseArgs_NQueens(d, argc, argv); break;
        case SGEN: parseArgs_SGen(d, argc, argv); break;
    }
}

/* a.out [s/s2] [file] */
void parseArgs_Sudoku(Dance *d, int argc, char *argv[])
{
    int i, c, test;
    Sudoku *s;
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));

    if(argc != 3)
        numArgError();
    
    s = malloc(sizeof(Sudoku));
    d->s = s;
    s->boardFile = fopen(argv[2], "r+");
    if(!s->boardFile)
        fileError(argv[2]);

    assert(2 == sscanf(argv[2], "tests/s/%dx%d.in", &s->x, &s->y));
    s->xy = s->x*s->y;
    s->gridSize = s->xy*s->xy;
    s->grid = calloc(s->gridSize, sizeof(int));

    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    fgets(buf, BUFSIZE*sizeof(char), s->boardFile);
    for(i = 0; i < s->gridSize; i++)
    {
        fgets(buf, BUFSIZE, s->boardFile);
        test = sscanf(buf, "%d", &c);
        /*
            board is invalid if sudoku numbers are not
            within 0 to xy inclusive
        */
        if(test != 1 || c < 0 || c > s->xy)
            invalidSudokuBoard();
        s->grid[i] = c;
    }
    free(buf);
}

/* a.out q [n]  */
void parseArgs_NQueens(Dance *d, int argc, char *argv[])
{
    if(argc != 3){
        numArgError();}
    if(1 != sscanf(argv[2], "%d", &d->nq))
        invalidN_NQueens();
}

/* a.out sg [file] [x] [y] */
void parseArgs_SGen(Dance *d, int argc, char *argv[])
{
    Sudoku *s;
    char *buf = malloc(BUFSIZE*sizeof(char));
    memset(buf, 0, BUFSIZE*sizeof(char));

    if(argc != 5)
        numArgError();

    s = malloc(sizeof(Sudoku));
    d->s = s;
    s->boardFile = fopen(argv[2], "w+");

    assert(1 == sscanf(argv[3], "%d", &s->x));
    assert(1 == sscanf(argv[4], "%d", &s->y));
    s->xy = s->x*s->y;
    s->gridSize = s->xy*s->xy;
    s->grid = calloc(s->gridSize, sizeof(int));

    assert(fseek(s->boardFile, 0, SEEK_SET) == 0);

    /* print board dimensions at top of file */
    sprintf(buf, "%d %d\n", s->x, s->y);
    fwrite(buf, 1, BUFSIZE*sizeof(char), s->boardFile);
    free(buf);
}