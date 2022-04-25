#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | >:push | <:pull ]"
OPTIONS="[ s:solve | g:generate | e:enumerate | zg: zchaff gen ]"
SUDOKU_FILE="sudokuFile.txt"
SOLUTION_FILE="solutionFile.txt"
DIMACS_INPUT_FILE="dimacsInput.txt"
DIMACS_OUTPUT_FILE="dimacsOutput.txt"
ENUMERATE_FILE="enumerateFile.txt"

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
         ./a.out s $SUDOKU_FILE $SOLUTION_FILE
      
      elif [ "$problem" = "g" ]; then
         echo "dimensions?"
         read dim
         
         ./a.out g $SUDOKU_FILE $SOLUTION_FILE ${dim}
      
      elif [ "$problem" = "e" ]; then
         echo "dimensions?"
         read dim
         
         ./a.out e $ENUMERATE_FILE ${dim}
      
      elif [ "$problem" = "zg" ]; then
         echo "dimensions?"
         read dim

         ./a.out zg0 $DIMACS_INPUT_FILE ${dim}
         ./zchaff.64bit.2007.3.12/zchaff64/zchaff $DIMACS_INPUT_FILE > $DIMACS_OUTPUT_FILE
         #./a.out zg1 $SOLUTION_FILE $DIMACS_OUTPUT_FILE

      fi

   fi

echo
done