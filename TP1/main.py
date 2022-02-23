from sys import stdin, stdout, setrecursionlimit
from collections import deque
from time import time
setrecursionlimit(2501)

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()
    
def outln(n = '', end = '\n'):
    return stdout.write(str(n) + end)

class Piece:    
    def __init__(self, array):
        self.numbers = [[array[0], array[1]], [array[3], array[2]]]
        #print("Created: ", self.numbers)
    
    def row(self, n):
        return ' '.join(self.numbers[n])
    
    def print_row(self, n):
        for i in range(2):
            outln(self.numbers[n][i], end = " ")
    
    def rotate(self, n):
        piece = self.copy()
        if n%4== 0:
            return piece
        elif n%4==1:
            temp = piece.numbers
            piece.numbers = [[temp[1][0], temp[0][0]], [temp[1][1], temp[0][1]]]
        elif n%4==2:
            temp = piece.numbers
            piece.numbers = [[temp[1][1], temp[1][0]], [temp[0][1], temp[0][0]]]
        elif n%4==3:
            temp = piece.numbers
            piece.numbers = [ [temp[0][1], temp[1][1]], [temp[0][0], temp[1][0]] ]
        return piece

    def match_up(self, piece):
        return (self.numbers[0][0] == piece.numbers[1][0]) and (self.numbers[0][1] == piece.numbers[1][1]) 

    def match_left(self, piece):
        return (self.numbers[0][0] == piece.numbers[0][1]) and (self.numbers[1][0] == piece.numbers[1][1]) 

    def copy(self):
        return Piece([self.numbers[0][0], self.numbers[0][1], self.numbers[1][1], self.numbers[1][0]])

    def get_probable(self, pieces):
        probable = []
        for i in range(len(pieces)):
            if self == pieces[i]:
                continue
            elif self.diff_numbers.intersection(pieces[i].diff_numbers)!=set():
                probable.append(pieces[i])
        return probable

class Board:    
    def __init__(self, rows, cols, first_piece):
        self.board = [[None for i in range(cols)] for j in range(rows)]
        self.board[0][0] = first_piece
        self.rows = rows
        self.cols = cols
        self.next_empty = 1
     
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

    def get_last(self):
        row = (self.next_empty - 1)//self.cols
        col = (self.next_empty - 1)%self.cols
        return row,col

    def insert(self, Piece):
        r,c = self.get_next()
        self.board[r][c] = Piece
        self.next_empty+=1

    def piece_fits(self, piece):
        r,c = self.get_next()
        #print(f"{r}--{c}")
        
        if r == 0:
            if piece.match_left(self.board[r][c-1]):
                return True
            return False

        elif c == 0:
            if piece.match_up(self.board[r-1][c]):
                return True
            return False

        else:
            if piece.match_up(self.board[r-1][c]) and piece.match_left(self.board[r][c-1]):
                return True
            return False

    def swap(self):
        pass
    
    def solve(self):
        pass
    
    def is_complete(self):
       return (self.next_empty)==(self.rows*self.cols)
	
    def pop(self):
        r,c = self.get_last()
        piece = self.board[r][c]
        self.board[r][c] = None
        self.next_empty-=1
        return piece

    def copy(self):
        new_b = Board(self.rows, self.cols, self.board[0][0])
        new_b.board = self.board.copy()
        return new_b
    
def resolve(board, pieces_to_use, used_pieces):
	pieces_to_use+=used_pieces
	used_pieces.clear()

	while True:
		#bool(empty list) = False
		if pieces_to_use:
			resolved = False
			#try all piece rotations
			for _ in range(4):
				if (board.piece_fits(pieces_to_use[0].rotate(_))):
					board.insert( pieces_to_use.popleft().rotate(_) )

					#piece fits, try next
					resolved = resolve(board, pieces_to_use.copy(), used_pieces.copy())

					if resolved:
						return resolved
					else:
						pieces_to_use.appendleft( board.pop() )

			used_pieces.append( pieces_to_use.popleft() )
			
		else:			
			if board.is_complete():
				return True
			return False

if __name__ == "__main__":
    n = int(readln())

    # Number of pieces, Rows, Cols
    for _ in range(n):
        pieces_to_use = deque() # to resolve uncomment this
        used_pieces = deque()


        N, R, C = list(map(int, readln().split()))

        first_piece = Piece(readln().split())
        # Create Board
        board = Board(R, C, first_piece)
        for __ in range(N - 1):
            pieces_to_use.append( Piece( readln().split() ) )
        start = time()
        #if resolve(board, pieces_to_use, used_pieces):
        if resolve(board, pieces_to_use, used_pieces):
            outln(board, end = "")
        else:
            outln("impossible puzzle!")
        outln(time()-start)