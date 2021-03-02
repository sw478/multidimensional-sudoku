#include "heuristic.h"

/*
   d->heurRoot acts as a header for hcols with 0 elements underneath

   maxColElements is optional, set to 0 if unknown
   maxColElements is just the maximum amount of elements underneath an hcol possible
*/
void initHeurList(Dance *d, int maxColElements)
{
   Doubly *hcol;
   Heur *heur, *heurHeader, *newHeurHeader;
   int hnum;
   d->heurRoot = initHeur(0);
   d->heurRoot->hcol = ((void*)d->root);

   // if maxColElements set to 0, set maxColElements to d->rmax
   if(maxColElements < 1)
      maxColElements = d->rmax;

   // initialize heur headers from the back
   for(hnum = 1; hnum < maxColElements; hnum++)
   {
      newHeurHeader = initHeur(hnum);
      newHeurHeader->hnext = d->heurRoot;
      newHeurHeader->hprev = d->heurRoot->hprev;
      d->heurRoot->hprev->hnext = newHeurHeader;
      d->heurRoot->hprev = newHeurHeader;
   }

   for(hcol = d->root->right; hcol != d->root; hcol = hcol->right)
   {
      // associate heur with current hcol
      heur = initHeur(hcol->drow - d->rmax);
      heur->hcol = ((void*)hcol);
      hcol->heur = heur;

      // find the correct heur header
      heurHeader = d->heurRoot;
      for(; heur->num > heurHeader->num; heurHeader = heurHeader->hnext);
      assert(heur->num == heurHeader->num);

      // add heur to heur header
      heur->next = heurHeader;
      heur->prev = heurHeader->prev;
      heurHeader->prev->next = heur;
      heurHeader->prev = heur;
      heur->head = heurHeader;
   }
}

Heur *initHeur(int num)
{
   Heur *heur = malloc(sizeof(Heur));
   heur->head = heur->prev = heur->next = heur;
   heur->hprev = heur->hnext = heur;
   heur->num = num;

   return heur;
}

void incHeur(Heur *heur)
{
   Heur *head = heur->head;

   // remove heur from current heur sublist
   heur->num++;
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;

   // go to correct heur header
   head = head->hnext;

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
   heur->head = head;
}

void decHeur(Heur *heur)
{
   Heur *head = heur->head;

   // remove heur from current heur sublist
   heur->num--;
   heur->next->prev = heur->prev;
   heur->prev->next = heur->next;

   // go to correct heur header
   head = head->hprev;

   // add heur to heur header
   heur->next = head;
   heur->prev = head->prev;
   head->prev->next = heur;
   head->prev = heur;
   heur->head = head;
}

void freeHeur(Dance *d)
{
   Heur *head, *temp;

   for(head = d->heurRoot->hnext; head != d->heurRoot;)
   {
      freeHeurHead(head);
      temp = head;
      head = head->hnext;
      free(temp);
   }
   freeHeurHead(head);

   free(d->heurRoot);
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
}
