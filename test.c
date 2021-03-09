#include "test.h"
#include "dance.h"
#include "auxil.h"

void testRandRows(Dance *d)
{
    Doubly *candidateRow, *hcol;
    int listSize, *hitList;

    printMatrix(d);
    hcol = d->root->right;

    listSize = hcol->drow - d->rmax;
    hitList = calloc(listSize, sizeof(int));
    candidateRow = nextRow(hcol, &listSize, &hitList);

    while(candidateRow != hcol)
    {
        printf("drow: %d\n", candidateRow->drow);
        candidateRow = nextRow(hcol, &listSize, &hitList);
    }
}