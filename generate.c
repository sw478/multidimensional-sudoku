#include "generate.h"
#include "hide.h"
#include "dance.h"
#include "auxil.h"
#include "hrowCover.h"
#define FOUND 1
#define NOT_FOUND 0

int generate(Dance *d)
{
    int *hitList, res = NOT_FOUND;
    Hide *h;

    if(d->hideRoot->num >= d->numClues)
        return NOT_FOUND;
    
    hitList = calloc(d->s->gridSize - d->hideRoot->num, sizeof(int));

    h = nextHide(d, &hitList);
    while(h != d->hideRoot)
    {
        fillSingleCell(d, h);

        d->numSols = 0;
        coverRowHeaders(d);
        algorithmX_SGen2(d);
        uncoverRowHeaders(d);
        if(d->numSols == 1)
        {
            saveGeneratedPuzzle(d);
            res = FOUND;
        }
        else
            res = generate(d);
        
        unfillSingleCell(d, h);
        if(res == FOUND)
            break;

        h = nextHide(d, &hitList);
    }

    free(hitList);
    return res;
}

/* returns a random igrid without hiding anything */
Hide *nextHide(Dance *d, int **hitList)
{
    Hide *h;
    int i, j, randInt, listSize = d->s->gridSize - d->hideRoot->num;
    if(listSize == 0)
        return d->hideRoot;

    /* pick random number in range of number of elements left */
    randInt = rand() % listSize;

    /* go to a random uncovered hide */
    for(h = d->hideRoot->next, i = 0; i < randInt; h = h->next, i++);

    /* continue going through list of uncovered hides until unvisited hide is found */
    for(j = i; (*hitList)[j] == 1; j++, h = h->next)
    {
        if(h == d->hideRoot)
            continue;
    }

    (*hitList)[j] = 1;
    return h;
}

int generate2(Dance *d)
{
    int igrid, randInt;
    int gridSize = d->s->gridSize;

    unfillAllCells(d);
    printf("all cells unfilled\n");

    for(d->numSols = 0; d->numSols != 1;)
    {
        /* pick a random cell to fill */
        randInt = rand() % gridSize;
        for(igrid = (randInt + 1) % gridSize; d->hideList[igrid]->filled; igrid = (igrid + 1) % gridSize)
        {
            if(igrid == randInt)
            {
                if(d->hideList[igrid]->filled)
                {
                    printf("All cells filled\n");
                    return 0;
                }
                break;
            }
        }
        //printf("igrid: %d\n", igrid);
        fillSingleCell(d, d->hideList[igrid]);
        //checkMatrix(d);

        printSudokuBoard_Gen(d);

        d->numSols = 0;
        coverRowHeaders(d);
        algorithmX_SGen2(d);
        uncoverRowHeaders(d);
    }

    return 0;
}

void printToSudokuFile(Dance *d)
{
    int igrid, gridSize = d->s->gridSize, *grid = d->s->grid;

    for(igrid = 0; igrid < gridSize; igrid++)
        fprintf(d->s->boardFile, "%d\n", grid[igrid]);
}