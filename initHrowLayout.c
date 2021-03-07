#include "initHrowLayout.h"

/*
    used for sudoku 2

    saves coordinates of layout in hrow using bits of rowLayout
*/
void initHrowLayout(Dance *d)
{
    Doubly *hrow, *xrow;
    int xy = d->s->xy, shift, index, igrid;

    /* maximum number of set of 16 bits to fully encapsulate the sudoku board */
    d->max16mult = ((xy*xy - 1) / 16) + 1;

    for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
    {
        hrow->rowLayout = calloc(d->max16mult, sizeof(uint16_t));
        /* skip first */
        for(xrow = hrow->right->right; xrow != hrow; xrow = xrow->right)
        {
            igrid = xrow->dcol - xy;
            shift = igrid % 16;
            index = igrid / 16;
            hrow->rowLayout[index] |= (1 << shift);
        }
    }
}

void freeHrowLayout(Dance *d)
{
    Doubly *hrow;

    for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
        free(hrow->rowLayout);
}

void printHrowLayout(Dance *d)
{
    Doubly *hrow;
    int index;

    for(hrow = d->root->down; hrow != d->root; hrow = hrow->down)
    {
        printf("%d\t", hrow->drow);
        for(index = 0; index < d->max16mult; index++)
            printBinary(hrow->rowLayout[index]);
        printf("\n");
    }
    printf("max16mult: %d\n", d->max16mult);
}

void printBinary(uint64_t num)
{
    int bit, ibit;

    printf(" ");
    for(ibit = 0; ibit < 16; ibit++)
    {
        bit = num % 2;
        num = num / 2;
        printf("%d", bit);
    }
}