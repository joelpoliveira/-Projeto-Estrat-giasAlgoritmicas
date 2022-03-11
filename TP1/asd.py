from sys import stdin, stdout, setrecursionlimit
from time import time
#import pprint
#pp = pprint.PrettyPrinter(depth=4)

setrecursionlimit(2500)


def read_all():
    return stdin.readlines()


def readln():
    return stdin.readline().rstrip()


def outln(n="", end="\n"):
    return stdout.write(str(n) + end)


# Dicionário que mapeia um inteiro para uma Peça ->  [ rotação, rotações, flag_usada ]
index_to_array = {}

def create_piece(array, index, counts):
    # modos possíveis da matriz
    possibilities = [
        [[array[0], array[1]], [array[3], array[2]]],
        [[array[3], array[0]], [array[2], array[1]]],
        [[array[2], array[3]], [array[1], array[0]]],
        [[array[1], array[2]], [array[0], array[3]]],
    ]

    for i in range(4):
        counts[int(array[i])] = (counts[int(array[i])] + 1) % 2
        # rotação         #used
    piece = [0, possibilities, 0]
    index_to_array[index] = piece
    return index

def rotate(index, n):
    return index_to_array[index][1][n]


def create_board(r, c, first_index):
    board = [[-1] * c for i in range(r)] 
    board[0][0] = first_index
    # r, c, next_piece, board(r x c)
    board_all_data = [r, c, 1, board]
    return board_all_data


def match_bottom(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_numbers[0][0] == piece_now[1][0]) and (
        piece_numbers[0][1] == piece_now[1][1]
    )


def match_right(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_numbers[0][0] == piece_now[0][1]) and (
        piece_numbers[1][0] == piece_now[1][1]
    )


def row(index, n):
    piece = index_to_array[index]
    return " ".join(piece[1][piece[0]][n])


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

# devolve uma lista com os 4 possíveis encaixes, 1 por cada lado
def get_single_matches(index):
    piece = index_to_array[index][1][0]
    return (
        ((piece[0][0], piece[1][0]), 0),
        ((piece[1][0], piece[1][1]), 1),
        ((piece[1][1], piece[0][1]), 2),
        ((piece[0][1], piece[0][0]), 3),
    )


def get_double_matches(index):
    piece = index_to_array[index][1][0]
    return (
        ((piece[0][1], piece[0][0], piece[1][0]), 0),
        ((piece[0][0], piece[1][0], piece[1][1]), 1),
        ((piece[1][0], piece[1][1], piece[0][1]), 2),
        ((piece[1][1], piece[0][1], piece[0][0]), 3),
    )

def is_complete(board):
    return (board[2]) == (board[0] * board[1])

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
            return 1
        return 0

    if c == 0:
        if match_bottom(board[3][r - 1][c], piece_numbers):
            return 1
        return 0

    if match_bottom(board[3][r - 1][c], piece_numbers) and match_right(
        board[3][r][c - 1], piece_numbers
    ):
        return 1
    return 0


def pop(board):
    r, c = get_current(board)
    index = board[3][r][c]
    board[3][r][c] = -1
    board[2] -= 1
    return index


#devolve o par atual onde a nova peça tem que encaixar
def get_current_match(board):
    r,c = get_current(board)
    
    if c == board[1] - 1:
        piece = index_to_array[board[3][r][0]]
        piece = piece[1][piece[0]]
        return (piece[1][1], piece[1][0]), 1

    elif r==0:
        piece = index_to_array[board[3][r][c]]
        piece = piece[1][piece[0]]
        return (piece[0][1], piece[1][1]), 0

    else:
        p_up = index_to_array[board[3][r - 1][c + 1]]
        p_up = p_up[1][p_up[0]]

        p_left = index_to_array[board[3][r][c]]
        p_left = p_left[1][p_left[0]]
        return (p_up[1][1], p_up[1][0], p_left[1][1]), 0

def solve(board, pieces):
    if is_complete(board):
        return 1

    match_now, rot_type = get_current_match(board)
    try:
        for i in range( len(pieces[match_now] ) ):
            index, rot = pieces[match_now][i]
            rot = (rot+rot_type)%4
            piece_now = index_to_array[index]

            #print(index, rot)
            if piece_now[2] == 0: # If piece hasn't been used
                if piece_fits(board, piece_now[1][rot]):
                    piece_now[0] = rot # mark rotation
                    piece_now[2] = 1 # mark as used

                    insert(board, index) # insert it

                    result = solve(board, pieces)
                    if result:
                        return 1

                    piece_now[0] = 0 # Unmark rotation
                    piece_now[2] = 0 # Unmark used
                    pop(board) # Remove from board
    except:
        pass
    return 0


if __name__ == "__main__":
    
    for _ in range(int(readln())):
        N, R, C = list(map(int, readln().split()))
        
        index_to_array = {}
        pieces = {}
        counts = [0] * 1000

        first_index = create_piece(readln().split(), 0, counts)

        # Create Board
        board = create_board(R, C, first_index)
        
        # Pre process some stuff
        for __ in range(N - 1):
            index = create_piece( readln().split(), __ + 1, counts)

            for i, r in get_single_matches(index):
                try:
                    pieces[i].append((index,r))
                except:
                    pieces[i] = [(index, r)]
            for i, r in get_double_matches(index):
                try:
                    pieces[i].append((index,r))
                except:
                    pieces[i] = [(index,r)]

        start = time()
        if sum(counts) <= 4 and solve(board, pieces):
            outln(time() - start)
            print_board(board)
        else:
            #outln(time() - start)
            outln("impossible puzzle!")
