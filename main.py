
fact = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800]

def initMatrixFileSudoku(x, y):
    xy = x*y
    gridSize = xy*xy
    fileName = "dance/ds1_%dx%d.txt" % (y, x)
    f = open(fileName, "w")

    for igrid in range(gridSize):
        sr = igrid // xy
        sc = igrid % xy
        sb = (sr // y) * y + sc // x

        for inum in range(xy):
            icol = []
            irow = igrid * xy + inum
            icol.append(igrid)
            icol.append(inum + sr*xy + gridSize)
            icol.append(inum + sc*xy + gridSize * 2)
            icol.append(inum + sb*xy + gridSize * 3)

            for i in range(4):
                f.write("%d %d\n" % (irow, icol[i]))

    rmax = xy*gridSize
    cmax = 4*gridSize

    f.close()
    return rmax, cmax

# first xy columns are for which number is associated with the layout
# next xy*xy columns refer to the layout's grid cell positions
def initMatrixFileSudoku2(x, y):
    xy = x*y
    fileName = "dance/ds2_%dx%d.txt" % (y, x)
    f = open(fileName, "w")

    xlist = [i for i in range(x)]
    ylist = [i for i in range(y)]
    xplist = []
    yplist = []

    heap(xplist, x, xlist)
    heap(yplist, y, ylist)

    xfy_powlist = iter(y, [], xplist, [])
    yfx_powlist = iter(x, [], yplist, [])

    print(len(xfy_powlist))
    print(len(yfx_powlist))

    print(len(xfy_powlist)*len(yfx_powlist))

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

            for irow in range(x):
                for icol in range(y):
                    row = irow * y
                    col = icol * x
                    row += col_coords[irow][icol]
                    col += row_coords[irow][icol]

                    loc = row * xy + col
                    coords.append(loc)

            coords_list.append(coords)

    rmax = len(coords_list)
    cmax = xy*xy

    irow = 0
    for coords in coords_list:
        print(coords)
        for j in range(xy):
            f.write("%d %d\n" % (irow, j))
            for coord in coords:
                icol = coord + xy
                f.write("%d %d\n" % (irow, icol))

            irow += 1

    f.close()
    return rmax, cmax

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

def main():
    #initMatrixFileSudoku(x=3, y=3)
    initMatrixFileSudoku2(x=2, y=3)

if __name__ == "__main__":
    main()
