# multidimensional sudoku
[![HitCount](http://hits.dwyl.com/sw478/multidimensional-sudoku.svg)](http://hits.dwyl.com/sw478/multidimensional-sudoku)

This project is based off of the main sudoku project, as most of the code is reusable. It generalizes the ideas of a traditional 9x9 sudoku to sudokus of any dimension, of any size.

## general notes

* Use "test.sh" to run everything.
* Sample sudokus are in /samples.
* DLX matrix coordinate files are in /dance.
* sudokuFile set to "sudokuFile.txt".
* Using [zchaff](https://www.princeton.edu/~chaff/zchaff.html#:~:text=zChaff%20is%20an%20implementation%20of,variables%20and%2010%20million%20clauses.) for bSat. bSat usage has only been loosely integrated for testing.
* The generator can write to the sudokuFile for the solver, or you can replace the sudokuFile with your own sudoku puzzle to solve.
* Heur, Hide, and SolTree from the last project can be used again in this project, not much was affected by the changes from generalizing sudokus.

## usage (test.sh)

Use "b:basic". Options:
* Solve: solve sudoku located in sudokuFile. If sudoku doesn't exist, generate one first, or copy a sample puzzle from /samples into sudokuFile.
* Generate: generate sudoku, input is just container dimensions.
* Enumerate: enumerate all sudokus of a given dimensions. Don't expect this finish on anything other than small sudokus (largest: [2, 2], [3, 2]). Meant for analyzing all 288 [2, 2] sudokus.
* bSat: create a dimacs file for inputed dimensions, then run zchaff on dimacs file. Doesn't do anything other than spit out SAT/UNSAT. Plan to expand on this functionality.

## process (dancing links)

### solver

* The program reads from a file containing the sudoku (sudokuFile) to solve,
* as well as a file (matrixFile) that contains coordinates for the dancing links matrix.
* If the correct matrixFile does not exist yet, matrixFileCreator.py can be used to create it.
* The generator can write to the sudokuFile for the solver to solve.
* Algorithm X is run on the matrix.
* A solution tree is created, identifying the solution(s).

### generator

* The program reads from a file (matrixFile) that contains coordinates for the dancing links matrix.
* If the correct matrixFile does not exist yet, matrixFileCreator.py can be used to create it.
* Algorithm X is run on the matrix to pick a single random solution (full sudoku).
* Hide is initialized with the solved board.
* Generate() is run to create a puzzle (partial sudoku) with the solved board as the only solution.
* Newly created puzzle is saved in sudokuFile.

## multidimensional generalization

A traditional sudoku is typically called a "9x9" sudoku, but could also be denoted as a [3, 3] sudoku. The [] notation describes the dimensions of a single "box" in the sudoku, and the entire sudoku's dimensions can be constrained by this alone. The length of the sudoku along each dimensions is the product of all of its box dimensions. In the "9x9" case, the length and width of one box is 3 and 3 respectively, and the length and width of the entire sudoku is 3x3 = 9. Since the box dimensions constrain the sudoku's dimensions, the process of defining the sudoku's dimensions can be said to start from the given box dimensions. The first box's shape is duplicated and stacked against each other in each dimension until the desired sudoku shape is reached.

### terminology
To make this easier to communicate, I have come up with some terminology that is specific to this project. In a traditional sudoku, you have rows, columns, and boxes. In a generalized multidimensional sudoku, the rows and columns are called "span" and boxes are called "containers". So for a traditional "9x9" sudoku, there are 2 types of span for the 2 dimensions (row, col) each 9 cells long, and the containers are also 9 cells in size.

### n
This is the number of dimension of the sudoku. From the [] notation, this is the length of the list.

### containerSize

This is the size of the containers, and the length of all span, units being number of cells. This number is calculated as the product of all container dimensions. In a traditional [3, 3] sudoku, the containerSize would be 3x3 = 9. Another example: a [4, 3, 2, 1] sudoku has a containerSize of 4x3x2x1 = 24.

### sudokuSize

This is the size of the entire multidimensional sudoku, units being number of cells. This is calculated as: containerSize ^ n. Note: It should be clear that a sudoku with dimensions [3, 3] is different from a sudoku with dimensions [3, 3, 1], as the first will have 9^2 = 81 cells while the second will have 9^3 = 729 cells.

### span and containers

There are n number of span types (row, col, etc.) each of containerSize length. In the diagrams below, the span types are differentiated using "span[i]", i being the dimension index number. There are (sudokuSize / containerSize) number of containers (81 / 9 = 9). This numContainers is also the number of span of a span type. Each set of span of a span type, and the set of all containers partition the sudoku.

## matrix

For the dancing links data structure, there is a 2D matrix.

Matrix rows describe the possible placements of individual number symbols in the sudoku, as well as the possible values it can have, so the total number of rows is calculated as: sudokuSize * containerSize.

Matrix columns are the constraints following the rules of sudoku. The number of columns for each constraint in parentheses.
* No more than one number can be in a cell. (sudokuSize)
* No more than one of each value in each container (sudokuSize)
* No more than one of each value in each span (sudokuSize * n)

## bSat

Another method besides dancing links is boolean sat. Although not fully implemented yet, it should yield faster results in generating these sudoku, as they can get very large quickly. The speed of solving these sudoku with bSat compared to dancing links is unknown, and dancing links has been fast enough so far.

## diagrams

A 3x2x1 sudoku layout:

![sudoku_3_2_1](images/msudoku_3_2_1.png)

A sample 3x2x1 puzzle and its solution:

![sudoku_3_2_1](images/msudoku_3_2_1_puzzle.png)

(This specific puzzle can be found in sudokuFile_example.txt)

A 2x2x2 sudoku layout:

![sudoku_2_2_2](images/msudoku_2_2_2.png)

For higher dimensions, an orthographic rhombic representation can be useful to visualize all possible 2D surfaces of a sudoku as an n-cube.

A 4x3x2x1 sudoku layout:

![sudoku_4_3_2_1](images/msudoku_4_3_2_1.png)