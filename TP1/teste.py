from sys import stdin, stdout, setrecursionlimit
from time import time

setrecursionlimit(2500)


def read_all():
    return stdin.readlines()


def readln():
    return stdin.readline().rstrip()


def outln(n="", end="\n"):
    return stdout.write(str(n) + end)


piece_rotations = {}
piece_data = {}

def create_piece(array, counter):
    possibilities = [
                        [ [array[0], array[1]], 
                        [array[3], array[2]] ],
                        
                        [ [array[3], array[0]], 
                        [array[2], array[1]] ],

                        [ [array[2], array[3]], 
                        [array[1], array[0]] ],

                        [ [array[1], array[2]], 
                        [array[0], array[3]] ],
    ]
    piece_rotations[counter] = possibilities
    piece_data[counter] = [0, 0] #rotation and used flag
    return counter


def row(index, n):
    piece = piece_rotations[index]
    rot = piece_data[index][0]

    return " ".join(piece[rot][n])


def print_row(piece, n):
    piece = piece_rotations[piece]
    rot = piece_data[piece][0]
    outln(piece[1][rot][n][0], end=" ")
    outln(piece[1][rot][n][1], end=" ")


def rotate(index, n):
    return piece_rotations[index][n]


def match_up(index, piece_numbers):
    rot = piece_data[index][0]
    piece_now = piece_rotations[index][rot]
    return (piece_now[0][0] == piece_numbers[1][0]) and (
        piece_now[0][1] == piece_numbers[1][1]
    )


def match_left(index, piece_numbers):
    rot = piece_data[index][0]
    piece_now = piece_rotations[index][rot]
    return (piece_now[0][0] == piece_numbers[0][1]) and (
        piece_now[1][0] == piece_numbers[1][1]
    )


def match_bottom(index, piece_numbers):
    rot = piece_data[index][0]
    piece_now = piece_rotations[index][rot]
    return (piece_numbers[0][0] == piece_now[1][0]) and (
        piece_numbers[0][1] == piece_now[1][1]
    )


def match_right(index, piece_numbers):
    rot = piece_data[index][0]
    piece_now = piece_rotations[index][rot]
    return (piece_numbers[0][0] == piece_now[0][1]) and (
        piece_numbers[1][0] == piece_now[1][1]
    )


def any_match(index, piece_numbers):
    if match_bottom(index, piece_numbers):
        return True
    if match_right(index, piece_numbers):
        return True
    if match_up(index, piece_numbers):
        return True
    if match_left(index, piece_numbers):
        return True
    return False


def create_board(r, c, first_index):
    board = [[-1 for i in range(c)] for j in range(r)]
    board[0][0] = first_index
    board_all_data = [r, c, 1, board]
    return board_all_data


def print_board(board):
    r = []
    for i in range(board[0]):
        for k in range(2):
            line = ""
            for j in range(board[1]):
                line += row(board[3][i][j], k) + "  "
            r.append(line.rstrip())
        r.append("")
    outln("\n".join(r), end="")


def debug_outln(board):
    for i in range(board[0]):
        for j in range(2):
            for x in range(board[1]):
                if board[3][i][x] != -1:
                    print_row(board[3][i][x], j)
                else:
                    print("|_|", end = '')
                outln('', end = ' ')
            outln('\n') if j == board[0] - 1 and i != board[1] - 1 else outln()



def get_next(board):
    row = board[2] // board[1]
    col = board[2] % board[1]

    return row, col


def get_current(board):
    row = (board[2] - 1) // board[1]
    col = (board[2] - 1) % board[1]

    return row, col


def insert(board, index):
    r, c = get_next(board)
    board[3][r][c] = index
    board[2] += 1


def piece_fits(board, piece_numbers):
    r, c = get_next(board)

    if r == 0:
        if match_right(board[3][r][c - 1], piece_numbers):
            return True
        return False

    if c == 0:
        if match_bottom(board[3][r - 1][c], piece_numbers):
            return True
        return False

    if match_bottom(board[3][r - 1][c], piece_numbers) and match_right(
        board[3][r][c - 1], piece_numbers
    ):
        return True
    return False


def is_complete(board):
    return (board[2]) == (board[0] * board[1])


def pop(board):
    r, c = get_current(board)
    index = board[3][r][c]
    board[3][r][c] = -1
    board[2] -= 1
    return index


def get_current_piece(board):
    r, c = get_current(board)
    return board[3][r][c]


def get_current_to_search(board):
    r, c = get_current(board)

    if c == board[1] - 1:
        rot = piece_data[board[3][r][0]][0]
        return board[3][r][0], (3 - rot)

    rot = piece_data[board[3][r][c]][0]
    return board[3][r][c], rot

def resolve(board, subsets):
    #debug_outln(board)
    if is_complete(board):
        return True

    current, match_index = get_current_to_search(board)
    #print(f"current = {current}, match = {match_index}")
    for i in subsets[current][match_index]:
        if piece_data[i][1] == 0:
            for rot in range(4):
                if piece_fits(board, rotate(i, rot)):
                    piece_data[i][0] = rot
                    piece_data[i][1] = 1
                    insert(board, i)
                    
                    result = resolve(board, subsets)
                    if result:
                        return True

                    piece_data[i][0] = 0    
                    piece_data[i][1] = 0
                    pop(board)

    return False


if __name__ == "__main__":
    n = int(readln())

    for _ in range(n):
        N, R, C = list(map(int, readln().split()))

        first_index = create_piece(readln().split(), 0)

        # Create Board
        board = create_board(R, C, first_index)

        pieces_subset = {first_index: [[], None, None, []]}
        x = {0: [0, 1, 2, 3], 1: [1, 2, 3, 0], 2: [2, 3, 0, 1], 3: [3, 0, 1, 2]}

        for __ in range(N - 1):
            index = create_piece(readln().split(), __ + 1)
            pieces_subset[index] = [[], [], [], []]
            
            for i in pieces_subset:
                if i != index:
                    for rot in range(4):

                        if i != first_index:
                            if match_bottom(i, rotate(index, rot)):
                                pieces_subset[i][3].append(index)
                                pieces_subset[index][x[rot][1]].append(i)

                            if match_right(i, rotate(index, rot)):
                                pieces_subset[i][0].append(index)
                                pieces_subset[index][x[rot][2]].append(i)

                            if match_up(i, rotate(index, rot)):
                                pieces_subset[i][1].append(index)
                                pieces_subset[index][x[rot][3]].append(i)

                            if match_left(i, rotate(index, rot)):
                                pieces_subset[i][2].append(index)
                                pieces_subset[index][x[rot][0]].append(i)

                        else:
                            if match_bottom(i, rotate(index, rot)):
                                pieces_subset[i][3].append(index)

                            if match_right(i, rotate(index, rot)):
                                pieces_subset[i][0].append(index)
        #print(pieces_subset)
        # print(index_to_array[0])
        #for elem in pieces_subset:
        #    print(pieces_subset[elem])
        #start = time()
        if resolve(board, pieces_subset):
            #outln(time()-start)
            print_board(board)
        else:
            #outln(time()-start)
            outln("impossible puzzle!")
