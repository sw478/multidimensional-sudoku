#!/bin/bash
USAGE="usage: [ b:basic | q:quit | t:time | s:style | c:complexity | v:valgrind | g:gprof]"

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

elif [ "$type" = "t" ]; then
   time ./a.out tests/s1.in

elif [ "$type" = "v" ]; then
   echo "dimension? \"row col\""
   read row col
   valgrind --leak-check=full --error-exitcode=13 ./a.out tests/s${row}x${col}.in

elif [ "$type" = "b" ]; then
   ./a.out tests/s1.in

elif [ "$type" = "2x3" ]; then
   ./a.out tests/s2x3.in

elif [ "$type" = "2x2" ]; then
   ./a.out tests/s2x2n1.in
   ./a.out tests/s2x2n2.in
   ./a.out tests/s2x2n3.in
   ./a.out tests/s2x2n4.in

elif [ "$type" = "3x4" ]; then
   ./a.out tests/s3x4.in

elif [ "$type" = "4x4" ]; then
   ./a.out tests/s4x4.in

elif [ "$type" = "g" ]; then
   make clean
   gcc -pg -Wall -std=c89 -pedantic *.c 
   ./a.out tests/s1.in
   gprof a.out gmon.out | head -20
   make clean
   make

fi

done
