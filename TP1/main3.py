from sys import stdin, stdout, setrecursionlimit
from time import time
setrecursionlimit(2500)

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()
    
def outln(n = '', end = '\n'):
    return stdout.write(str(n) + end)

index_to_array = {}

def create_piece(array, counter):
    possibilities = [
                            [ [array[0], array[1] ], 
                              [array[3], array[2] ] ], 
                        
                            [ [array[3], array[0] ],
                              [array[2], array[1] ] ],

                            [ [array[2], array[3] ],
                              [array[1], array[0] ] ],

                            [ [array[1], array[2] ],
                              [array[0], array[3] ] ]
                        ]
    piece = [0, possibilities, 0]
    index_to_array[counter] = piece
    counter+=1
    return counter-1

def row(index, n):
    piece = index_to_array[index]
    return ' '.join(piece[1][piece[0]][n])
    
def print_row(piece, n):
    for i in range(2):
        outln(piece[1][piece[0]][n][i], end = " ")
    
def rotate(index, n):
    return index_to_array[index][1][n]

def match_up(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_now[0][0] == piece_numbers[1][0]) and (piece_now[0][1] == piece_numbers[1][1]) 

def match_left(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_now[0][0] == piece_numbers[0][1]) and (piece_now[1][0] == piece_numbers[1][1]) 
    
def match_bottom(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_numbers[0][0] == piece_now[1][0]) and (piece_numbers[0][1] == piece_now[1][1])

def match_right(index, piece_numbers):
    piece = index_to_array[index]
    piece_now = piece[1][piece[0]]
    return (piece_numbers[0][0] == piece_now[0][1]) and (piece_numbers[1][0] == piece_now[1][1])
 
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

def create_board(r,c, first_index):
    board = [[-1 for i in range(c)]for j in range(r)]
    board[0][0] = first_index
    board_all_data = [r,c,1,board]
    return board_all_data

def print_board(board):
    r = []
    for i in range(board[0]):
        for k in range(2):
            line = ""
            for j in range(board[1]):
                line += row(board[3][i][j], k) + "  "
            r.append( line.rstrip() )
        r.append("")
    outln('\n'.join(r), end = "")

"""def debug_outln(self):
    for i in range(self.rows):
        for j in range(2):
            for x in range(self.cols):
                if self.board[i][x] is not None:
                    self.board[i][x].print_row(j)
                else:
                    print("|_|", end = '')
                outln('', end = ' ')
            outln('\n') if j == self.rows - 1 and i != self.rows - 1 else outln()
"""
def get_next(board):
    row = board[2]//board[1]
    col = board[2]%board[1]

    return row,col

def get_current(board):
    row = (board[2] - 1)//board[1]
    col = (board[2] - 1)%board[1]

    return row,col

def insert(board, index):
    r,c = get_next(board)
    board[3][r][c] = index
    board[2] += 1

def piece_fits(board, piece_numbers):
    r,c = get_next(board)

    if r == 0:
        if match_right(board[3][r][c - 1], piece_numbers):
            return True
        return False

    if c == 0:
        if match_bottom(board[3][r - 1][c], piece_numbers):
            return True
        return False

    if match_bottom(board[3][r - 1][c], piece_numbers) and match_right(board[3][r][c-1], piece_numbers):
        return True
    return False
   
def is_complete(board):
    return (board[2])==(board[0]*board[1])
	
def pop(board):
    r,c = get_current(board)
    index = board[3][r][c]
    board[3][r][c] = -1
    board[2] -= 1
    return index

def get_current_piece(board):
    r,c = get_current(board)
    return board[3][r][c]

def get_current_to_search(board):
    r,c = get_current(board)
    if c==board[1]-1:
        return board[3][r][0]
    return board[3][r][c]

def resolve(board, subsets):
    #board.debug_outln()
    
    if is_complete(board):
        return True

    current = get_current_to_search(board)
    
    for i in subsets[current]:
        if index_to_array[i][2] == 0:
            for rot in range(4):
                if piece_fits(board, rotate(i, rot) ):
                    index_to_array[i][0] = rot
                    index_to_array[i][2] = 1
                    insert(board, i )

                    result = resolve(board, subsets)
                    if result:
                        return True
                    
                    index_to_array[i][0] = 0
                    index_to_array[i][2] = 0
                    pop(board)
    return False

if __name__ == "__main__":
    n = int(readln())

    for _ in range(n):
        N, R, C = list(map(int, readln().split()))

        first_index = create_piece(readln().split(), 0)

        # Create Board
        board = create_board(R, C, first_index)

        pieces_subset = {first_index : []}

        for __ in range(N-1):
            index = create_piece( readln().split(), __ + 1 ) 
            pieces_subset[index] = []

            for i in pieces_subset:
                if i!=index:
                    for rot in range(4):

                        if i!=first_index:
                            if any_match(i, rotate(index, rot)):
                                pieces_subset[i].append(index)
                                pieces_subset[index].append(i)
                                break

                        else:
                            if match_right(i, rotate(index, rot) ) or match_bottom( i, rotate(index, rot) ):
                                pieces_subset[i].append(index)
                                break
        #print(index_to_array)
        #start = time()
        if resolve(board, pieces_subset):
            #outln(time()-start)
            print_board(board)
        else:
            #outln(time()-start)
            outln("impossible puzzle!")
        