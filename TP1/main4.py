from sys import stdin, stdout, setrecursionlimit
from time import time

setrecursionlimit(2500)
N = 0
R = 0
C = 0

def read_all():
    return stdin.readlines()

def readln():
    return stdin.readline().rstrip()


def outln(n="", end="\n"):
    return stdout.write(str(n) + end)

class Piece:
    left = None
    right = None
    up = None
    down = None
    values = []
    uses = 0
    rotation = 0

    def __str__(self):
        return (
            "u:" + str(self.up) +
            " r:" + str(self.right) +
            " d:" + str(self.down) + 
            " l:" + str(self.left) 
        )

class Aux:
    inserted_pieces = 1


def print_board(board, r, c):
    for i in range(r):
        line = ''
        for k in range(2):
            for j in range(c):
                if k == 0:
                    line += str(board[i][j].up) + ''
                else:
                    line += str(board[i][j].down) + ''
                line += '\n' if j == c - 1 else '  '
        outln(line)
        
    #outln("\n", end="")


def rotate(piece):
    rotated_piece = Piece()
    temp = piece.values
    rotated_piece.up = (temp[3], temp[0])
    rotated_piece.right = (temp[0], temp[1])
    rotated_piece.down = (temp[2], temp[1])
    rotated_piece.left = (temp[3], temp[2])
    return rotated_piece

def get_current(a):
    row = a.inserted_pieces // C
    col = a.inserted_pieces % C
    
    return row, col


# def solve(board, pieces_dict, r, c):

#     # Fazer board
#     for i in range(r):
#         for j in range(c - 1):
#             if i == 0:
#                 # se estivermos na 1º linha
#                 #print(f'({i}, {j}) ===', len(pieces_dict['l' + str(board[i][j].right)]) )
#                 board[i][j + 1] = pieces_dict['l' + str(board[i][j].right)]
#             else:
#                 if j == 0: # linha n, coluna 0 -> nenhuma peça do lado esquerdo
#                     #print(f'({i}, {j}) ===', len(pieces_dict['u' + str(board[i-1][j].down)]) )
#                     board[i][j] = pieces_dict['u' + str(board[i-1][j].down)]
#                 else:
#                     pass
    
#     print('\n')
#     print_board(board, r, c)


def solve_2(board, pieces_dict, r, c, a):
    
    row, col = get_current(a)

    if a.inserted_pieces == N:
        print("DONE")
        return
    
    print(f"({row}, {col})")
    if row == 0:
        if col == 0:
            pass
        else:
            #print(pieces_dict['l' + str(board[row][col - 1].right)])
            arr = pieces_dict.get('l' + str(board[row][col - 1].right))
            if arr == []:
                return
            board[row][col] = arr[0]
            
            pieces_dict.pop()
            
            solve_2(board, pieces_dict, r, c, a)
            #print(str(board[row][col-1]))
    
    a.inserted_pieces += 1
    solve_2(board, pieces_dict, r, c, a)

    #print('\n')
    #print_board(board, r, c)
            

if __name__ == "__main__":
    setrecursionlimit(2500)
    
    for _ in range(int(readln())):
        N, R, C = list(map(int, readln().split()))
        
        pieces = []
        
        for i in range(N):
            array = list(map(int, readln().split()))
            
            new_piece = Piece()
            new_piece.values = array
            new_piece.up = (array[0], array[1])
            new_piece.right = (array[1], array[2])
            new_piece.down = (array[3], array[2])
            new_piece.left = (array[0], array[3])
            
            pieces.append(new_piece)
    
        # Create Board
        board = [[Piece() for _ in range(C)] for __ in range(R)]
        #board = [[None for _ in range(C)] for __ in range(R)]
        board[0][0] = pieces[0]
        
        # Colocar todas as peças na board para debug
        # for i in range(R):
        #     for j in range(C):
        #         array = list(map(int, readln().split()))
        #         new_piece = Piece()
        #         new_piece.up = (array[0], array[1])
        #         new_piece.right = (array[1], array[2])
        #         new_piece.down = (array[3], array[2])
        #         new_piece.left = (array[0], array[3])
        #         board[i][j] = new_piece
                    
        
        #print_board(board, R, C)
        
        pieces_dict = {}
    
        # apenas guardar up and left pela maneira como as peças sao inseridas na board. da esquerda para direita e de cima para baixo
        for p in pieces:
            temp = p
            for i in range(3):
                if pieces_dict.get('u' + str(temp.up)) == None:
                    pieces_dict['u' + str(temp.up)] = [p]
                else:
                    pieces_dict['u' + str(temp.up)].append(p)

                # pieces_dict['r' + str(temp.right)] = p
                # pieces_dict['d' + str(temp.down)] = p
                
                if pieces_dict.get('l' + str(temp.left)) == None:
                    pieces_dict['l' + str(temp.left)] = [p]
                else:
                    pieces_dict['l' + str(temp.left)].append(p)
                
                temp = rotate(p)
                temp.rotation = i + 1
        
        for key, value in pieces_dict.items():
            print(key + ' -> ' + str(len(value)))
        
        a = Aux()
        #a.inserted_pieces = 1
        
        solve_2(board, pieces_dict, R, C, a)
        
        print('\n')
        print_board(board, R, C)

        # print(index_to_array)
        # start = time()
        # if resolve(board, pieces_subset):
        #     outln(time() - start)
        #     print_board(board)
        # else:
        #     outln(time() - start)
        #     outln("impossible puzzle!")
