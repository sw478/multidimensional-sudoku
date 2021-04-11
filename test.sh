#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | t:time ]"
OPTIONS="[ s:solve | g:generate | b: bSAT ]"
SUDOKU_FILE="sudokuFile.txt"
SOLUTION_FILE="solutionFile.txt"

while true; do

   echo $USAGE
   read type

   if [ "$type" = "q" ]; then
      exit 0

   elif [ "$type" = "m" ]; then
      make clean
      make

   elif [ "$type" = "b" ] || [ "$type" = "t" ] || [ "$type" = "v" ]; then
      echo "if sudokuFile does not exist yet, generate sudoku first"
      echo $OPTIONS
      read problem

      make

      if [ "$problem" = "s" ]; then
         
         if [ "$type" = "t" ]; then
            time ./a.out s $SUDOKU_FILE $SOLUTION_FILE

         elif [ "$type" = "v" ]; then
            valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out s $SUDOKU_FILE $SOLUTION_FILE

         elif [ "$type" = "b" ]; then
            ./a.out s $SUDOKU_FILE $SOLUTION_FILE
         fi
      
      elif [ "$problem" = "g" ]; then
         echo "dimensions?"
         read dim
         
         if [ "$type" = "t" ]; then
            time ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}

         elif [ "$type" = "v" ]; then
            valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}

         elif [ "$type" = "b" ]; then
            ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}
         fi
      
      elif [ "$problem" = "b" ]; then
         
         if [ "$type" = "t" ]; then
            time ./a.out b $SUDOKU_FILE $SOLUTION_FILE

         elif [ "$type" = "v" ]; then
            valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out b $SUDOKU_FILE $SOLUTION_FILE

         elif [ "$type" = "b" ]; then
            ./a.out b $SUDOKU_FILE $SOLUTION_FILE
         fi

      fi

   fi

echo
done