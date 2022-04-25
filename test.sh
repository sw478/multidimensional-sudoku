#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | >:push | <:pull ]"
OPTIONS="[ s:solve | g:generate | e:enumerate | zg: zchaff gen ]"
EXECUTABLE="./mSudoku"
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

      if [ "$problem" = "s" ]; then
         make
         $EXECUTABLE s $SUDOKU_FILE $SOLUTION_FILE
      
      elif [ "$problem" = "g" ]; then
         echo "dimensions?"
         read dim
         
         make
         $EXECUTABLE g $SUDOKU_FILE $SOLUTION_FILE ${dim}
      
      elif [ "$problem" = "e" ]; then
         echo "dimensions?"
         read dim
         
         make
         $EXECUTABLE e $ENUMERATE_FILE ${dim}
      
      elif [ "$problem" = "zg" ]; then
         echo "dimensions?"
         read dim

         make
         $EXECUTABLE zg0 $DIMACS_INPUT_FILE ${dim}
         ./zchaff.64bit.2007.3.12/zchaff64/zchaff $DIMACS_INPUT_FILE > $DIMACS_OUTPUT_FILE
         $EXECUTABLE zg1 $DIMACS_OUTPUT_FILE $SUDOKU_FILE $SOLUTION_FILE ${dim}

      fi

   fi

echo
done