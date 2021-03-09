#include "secondaryColumns.h"

void set_secondary_columns(Dance *d, int index)
{
   d->sec_hcol_index = index;
}

/*
   convert hcols

      root <> ... <> temp2 <> hcol <> ... <> temp1 <> root

   to

      root <> ... <> temp2 <> root
      hcol <> ... <> temp1 <> hcol

   run this before initHeur to prevent heurs on secondary columns
*/
void stitch_secondary(Dance *d)
{
   Doubly *hcol, *temp1, *temp2;

   for(hcol = d->root->right; hcol->dcol != d->sec_hcol_index; hcol = hcol->right);
   d->hcol_sec = hcol;

   temp1 = d->root->left;
   temp2 = hcol->left;

   d->root->left = temp2;
   temp2->right = d->root;

   hcol->left = temp1;
   temp1->right = hcol;
}

void unstitch_secondary(Dance *d)
{
   Doubly *hcol, *temp1, *temp2;

   hcol = d->hcol_sec;

   temp1 = hcol->left;
   temp2 = d->root->left;

   d->root->left = temp1;
   temp1->right = d->root;

   hcol->left = temp2;
   temp2->right = hcol;
}