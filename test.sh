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
      echo "dimensions?"
      read x y
      
      if [ "$type" = "t" ]; then
         make
         time ./a.out ${problem} tests/${x}x${y}.txt

      elif [ "$type" = "v" ]; then
         make
         valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out ${problem} tests/${x}x${y}.txt

      elif [ "$type" = "b" ]; then
         make
         ./a.out ${problem} tests/${x}x${y}.txt
      fi
   
   elif [ "$problem" = "g" ]; then
      echo "dimensions?"
      read x y
      
      if [ "$type" = "t" ]; then
         time ./a.out g tests/${x}x${y}.txt ${x} ${y}

      elif [ "$type" = "v" ]; then
         valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out g tests/${x}x${y}.txt ${x} ${y}

      elif [ "$type" = "b" ]; then
         make
         ./a.out g tests/${x}x${y}.txt ${x} ${y}
      fi

   fi

fi

done