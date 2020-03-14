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

elif [ "$type" = "bin" ]; then
   echo bin
   ./sol bin 2>testerr.expect
   xxd -b bin.lzw > test.expect
   ./a.out bin 2>testerr.out
   xxd -b bin.lzw > test.out
   diff test.out test.expect | cat -e

   echo bin-1
   ./sol bin-1 2>testerr.expect
   xxd -b bin-1.lzw > test.expect
   ./a.out bin-1 2>testerr.out
   xxd -b bin-1.lzw > test.out
   diff test.out test.expect | cat -e

   echo bin+1
   ./sol bin+1 2>testerr.expect
   xxd -b bin+1.lzw > test.expect
   ./a.out bin+1 2>testerr.out
   xxd -b bin+1.lzw > test.out
   diff test.out test.expect | cat -e

elif [ "$type" = "b" ]; then
   echo "input filename:"
   read fname
   rm $fname.lzw
   rm test.out
   rm test.expect
   echo "recycle: 9-15"
   read rec
   ./sol -r$rec $fname 2>testerr.expect
   cp $fname.lzw test.expect
   ./a.out -r$rec $fname 2>testerr.out
   cp $fname.lzw test.out
   diff test.out test.expect | cat -e
   diff testerr.out testerr.expect | cat -e

elif [ "$type" = "bee" ]; then
   echo bee
   ./sol bee.txt 2>testerr.expect
   xxd -b bee.txt.lzw > test.expect
   ./a.out bee.txt 2>testerr.out
   xxd -b bee.txt.lzw > test.out
   diff test.out test.expect | cat -e
   diff testerr.out testerr.expect | cat -e

elif [ "$type" = "rec" ]; then
   echo rec1
   ./sol rec1
   xxd -b rec1.lzw > rec1.expect
   ./a.out rec1
   xxd -b rec1.lzw > rec1.out
   diff rec1.out rec1.expect | cat -e

   echo rec2
   ./sol rec2
   xxd -b rec2.lzw > rec2.expect
   ./a.out rec1
   xxd -b rec2.lzw > rec2.out
   diff rec2.out rec2.expect | cat -e

   echo rec1 -r9
   ./sol rec1 -r9
   xxd -b rec1.lzw > rec1.expect
   ./a.out rec1 -r9
   xxd -b rec1.lzw > rec1.out
   diff rec1.out rec1.expect | cat -e

   echo rec2 -r9
   ./sol rec2 -r9
   xxd -b rec2.lzw > rec2.expect
   ./a.out rec1 -r9
   xxd -b rec2.lzw > rec2.out
   diff rec2.out rec2.expect | cat -e

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

elif [ "$type" = "big" ]; then
   echo bigFile -r9
   ./sol -r9 bigFile.txt 2>testerr.expect
   xxd -b bigFile.txt.lzw > bigFile.expect
   ./a.out -r9 bigFile.txt 2>testerr.out
   xxd -b bigFile.txt.lzw > bigFile.out
   diff bigFile.out bigFile.expect | cat -e
   diff testerr.out testerr.expect

   echo bigFile -r15
   ./sol -r15 bigFile.txt 2>testerr.expect
   xxd -b bigFile.txt.lzw | head -9525 > bigFile.expect
   ./a.out -r15 bigFile.txt 2>testerr.out
   xxd -b bigFile.txt.lzw | head -9525 > bigFile.out
   diff bigFile.out bigFile.expect | cat -e
   diff testerr.out testerr.expect

elif [ "$type" = "v" ]; then
   valgrind --leak-check=full --error-exitcode=13 ./sol -r9 bigFile.txt
   valgrind --leak-check=full --error-exitcode=13 ./a.out -r9 bigFile.txt
   valgrind --leak-check=full --error-exitcode=13 ./sol -r15 bigFile.txt
   valgrind --leak-check=full --error-exitcode=13 ./a.out -r15 bigFile.txt

elif [ "$type" = "t" ]; then
   echo "bigFile -r9"
   echo "sol"
   time ./sol bigFile.txt -r9
   echo "a.out"
   time ./a.out bigFile.txt -r9
   echo "bigFile -r10"
   echo "sol"
   time ./sol bigFile.txt -r10
   echo "a.out"
   time ./a.out bigFile.txt -r10
   echo "bigFile -r11"
   echo "sol"
   time ./sol bigFile.txt -r11
   echo "a.out"
   time ./a.out bigFile.txt -r11
   echo "bigFile -r12"
   echo "sol"
   time ./sol bigFile.txt -r12
   echo "a.out"
   time ./a.out bigFile.txt -r12
   echo "bigFile -r13"
   echo "sol"
   time ./sol bigFile.txt -r13
   echo "a.out"
   time ./a.out bigFile.txt -r13
   echo "bigFile -r14"
   echo "sol"
   time ./sol bigFile.txt -r14
   echo "a.out"
   time ./a.out bigFile.txt -r14
   echo "bigFile -r15"
   echo "sol"
   time ./sol bigFile.txt -r15
   echo "a.out"
   time ./a.out bigFile.txt -r15

elif [ "$type" = "z" ]; then
   rm Project7.zip
   zip Project7.zip *.c *.h

elif [ "$type" = "h" ]; then
   handin kmammen-grader Project7 Project7.zip

else
   echo "usage: [ c:complexity | s:style | b:basic | v:valgrind | q:quit | z:zip | h:handin ]"

fi

done
