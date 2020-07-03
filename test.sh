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
   ./a.out tests/s1.in
   gprof a.out gmon.out | head -20
   make clean
   make

elif [ "$type" = "t" ] || [ "$type" = "v" ] || [ "$type" = "b" ]; then
   echo "mode? 1 to solve, 2 to generate"
   read mode
   echo "dimension? \"row col\""
   read row col

if [ "$type" = "t" ]; then
   time ./a.out ${mode} tests/s${row}x${col}.in

elif [ "$type" = "v" ]; then
   valgrind --leak-check=full --error-exitcode=13 ./a.out ${mode} tests/s${row}x${col}.in

elif [ "$type" = "b" ]; then
   ./a.out ${mode} tests/s${row}x${col}.in
fi

fi

done
