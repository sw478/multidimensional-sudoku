#!/bin/bash
make clean
make

while true; do

echo "what type of test:"
read type

if [ "$type" = "q" ]; then
   exit 1

elif [ "$type" = "c" ]; then
   complexity -t0 -s1 *.c

elif [ "$type" = "s" ]; then
   ~kmammen-grader/bin/styleCheckC *.c

elif [ "$type" = "gprof" ]; then
   make clean
   gcc -pg -Wall -std=c89 -pedantic *.c 
   ./a.out -r9 bigFile.txt
   gprof a.out gmon.out | head -20
   make clean
   gcc -pg -Wall -std=c89 -pedantic *.c 
   ./a.out -r15 bigFile.txt
   gprof a.out gmon.out | head -20
   make clean
   make

elif [ "$type" = "t" ]; then
   echo "bigFile -r9"
   echo "sol"
   time ./sol bigFile.txt -r9
   echo "a.out"
   time ./a.out bigFile.txt -r9

else
   echo "usage: [ c:complexity | s:style | q:quit ]"

fi

done
