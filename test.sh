#!/bin/bash
USAGE="usage: [ b:basic | m:make | q:quit | t:time]"

while true; do

echo "what type of test:"
echo $USAGE
read type

if [ "$type" = "q" ]; then
   exit 1

elif [ "$type" = "m" ]; then
   make clean
   make

elif [ "$type" = "c" ]; then
   complexity -t0 -s1 *.c

elif [ "$type" = "s" ]; then
   ~kmammen-grader/bin/styleCheckC *.c

elif [ "$type" = "g" ]; then
   make clean
   gcc -pg -Wall -std=c89 -pedantic *.c 
   echo "mode? s to solve, g to generate"
   read mode
   echo "dimension? \"row col\""
   read row col
   ./a.out ${mode} tests/${mode}${row}x${col}.in
   gprof a.out gmon.out | head -25
   make clean
   make

elif [ "$type" = "t" ] || [ "$type" = "v" ] || [ "$type" = "b" ]; then
   echo "mode? s to solve, g to generate"
   read mode
   echo "dimension? \"row col\""
   read row col

if [ "$type" = "t" ]; then
   time ./a.out ${mode} tests/${mode}${row}x${col}.in

elif [ "$type" = "v" ]; then
   valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out ${mode} tests/${mode}${row}x${col}.in

elif [ "$type" = "b" ]; then
   ./a.out ${mode} tests/${mode}${row}x${col}.in
fi

fi

done
