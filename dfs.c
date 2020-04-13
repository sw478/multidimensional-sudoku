#include "sudoku.h"

/* 
 * depth first search
 * returns 0 if works, 1 if couldn't find a solution
 */
int dfs(Sudoku *s)
{
   int r, c, i;
   Sudoku *branch = malloc(sizeof(Sudoku));

   for(r = 0; r < s->xy; r++)
   {
      for(c = 0; c < s->xy; c++)
      {
         if(s->grid[r*s->xy + c] != 0)
            continue;
         for(i = 1; i <= s->xy; i++)
         {
            s->steps++;
            if(checkBoard(s, r, c, i))
               continue;
            memcpy(branch, s, sizeof(Sudoku));
            branch->grid[r*s->xy + c] = i;
            branch->elements++;
            if(0 == dfs(branch))
            {
               memcpy(s, branch, sizeof(Sudoku));
               free(branch);
               return 0;
            }
         }
         free(branch);
         return 1;
      }
   }

   free(branch);
   return 0;
}
