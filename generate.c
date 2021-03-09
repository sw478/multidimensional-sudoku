#include "generate.h"
#include "hide.h"
#include "dance.h"
#include "auxil.h"
#include "hrowCover.h"

void generate(Dance *d)
{
    int igrid, randInt;
    int gridSize = d->s->gridSize;

    unfillAllCells(d);
    printf("all cells unfilled\n");

    for(d->numSols = 0; d->numSols != 1;)
    {
        /* pick a random cell to fill */
        randInt = rand() % gridSize;
        for(igrid = randInt; d->hideList[igrid]->filled; igrid = (igrid + 1) % gridSize);
        //printf("igrid: %d\n", igrid);
        fillSingleCell(d, igrid);

        //printSudokuBoard_Gen(d);

        d->numSols = 0;
        coverRowHeaders(d);
        algorithmX_SGen2(d);
        uncoverRowHeaders(d);
    }
}

void printToSudokuFile(Dance *d)
{
    int igrid, gridSize = d->s->gridSize, *grid = d->s->grid;

    for(igrid = 0; igrid < gridSize; igrid++)
        fprintf(d->s->boardFile, "%d\n", grid[igrid]);
}