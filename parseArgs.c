#include "parseArgs.h"
#include "error.h"

/*
 * argument format: a.out [problem (s/s2/q)] ...
 * 
 * sudoku/sudoku2:
 * a.out [s/s2] [mode: s for solve, g for gen] [file: empty to be
 * filled with only dimensions if generating, filled if solving]
 * 
 * n queens:
 * a.out [q] [n]
 */
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

    if(d->problem == SUDOKU || d->problem == SUDOKU2)
        parseArgs_Sudoku(d, argc, argv);
    else if(d->problem == NQUEENS)
        parseArgs_NQueens(d, argc, argv);
   
}

void parseArgs_Sudoku(Dance *d, int argc, char *argv[])
{
    int i, c, test;
    Sudoku *s;
    char *buf = malloc(BUFSIZE*sizeof(char)), temp;
    memset(buf, 0, BUFSIZE*sizeof(char));


    s = malloc(sizeof(Sudoku));
    if(argc != 4)
        numArgError();
    if(!strcmp(argv[2], "s")) /* solve */
        s->mode = 1;
    else if(!strcmp(argv[2], "g")) /* generate */
        s->mode = 2;
    else
        usage();
    if(s->mode == 1)
    {
        s->in = fopen(argv[3], "r+");
        if(!s->in)
            fileError(argv[3]);
    }
    if(s->mode == 2)
        s->in = fopen(argv[3], "w+");

    assert(3 == sscanf(argv[3], "tests/%c/%dx%d.in", &temp, &s->x, &s->y));
    s->xy = s->x*s->y;
    s->gridSize = s->xy*s->xy;
    s->grid = calloc(s->gridSize, sizeof(int));

    assert(fseek(s->in, 0, SEEK_SET) == 0);
    if(s->mode == 2)
    {
        sprintf(buf, "%d %d\n", s->x, s->y);
        fwrite(buf, 1, BUFSIZE*sizeof(char), s->in);
        free(buf);
        return;
    }

    fgets(buf, BUFSIZE*sizeof(char), s->in);
    for(i = 0; i < s->gridSize; i++)
    {
        fgets(buf, BUFSIZE, s->in);
        test = sscanf(buf, "%d", &c);
        /*
            board is invalid if sudoku numbers are not
            within 0 to xy inclusive
        */
        if(test < 1 || c < 0 || c > s->xy)
            invalidSudokuBoard();
        s->grid[i] = c;
    }
    d->s = s;
    free(buf);
}

void parseArgs_NQueens(Dance *d, int argc, char *argv[])
{
    if(argc != 3){
        printf("argc: %d\n", argc);
        numArgError();}
    if(1 != sscanf(argv[2], "%d", &d->nq))
        invalidN_NQueens();
}