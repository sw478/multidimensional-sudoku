# multidimensional sudoku
[![HitCount](http://hits.dwyl.com/sw478/multidimensional-sudoku.svg)](http://hits.dwyl.com/sw478/multidimensional-sudoku)

This project is based off of the main sudoku project, as most of the code is the same. It generalizes the ideas of a traditional 9x9 sudoku to any sudoku of any dimension, of any length.

## general notes

* In this project, sudokuFile is set to "sudokuFile.txt".
* The generator can write to the sudokuFile for the solver, or you can input your own sudoku puzzle to solve.
* Heur, Hide, and SolTree can be used again in this project, not much was affected by the changes in processing sudokus.

## process

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
* Algorithm X is run on the matrix to pick a single random solution.
* Hide is initialized with the solved board.
* Generate() is run to create a puzzle with the solved board as the only solution.
* Newly created puzzle is saved in sudokuFile.

## explanation

A traditional sudoku is typically called a 9x9 sudoku, but to be more precise, it can be denoted as a (3x3)^2 sudoku. The reason for this is that (3x3) adequately describes the sudoku's box dimension, which is a key feature of sudokus that differenciate them from latin squares. The rows and columns span the two dimensions of the board, and the boxes divide up the board in both (all) dimensions.

In a traditional sudoku, you have rows, columns, and boxes. In a generalized sudoku, the rows and columns will be called "span" and boxes will be called "containers". The number of dimensions (n) in the sudoku is 2. The dimensions specified are the box dimensions, so in this case, it's called a [3, 3] sudoku, not 9x9.

### containerSize

This is the size of the containers, as well as the length of all span. (In a traditional sudoku, containerSize would be 9). This number is calculated as the product of all dimensions (3x3 = 9).

### sudokuSize

This is the size of the sudoku, which is calculated as: containerSize ^ n (9^2 = 81). It should be clear that a sudoku with dimensions [3, 3] is different from a sudoku with dimensions [3, 3, 1], as the first will have 81 cells while the second will have 729 cells.

### span and containers

There are n number of span types (row, col, etc.) each of containerSize length and there are (sudokuSize / containerSize) number of containers (81 / 9 = 9).

## matrix

Matrix rows describe the possible placements of individual numbers in the sudoku, as well as the possible values it can have, so number of rows is: (sudokuSize*containerSize).

Matrix columns are the constraints: (number of columns for each constraint in parentheses)
* No more than one number can be in a cell. (sudokuSize)
* No more than one of each value in each container (sudokuSize)
* No more than one of each value in each span (sudokuSize * n)

## diagrams

A 3x2x1 sudoku layout:

![sudoku_3_2_1](images/msudoku_3_2_1.png)

A 3x2x1 puzzle and its solution:

![sudoku_3_2_1](images/msudoku_3_2_1_puzzle.png)

(This specific puzzle can be found in sudokuFile_example.txt)

A 2x2x2 sudoku layout:

![sudoku_2_2_2](images/msudoku_2_2_2.png)

For higher dimensions, an orthographic rhombic representation can be useful to visualize all possible 2D surfaces of a sudoku as an n-cube.

A 4x3x2x1 sudoku layout:

![sudoku_4_3_2_1](images/msudoku_4_3_2_1.png)