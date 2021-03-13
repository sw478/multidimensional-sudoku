#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | t:time ]"
OPTIONS="[ s:solve | g:generate ]"
SUDOKU_FILE="sudokuFile.txt"

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

      if [ "$problem" = "s" ]; then
         
         if [ "$type" = "t" ]; then
            make
            time ./a.out s $SUDOKU_FILE

         elif [ "$type" = "v" ]; then
            make
            valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out s $SUDOKU_FILE

         elif [ "$type" = "b" ]; then
            make
            ./a.out s $SUDOKU_FILE
         fi
      
      elif [ "$problem" = "g" ]; then
         echo "dimensions?"
         read dim
         
         if [ "$type" = "t" ]; then
            time ./a.out g $SUDOKU_FILE ${dim}

         elif [ "$type" = "v" ]; then
            valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out g $SUDOKU_FILE ${dim}

         elif [ "$type" = "b" ]; then
            make
            ./a.out g $SUDOKU_FILE ${dim}
         fi

      fi

   fi

echo
done