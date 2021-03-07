
fact = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800]

# to solve a custom exact cover problem, you have to write your own
# initMatrixFile() function. it will have to return the dimensions
# of the matrix (rmax, cmax) and change MATRIX_FILE to a custom file
# location. once AlgorithmX is done running, solTrie will have the rows
# of your solution stored. you can use printSolutions() in auxil.c to
# print out all your solutions

def initMatrixFile_Sudoku(x, y):
    xy = x*y
    gridSize = xy*xy
    matrixFile = "dance/ds1_%dx%d.txt" % (y, x)
    f = open(matrixFile, "w")

    for igrid in range(gridSize):
        sr = igrid // xy
        sc = igrid % xy
        sb = (sr // y) * y + sc // x

        for inum in range(xy):
            mrow = igrid * xy + inum

            f.write("%d %d\n" % (mrow, igrid))
            f.write("%d %d\n" % (mrow, inum + sr * xy + gridSize))
            f.write("%d %d\n" % (mrow, inum + sc * xy + gridSize * 2))
            f.write("%d %d\n" % (mrow, inum + sb * xy + gridSize * 3))

    rmax = xy*gridSize
    cmax = 4*gridSize

    f.close()
    return rmax, cmax

# first xy columns are for which number is associated with the layout
# next xy*xy columns refer to the layout's grid cell positions
# filename is ds2_(y)x(x), s2 meaning the second matrix setup for sudoku
def initMatrixFile_Sudoku2(x, y):
    # anything above 3x3 creates a ridiculously large matrixFile
    xy = x*y
    fileName = "dance/ds2_%dx%d.txt" % (y, x)
    f = open(fileName, "w")

    xlist = [i for i in range(x)]
    ylist = [i for i in range(y)]
    xplist = []
    yplist = []

    # get all permutations of _list into _plist
    heap(xplist, x, xlist)
    heap(yplist, y, ylist)

    # get all permutations of y number of xplists and x number of yplists
    xfy_powlist = iter(y, [], xplist, [])
    yfx_powlist = iter(x, [], yplist, [])

    print(len(xfy_powlist))
    print(len(yfx_powlist))

    coords_list = []
    for xfy_perms in xfy_powlist:
        for yfx_perms in yfx_powlist:
            coords = []
            row_coords = [[-1 for i in range(y)] for j in range(x)]
            col_coords = [[-1 for i in range(y)] for j in range(x)]

            for i, xfy_perm in enumerate(xfy_perms):
                for j, xfy in enumerate(xfy_perm):
                    row_coords[xfy][i] = j

            for i, yfx_perm in enumerate(yfx_perms):
                for j, yfx in enumerate(yfx_perm):
                    col_coords[i][yfx] = j

            for brow in range(x):
                for bcol in range(y):
                    row = brow * y
                    col = bcol * x
                    row += col_coords[brow][bcol]
                    col += row_coords[brow][bcol]

                    coord = row * xy + col
                    coords.append(coord)

            #printLayout(coords, x, y)

            coords_list.append(coords)

    rmax = xy*len(coords_list)
    cmax = xy + xy*xy

    mrow = 0
    for coords in coords_list:
        for j in range(xy):
            f.write("%d %d\n" % (mrow, j))
            for coord in coords:
                bcol = coord + xy
                f.write("%d %d\n" % (mrow, bcol))

            mrow += 1

    f.close()

    print("rmax: " + repr(rmax))
    print("cmax: " + repr(cmax))


    print("num_layouts: " + repr(len(coords_list)))

    return rmax, cmax

def printLayout(coords, x, y):
    xy = x * y
    coords.sort()

    inext = 0
    for row in range(xy):
        if (row % y == 0):
            print("")
        for col in range(xy):
            if (col % x == 0):
                print(" ", end="")
            if inext < xy and coords[inext] == row * xy + col:
                print("  X", end="")
                inext += 1
            else:
                print("  _", end="")
        print("")
    print("")

def iter(pow, perms, plist, powlist):
    if(pow == 0):
        powlist.append(perms.copy())
        return powlist

    for p in plist:
        new_perms = perms.copy()
        new_perms.append(p)
        powlist = iter(pow-1, new_perms, plist, powlist)

    return powlist

def heap(plist, k, A):
    if(k == 1):
        plist.append(A.copy())
        return plist

    plist = heap(plist, k - 1, A)

    for i in range(k-1):
        if(k % 2 == 0):
            swap(A, i, k-1)
        else:
            swap(A, 0, k-1)
        plist = heap(plist, k - 1, A)

    return plist

def swap(A, i, j):
    temp = A[i]
    A[i] = A[j]
    A[j] = temp

"""
    each matrix row corresponds to a position on the board
    matrix columns correspond to ranks (rows), files (columns),
    and diagonals in that order
    
    there are 4n-6 number of non trivial diagonals for each board
    
    diagonals (n = 4) denoted as such:
    
    diag1:
    X 0 1 2
    0 1 2 3
    1 2 3 4
    2 3 4 X
    
    diag2:
    7 8 9 X
    6 7 8 9
    5 6 7 8
    X 5 6 7
    
    -1 will be used to denote diagonals to be ignored
    
    note that the diagonals do not necessarily need a queen on it
    to make a valid board, but can have no more than 1 queen on any,
    so these columns are considered "secondary" and have to be adjusted
    for in Algorithm X
    
"""
def initMatrixFile_NQueens(n):
    n2 = pow(n, 2)
    diag2_start_index = 2*n - 3

    fileName = "dance/dq_%d.txt" % (n)
    f = open(fileName, "w")

    for mrow in range(n2):
        rank = mrow % n
        file = mrow // n
        if(rank == 0 and file == 0) or (rank == n-1 and file == n-1):
            diag1 = -1
        else:
            diag1 = rank + file - 1
        if(rank == 0 and file == n-1) or (rank == n-1 and file == 0):
            diag2 = -1
        else:
            diag2 = diag2_start_index + (rank + (n - file) - 1) - 1

        f.write("%d %d\n" % (mrow, rank))
        f.write("%d %d\n" % (mrow, file + n))
        if(diag1 != -1):
            f.write("%d %d\n" % (mrow, diag1 + n*2))
        if(diag2 != -1):
            f.write("%d %d\n" % (mrow, diag2 + n*2))

    f.close()


def main():
    #initMatrixFile_Sudoku(x=2, y=2)
    #initMatrixFile_Sudoku2(x=2, y=2)
    for i in range(21):
        initMatrixFile_NQueens(n=i)

if __name__ == "__main__":
    main()
