#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | >:push | <:pull ]"
OPTIONS="[ s:solve | g:generate | e:enumerate | zg: zchaff gen ]"
SUDOKU_FILE="sudokuFile.txt"
SOLUTION_FILE="solutionFile.txt"
DIMACS_FILE="dimacs.txt"

while true; do

   echo $USAGE
   read type

   if [ "$type" = "q" ]; then
      exit 0

   elif [ "$type" = "m" ]; then
      make clean
      make

   elif [ "$type" = ">" ]; then
      git add .
      git commit -m "."
      git push

   elif [ "$type" = ">" ]; then
      git pull

   elif [ "$type" = "b" ]; then
      echo "if sudokuFile does not exist yet, generate sudoku first"
      echo $OPTIONS
      read problem

      make

      if [ "$problem" = "s" ]; then
         
         if [ "$type" = "t" ]; then
            time ./a.out s $SUDOKU_FILE $SOLUTION_FILE

         elif [ "$type" = "b" ]; then
            ./a.out s $SUDOKU_FILE $SOLUTION_FILE
         fi
      
      elif [ "$problem" = "g" ]; then
         echo "dimensions?"
         read dim
         
         if [ "$type" = "t" ]; then
            time ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}

         elif [ "$type" = "b" ]; then
            ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}
         fi
      
      elif [ "$problem" = "e" ]; then
         echo "dimensions?"
         read dim
         
         ./a.out e $SOLUTION_FILE ${dim}
      
      elif [ "$problem" = "zg" ]; then
         echo "dimensions?"
         read dim

         ./a.out zg0 $SUDOKU_FILE $SOLUTION_FILE ${dim}
         ./zchaff.64bit.2007.3.12/zchaff64/zchaff $DIMACS_FILE > zchaff_out.txt

      fi

   fi

echo
done