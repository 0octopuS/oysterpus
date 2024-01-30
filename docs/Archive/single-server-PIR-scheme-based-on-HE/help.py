import math


def squish(row, col, delta, basis):
    m = [[i*col + j for j in range(col)] for i in range(row)]
    new_col = math.ceil((col + delta-1)/delta)
    new_m = [[0 for j in range(new_col)] for i in range(row)]

    # new_m = [0 for i in range(row*new_col)]
    print("m     = ", m)
    print("new_m = ", new_m)
    print("new_col = ", new_col)

    for i in range(row):
        for j in range(col):
            for k in range(delta):
                print(">>> ", i, j, k)
                if (delta * j + k < col):
                    val = m[i][delta * j + k]
                    print("    val=", val)
                    print("    k*basis=", k*basis)
                    print("    index=", i*col + j)
                    new_m[i][j] += (val << (k*basis))
    print("new_m = ", new_m)
    return new_m


def unsquish(col, delta, basis, new_M):
    row = len(new_M)
    new_col = len(new_M[0])
    mask = (1 << basis) - 1
    m = [[0 for j in range(col)] for i in range(row)]

    for i in range(row):
        for j in range(new_col):
            for k in range(delta):
                if (delta * j + k < col):
                    m[i][j*delta + k] = new_M[i][j] >> (k*basis) & mask

    print("m = ", m)


m = squish(5, 4, 3, 10)
unsquish(4, 3, 10, m)
