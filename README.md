# sudoku

This project features a sudoku solver written in C. C was chosen since a good portion of this code takes advantage
of pointer logic. Using Donald Knuth's Algorithm X and a dancing links data structure, we are able to solve this
exact cover problem. Since the algorithm and data structure are meant for exact cover problems in general, this
program can be used to solve those other problems, such as polynomino tiling and the n-queens problem.

* https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X
* https://en.wikipedia.org/wiki/Dancing_Links
* https://en.wikipedia.org/wiki/Exact_cover

![seppuku](images/sudoku_seppuku.png)

## unistd_io.h

Create a file of this name that includes io.h if running on windows or unistd.h if on linux

## logic

Given a list of coordinates for an exact cover matrix, solution(s) are found by finding a set of disjoint rows with coordinates that
span all columns.

Example from wikipedia:

        1   2   3   4   5   6   7
    A   X   .   .   X   .   .   X
    B   X   .   .   X   .   .   .
    C   .   .   .   X   X   .   X
    D   .   .   X   .   X   X   .
    E   .   X   X   .   .   X   X
    F   .   X   .   .   .   .   X

If the above is the input matrix, a solution would be the set of rows { B, D, F }:

        1   2   3   4   5   6   7
    B   X   .   .   X   .   .   .
    D   .   .   X   .   X   X   .
    F   .   X   .   .   .   .   X

As you can see, each column has exactly one X.

For sudokus, we can model them by seeing that for a board with:

    box dimensions = x & y,
    row, col, and box size = xy
    gridSize = xy*xy
    
There are 4 types of "constraints": numbers 1 through xy must be in a row, column, and box, and no more than one number can be in a
single cell. These constraints are what define the matrix's columns (4\*gridSize), and its rows represent the possible placement
of a number and the value of the number itself (xy\*gridSize).

Solving the matrix in this way allows us to easily find a solution for a sudoku puzzle.

## components

![dancing links](images/dancing_links.png)

### doubly and dance

The main data structure used here is the dancing link matrix ("dance"). It's a two dimensional linked "list" made up of
objects/structs ("doubly") that point to its neighboring doubly (up/down/left/right) and its row and column headers.
Each one stores its own matrix coordinate ("drow", "dcol"), and any other needed data. In the above example, each X would represent
a doubly in the matrix, and each doubly would point to the doubly directly to their left, right, above, and below. Having pointers
set up like this enables the program to perform common operations quickly. Since the matrix is usually sparse, traversing the matrix
takes minimal time, as traversal only stops by existing doubly.
The two most common operations is covering and uncovering a doubly.

These operations:

    doubly->next->prev = doubly->prev
    doubly->prev->next = doubly->next

cover a doubly (hides it from the rest of the matrix) without having to delete it, and

    doubly->next->prev = doubly
    doubly->prev->next = doubly

uncovers it.

In Algorithm X, you usually cover and uncover rows and columns at a time, but you don't need to store the location of each
covered doubly. If you covered doubly1, then covered doubly2 (doubly1's immediate neighbor) doubly1 still has a pointer to
doubly2. And so now you can cover as many consecutive doubly as you want, and you only need to keep track of a single doubly.

While you can take advantage of this fact, uncovering a group of consecutive doubly should be done carefully, and ideally
in the reverse order it was covered in, otherwise some doubly would point to the wrong doubly in the process of uncovering.

ex. cover and uncover b and c

starting with:

    a->next = b     b->prev = a
    b->next = c     c->prev = b
    c->next = d     d->prev = c

covering b:

    a->next = c     (b)->prev = a
    (b)->next = c   c->prev = a
    c->next = d     d->prev = c

covering c:

    a->next = d     (b)->prev = a
    (b)->next = c   (c)->prev = a
    (c)->next = d   d->prev = a

Everything is fine at this point if you want to perform some intermediate operation on the uncovered doubly a or d,
but if c is not uncovered first:

uncovering b:

    a->next = b     b->prev = a
    b->next = c     (c)->prev = b
    (c)->next = d   d->prev = a

As you can see, b->next shouldn't be pointing to c, which is still covered, but point to d. And d->prev shouldn't still
be pointing to a, but point to b. Now if you continue and finish the uncovering process:

uncovering d:

    a->next = b     b->prev = a
    b->next = c     c->prev = b
    c->next = d     d->prev = c

At this point, all doubly are pointing to where they should, even after uncovering in the wrong order. So it's fine to
uncover in any order, as long as you don't perform any operations on the doubly midway through the
uncovering process that could change the state of the pointers.

### root, column & row headers

Frequently referenced as "hcol" and "hrow", the column and row headers are also doubly objects, as well as the matrix root.
The headers store useful information, such as how many elements are in its row, or how many in its column. They are also
used to easily access the matrix's elements. The root is simply the doubly above all row headers and to the left of all
column headers, and is the typical entrance point to the matrix.

To tack onto the list of advantages using pointers has, you can easily check whether or not the matrix is empty by just
calling:

    if(root == root->right)
        # matrix is empty

Since covering the last element in a list will lead to this statement evaluating true, this simplifies a lot of things.
Due to how Algorithm X works, if there aren't any columns, there won't be any rows, and this check becomes simple.
(If a solution is found, the matrix will be empty like this)

Another thing to point out, since each column header doesn't have a meaningful row number, or a row header a meaningful
column number, that memory location is used to store the number of elements in its row/column.
Since there's no way to tell if that doubly is actually a header or just a doubly with incorrect coordinates if you assign
just that nunmber, rmax and cmax are added appropriately to those values. This allows the program to easily tell if a
doubly is a column header by calling

    if(doubly->drow > cmax)
        # doubly is an hcol

and can find the number of elements below it, which would just be

    num_elements_below = doubly->drow - cmax

note: an "empty" matrix just means all doubly in the matrix has been covered
note: rmax and cmax are the dimensions of the matrix
note: row headers aren't used for the computation itself, but are useful for printing and identifying solutions,
as well as initializing the matrix

### solTree

The solTree objects are used to store the solution(s) of the matrix. Each row in the set of a solution will be associated with
a solTree object. If a matrix has a single solution, the tree will just be a list, otherwise, it'll just be a normal tree with its
leaves signifying a solution. There's also no necessary order to the tree.
The tree grows along with the algorithm, since each call to Algorithm X is associated with a candidate matrix row.

### hide

The hide feature allows the program to cover a list of rows from the matrix before running Algorithm X. All sudoku puzzles of the
same dimension have the same initial matrix, but since they have different prefilled cells, different rows can be eliminated from
analysis. Initializing the matrix can be costly if multiple sudokus are to be solved, so this allows the program to initialize it
just once. This functionality can also be helpful in generating sudokus puzzles.

### heur & heurList

In Algorithm X, there are a list of columns to choose from, from which a heuristic can be used. One heuristic is choosing a column
with the least amount of elements underneath (nElements). In this design, each column is associated with a heur struct, and the heur
keeps track of nElements as well. Each heur is grouped together in a list with other heurs that have the same nElements.
heurList is a list of these list of heur. When a doubly is covered or uncovered, the doubly's column header's nElement
count is appropriately incremented or decremented and the heur it's linked to is moved from one heur list to another

        1   2   3   4   5   6   7
    A   X   .   .   X   .   .   X
    B   X   .   .   X   .   .   .
    C   .   .   .   X   X   .   X
    D   .   .   X   .   X   X   .
    E   .   X   X   .   .   X   X
    F   .   X   .   .   .   .   X
    
heurList:

    root - h(1) - h(2) - h(3) - h(4)
                   |      |      |
                  col1   col4   col7
                   |
                  col2
                   |
                  col3
                   |
                  col5
                   |
                  col6

This enables you to get a column header with minimal elements underneath in minimal time.

    while(heurHeader->next == heurHeader)
        min_hcol = heurHeader->next

The incrementing and decrementing operations are also in O(1) time.

Choosing a column with minimal elements, while a heuristic, doesn't guarantee a faster solve,
so for some cases, the program can be faster if it chooses a random column.

There is a heur macro that can be set on to tell the program to use heurs, otherwise if set off,
heuristic2() will find the column with min nElements in O(n) time, n = number of columns.

## process (sudoku)

The program takes in a file of a sudoku you want it to solve, and takes note of its dimensions, x and y.
It then creates a file that lists the coordinates of elements in the initial matrix. hideAllCells() then covers
the appropriate rows of the matrix according to the given clues in the sudoku. Algorithm X is run on the matrix,
creating a solTree in the process. unhideAllCells() uncovers the previously covered rows. Another
sudoku board with the same dimensions can also be run now without having to reinitialize the matrix.

To use this program to solve another exact cover problem, you can just swap out and replace some functions for it
to work.

## possible project paths to take

enable processing boards of higher dimensions, ex. 2x2x2, 2x3x4, 3x3x3x3\
figure out an efficient way to generate boards
write side program to enumerate polynominos

## data (sudoku)

estimated time to create a random fully populated board of varying sizes:\
(rows are chosen randomly)\
3x3: 0.05 sec\
4x4: 0.14 sec\
4x5: 0.26 sec\
5x5: 0.6 sec\
5x6: 18 sec\
6x6: 5 mins\
7x7: unknown / too long

estimated time to create a canonical fully populated board of varying sizes:\
(rows are not chosen randomly)\
3x3: 0.035 sec\
3x17: 12 sec\
3x18: 5 sec\
3x19: 55 sec\
4x4: 0.13 sec\
4x12: 2 min 30 sec\
5x5: 0.48 sec\
5x10: 2 sec\
5x11: 4 sec\
5x12: 6 sec\
5x13: 1 min 21 sec\
6x6: 0.9 sec\
6x7: 1.2 sec\
6x8: 1.4 sec\
6x9: 3 sec\
6x10: 4 min 45 sec\
7x7: 2.5 sec\
7x8: 12 sec\
7x9: 25 sec\
8x8: unknown / too long