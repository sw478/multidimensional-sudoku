#!/bin/bash
USAGE="[ q:quit | m:make | b:basic | t:time ]"

while true; do

echo
echo $USAGE
read type

if [ "$type" = "q" ]; then
   exit 0

elif [ "$type" = "m" ]; then
   make clean
   make

elif [ "$type" = "b" ] || [ "$type" = "t" ] || [ "$type" = "v" ]; then
   echo "problem:"
   echo "\"s\" for sudoku"
   echo "\"g\" for generating sudoku"
   read problem

   if [ "$problem" = "s" ]; then
      
      if [ "$type" = "t" ]; then
         make
         time ./a.out s tests/dm.txt

      elif [ "$type" = "v" ]; then
         make
         valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out s tests/dm.txt

      elif [ "$type" = "b" ]; then
         make
         ./a.out s tests/dm.txt
      fi
   
   elif [ "$problem" = "g" ]; then
      echo "dimensions?"
      read dim
      
      if [ "$type" = "t" ]; then
         time ./a.out g tests/dm.txt ${dim}

      elif [ "$type" = "v" ]; then
         valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out g tests/dm.txt ${dim}

      elif [ "$type" = "b" ]; then
         make
         ./a.out g tests/dm.txt ${dim}
      fi

   fi

fi

done