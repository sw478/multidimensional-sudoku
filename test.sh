#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | >:push | <:pull ]"
OPTIONS0="[ s:dlx solve | g:dlx generate | e:dlx enumerate ]"
OPTIONS1="[ zs:zchaff solve | zg:zchaff gen ]"
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

   elif [ "$type" = "<" ]; then
      git pull

   elif [ "$type" = "b" ]; then
      echo $OPTIONS0
      echo $OPTIONS1
      read option

      if [ "$option" = "s" ]; then
         make
         $EXECUTABLE s $SUDOKU_FILE $SOLUTION_FILE
      
      elif [ "$option" = "g" ]; then
         echo "dimensions?"
         read dim
         
         make
         $EXECUTABLE gf $SOLUTION_FILE ${dim}
         $EXECUTABLE gp $SOLUTION_FILE $SUDOKU_FILE
      
      elif [ "$option" = "e" ]; then
         echo "dimensions?"
         read dim
         
         make
         $EXECUTABLE e $ENUMERATE_FILE ${dim}
      
      elif [ "$option" = "zg" ]; then
         echo "dimensions?"
         read dim

         make
         $EXECUTABLE zg0 $DIMACS_INPUT_FILE ${dim}
         ./zchaff $DIMACS_INPUT_FILE > $DIMACS_OUTPUT_FILE
         $EXECUTABLE zg1 $DIMACS_OUTPUT_FILE $SUDOKU_FILE $SOLUTION_FILE ${dim}
         $EXECUTABLE gp $SOLUTION_FILE $SUDOKU_FILE
      
      elif [ "$option" = "zs" ]; then
         make
         $EXECUTABLE zs0 $DIMACS_INPUT_FILE $SUDOKU_FILE
         ./zchaff $DIMACS_INPUT_FILE > $DIMACS_OUTPUT_FILE
         $EXECUTABLE zs1 $DIMACS_OUTPUT_FILE $SUDOKU_FILE $SOLUTION_FILE

      fi

   fi

echo
done