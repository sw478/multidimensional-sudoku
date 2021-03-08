#!/bin/bash
USAGE="usage: [ b:basic | m:make | q:quit | t:time]"

while true; do

echo
echo "what type of test:"
echo $USAGE
read type

if [ "$type" = "q" ]; then
   exit 0

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
   echo "mode? \"s\" to solve, \"g\" to generate"
   read mode
   echo "dimension? \"row\" \"col\""
   read row col
   ./a.out ${mode} tests/${mode}/${row}x${col}.in
   gprof a.out gmon.out | head -25
   make clean
   make

elif [ "$type" = "t" ] || [ "$type" = "v" ] || [ "$type" = "b" ] || [ "$type" = "e" ]; then
   echo "problem: \"s\" for sudoku \"s2\" for sudoku2 \"q\" for n queens"
   read problem

if [ "$problem" = "s" ] || [ "$problem" = "s2" ]; then
   echo "mode? \"s\" to solve, \"g\" to generate"
   read mode
   echo "dimension? \"row\" \"col\""
   read row col

if [ "$type" = "t" ]; then
   time ./a.out ${problem} ${mode} tests/${mode}/${row}x${col}.in

elif [ "$type" = "v" ]; then
   valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out ${problem} ${mode} tests/${mode}/${row}x${col}.in

elif [ "$type" = "b" ]; then
   make
   ./a.out ${problem} ${mode} tests/${mode}/${row}x${col}.in

elif [ "$type" = "e" ]; then
   make
   ./a.out ${problem} ${mode} tests/${mode}/${row}x${col}emp.in

fi

elif [ "$problem" = "q" ]; then
   echo "n?"
   read n

if [ "$type" = "t" ]; then
   time ./a.out ${problem} ${n}

elif [ "$type" = "v" ]; then
   valgrind --leak-check=full --error-exitcode=13 --track-origins=yes ./a.out ${problem} ${n}

elif [ "$type" = "b" ]; then
   make
   ./a.out ${problem} ${n}

elif [ "$type" = "e" ]; then
   make
   ./a.out ${problem} ${n}

fi

fi

fi

done
