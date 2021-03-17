#include "heuristic.h"
#include "error.h"

/* using heur */
Doubly *heuristic(Dance *d)
{
   Heur *heurHeader;
   int hnum, listSize = d->maxColElements + 1;

   for(hnum = 1; hnum < listSize; hnum++)
   {
      heurHeader = d->heurList[hnum];
      if(heurHeader->next != heurHeader)
         return heurHeader->next->hcol;
   }

   return d->root;
/*
   if(d->heurRoot->hnext == d->heurRoot)
   {
      assert(0);
      return d->root;
   }

   // find the first non-empty heur header
   for(heurHeader = d->heurRoot->hnext; heurHeader != d->heurRoot; heurHeader = heurHeader->hnext)
   {
      if(heurHeader->next != heurHeader)
         return heurHeader->next->hcol;
   }

   // signifying none has been found
   return d->root;
   */
}

/* original setup, O(cmax) time */
Doubly *heuristic2(Dance *d)
{
   Doubly *hcol, *minXs;

   for(hcol = minXs = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      if(hcol->drow < minXs->drow)
         minXs = hcol;
   }

   return minXs;
}

/*
   d->heurRoot acts as a header for hcols with 0 elements underneath

   maxColElements is optional, set to 0 if unknown
   maxColElements is just the maximum amount of elements underneath an hcol possible
*/
void initHeurList(Dance *d, int maxColElements)
{
   Doubly *hcol;
   Heur *heur, *heurHeader;//, *newHeurHeader;
   int hnum;
   //d->heurRoot = initHeur(0);
   //d->heurRoot->hcol = ((void*)d->root);

   assert(maxColElements > 0 && maxColElements <= d->rmax);

   d->heurList = malloc((maxColElements+1) * sizeof(Heur));
   d->maxColElements = maxColElements;
   for(hnum = 0; hnum < maxColElements+1; hnum++)
      d->heurList[hnum] = initHeur(hnum);

/*
   // initialize heur headers from the back
   // note off by one
   for(hnum = 1; hnum < maxColElements + 1; hnum++)
   {
      newHeurHeader = initHeur(hnum);
      newHeurHeader->hnext = d->heurRoot;
      newHeurHeader->hprev = d->heurRoot->hprev;
      d->heurRoot->hprev->hnext = newHeurHeader;
      d->heurRoot->hprev = newHeurHeader;
   }
*/
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      hnum = hcol->drow - d->rmax;
      assert(hnum <= maxColElements);
      heur = initHeur(hnum);
      heur->hcol = ((void*)hcol);
      hcol->heur = heur;

      heurHeader = d->heurList[hnum];
      heur->next = heurHeader;
      heur->prev = heurHeader->prev;
      heurHeader->prev->next = heur;
      heurHeader->prev = heur;
   }
/*
   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      // associate heur with current hcol
      hnum = hcol->drow - d->rmax;
      assert(hnum <= maxColElements);
      heur = initHeur(hnum);
      heur->hcol = ((void*)hcol);
      hcol->heur = heur;

      // find the correct heur header
      heurHeader = d->heurRoot->hnext;
      for(; heur->num > heurHeader->num; heurHeader = heurHeader->hnext)
         assert(heurHeader != d->heurRoot);

      // add heur to heur header
      heur->next = heurHeader;
      heur->prev = heurHeader->prev;
      heurHeader->prev->next = heur;
      heurHeader->prev = heur;
      heur->heurHeader = heurHeader;
   }*/
}

Heur *initHeur(int num)
{
   Heur *heur = malloc(sizeof(Heur));
   heur->prev = heur->next = heur;
   //heur->heurHeader = heur->hprev = heur->hnext = heur;
   heur->num = num;

   return heur;
}

void incHeur(Dance *d, Heur *heur, int amount)
{
   Heur *head;// = heur->heurHeader;
   //int i;

   // remove heur from current heur sublist
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;
   heur->num += amount;
   
   head = d->heurList[heur->num];

   /*
      check if hcol and heur num matches up
      not needed unless debugging
   */
   //if(((Doubly*)heur->hcol)->drow - d->rmax != heur->num)
   //   heurNumError();

   // go to correct heur header
   //for(i = 0; i < amount; i++)
     // head = head->hnext;

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
}

void decHeur(Dance *d, Heur *heur, int amount)
{
   Heur *head;// = heur->heurHeader;
   //int i;

   // remove heur from current heur sublist
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;
   heur->num -= amount;

   head = d->heurList[heur->num];

   /*
      check if hcol and heur num matches up
      not needed unless debugging
   */
   //if(((Doubly*)heur->hcol)->drow - d->rmax != heur->num)
   //   heurNumError();

   // go to correct heur header
   //for(i = 0; i < amount; i++)
     // head = head->hprev;

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
}

void freeHeur(Dance *d)
{
   int hnum;
   //Heur *head, *temp;
/*
   for(head = d->heurRoot->hnext; head != d->heurRoot;)
   {
      freeHeurHead(head);
      temp = head;
      head = head->hnext;
      free(temp);
   }
   freeHeurHead(head);

   free(d->heurRoot);
   */
   for(hnum = 0; hnum < d->maxColElements+1; hnum++)
      freeHeurHead(d->heurList[hnum]);
   free(d->heurList);
}

void freeHeurHead(Heur *head)
{
   Heur *heur, *temp;
   for(heur = head->next; heur != head;)
   {
      temp = heur;
      heur = heur->next;
      free(temp);
   }
   free(head);
}
