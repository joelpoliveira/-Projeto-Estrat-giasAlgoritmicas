from sys import stdin, stdout, setrecursionlimit
from time import time
setrecursionlimit(2500)

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()
    
def outln(n = '', end = '\n'):
    return stdout.write(str(n) + end)
    
class Piece:    
    def __init__(self, array):
        self.numbers = [
                            [ [array[0], array[1] ], 
                              [array[3], array[2] ] ], 
                        
                            [ [array[3], array[0] ],
                              [array[2], array[1] ] ],

                            [ [array[2], array[3] ],
                              [array[1], array[0] ] ],

                            [ [array[1], array[2] ],
                              [array[0], array[3] ] ]
                        ]
        self.now = 0
        #print("Created: ", self.numbers)
    
    def row(self, n):
        return ' '.join(self.numbers[self.now][n])
    
    def print_row(self, n):
        for i in range(2):
            outln(self.numbers[self.now][n][i], end = " ")
    
    def rotate(self, n):
        return self.numbers[n%4]

    def match_up(self, piece_numbers):
        return (self.numbers[self.now][0][0] == piece_numbers[1][0]) and (self.numbers[self.now][0][1] == piece_numbers[1][1]) 

    def match_left(self, piece_numbers):
        return (self.numbers[self.now][0][0] == piece_numbers[0][1]) and (self.numbers[self.now][1][0] == piece_numbers[1][1]) 

    def copy(self):
        return Piece([self.numbers[0][0], self.numbers[0][1], self.numbers[1][1], self.numbers[1][0]])
    
    def match_bottom(self, piece_numbers):
        return (piece_numbers[0][0] == self.numbers[self.now][1][0]) and (piece_numbers[0][1] == self.numbers[self.now][1][1])

    def match_right(self, piece_numbers):
        return (piece_numbers[0][0] == self.numbers[self.now][0][1]) and (piece_numbers[1][0] == self.numbers[self.now][1][1])
 
    def any_match(self, piece_numbers):
        if self.match_bottom(piece_numbers):
            return True
        if self.match_right(piece_numbers):
            return True
        if self.match_up(piece_numbers):
            return True
        if self.match_left(piece_numbers):
            return True
        return False

class Board:    
    def __init__(self, rows, cols, first_piece):
        self.board = [[None for i in range(cols)] for j in range(rows)]
        self.board[0][0] = first_piece
        self.rows = rows
        self.cols = cols
        self.next_empty = 1
        self.orientation = cols<=rows

    def __str__(self):
        r = []
        for i in range(self.rows):
            for k in range(2):
                line = ""
                for j in range(self.cols):
                    line += self.board[i][j].row(k) + "  "
                r.append( line.rstrip() )
            r.append("")
        return '\n'.join(r)

    def debug_outln(self):
        for i in range(self.rows):
            for j in range(2):
                for x in range(self.cols):
                    if self.board[i][x] is not None:
                        self.board[i][x].print_row(j)
                    else:
                        print("|_|", end = '')
                    outln('', end = ' ')
                outln('\n') if j == self.rows - 1 and i != self.rows - 1 else outln()

    def get_next(self):
        row = self.next_empty//self.cols
        col = self.next_empty%self.cols

        return row,col

    def get_current(self):
        row = (self.next_empty - 1)//self.cols
        col = (self.next_empty - 1)%self.cols

        return row,col

    def insert(self, Piece):
        r,c = self.get_next()
        self.board[r][c] = Piece
        self.next_empty+=1

    def piece_fits(self, piece_numbers):
        r,c = self.get_next()

        if r == 0:
            if self.board[r][c-1].match_right(piece_numbers):
                return True
            return False

        if c == 0:
            if self.board[r - 1][c].match_bottom(piece_numbers):
                return True
            return False

        if self.board[r - 1][c].match_bottom(piece_numbers) and self.board[r][c - 1].match_right(piece_numbers):
            return True
        return False
   
    def is_complete(self):
       return (self.next_empty)==(self.rows*self.cols)
	
    def pop(self):
        r,c = self.get_current()
        piece = self.board[r][c]
        self.board[r][c] = None
        self.next_empty-=1
        return piece

    def get_current_piece(self):
        r,c = self.get_current()
        return self.board[r][c]

    def get_current_to_search(self):
        r,c = self.get_current()
        if c==self.cols-1:
            return self.board[r][0]
        return self.board[r][c]
pieces = set()

def resolve(board, subsets):
    #board.debug_outln()
    
    if board.is_complete():
        return True

    current = board.get_current_to_search()

    for i in range(len(subsets[current])):
        if subsets[current][i] in pieces:
            for rot in range(4):
                if board.piece_fits( subsets[current][i].rotate(rot) ):
                    subsets[current][i].now = rot
                    board.insert( subsets[current][i] )
                    pieces.remove( subsets[current][i] )

                    result = resolve(board, subsets)
                    if result:
                        return True
                    
                    pieces.add( board.pop() )

    return False

if __name__ == "__main__":
    n = int(readln())

    # Number of pieces, Rows, Cols
    for _ in range(n):
        N, R, C = list(map(int, readln().split()))

        first_piece = Piece(readln().split())

        # Create Board
        board = Board(R, C, first_piece)

        pieces_subset = {first_piece : []}

        for __ in range(N-1):
            piece = Piece( readln().split() ) 
            pieces.add(piece)
            pieces_subset[piece] = []

            for elem in pieces_subset:
                if elem!=piece:
                    for rot in range(4):

                        if elem!=first_piece:
                            if elem.any_match(piece.rotate(rot)):
                                pieces_subset[elem].append(piece)
                                pieces_subset[piece].append(elem)
                                break

                        else:
                            if elem.match_right( piece.rotate(rot) ) or elem.match_bottom( piece.rotate(rot) ):
                                pieces_subset[elem].append(piece)
                                break

        start = time()
        if resolve(board, pieces_subset):
            outln(time()-start)
            outln(board, end = "")
        else:
            outln(time()-start)
            outln("impossible puzzle!")
        