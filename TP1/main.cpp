#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

unordered_map<std::string, std::vector<int>> matches;

void create_piece(int _1,int _2,int _3,int _4,int counter, int pieces[][6]){
    pieces[counter][0] = _1;
    pieces[counter][1] = _2;
    pieces[counter][2] = _3;
    pieces[counter][3] = _4;
    pieces[counter][4] = 0; //rotação
    pieces[counter][5] = 0; //flag_used;
}

void fill(int r, int c, int *board){
    for(int i = 0; i < r; i++){
        for (int j = 0; j < c; j++){
            board[i*c + j] = -1;
        }
    }
}

std::string get_key(int a, int b){
    std::stringstream sa,sb;
    sa << std::setw(3) << std::setfill('0') << a;
    sb << std::setw(3) << std::setfill('0') << b;
    sa << sb.str();
    return sa.str();
}

void print_board(int * board, int r, int c, int pieces[][6]){
    for ( int i = 0; i < r; i++){
        for ( int l = 0; l < 2; l++){
            for (int j = 0; j < c; j++){
                int index = i*c + j;
                int rot = 4-pieces[board[index]][4];
                if (l == 0)
                    std::cout<<pieces[board[index]][rot%4]<<" "<<pieces[board[index]][(rot+1)%4];
                else
                    std::cout<<pieces[board[index]][(rot+3)%4]<<" "<<pieces[board[index]][(rot+2)%4];
                
                if(j!=c-1)
                    std::cout<<"  ";
            }
            std::cout<<"\n";
        }
        if(i!=r-1)
            std::cout<<"\n";
    }
    
}

std::string get_current_match(int * board, int c, int next_empty, int pieces[][6]){
    int r_now, c_now, p1, p2, rot, index;
    r_now = (next_empty - 1)/c;
    c_now = (next_empty - 1)%c;
    if (c_now == c-1){
        index = board[r_now*c];
        rot = 4 - pieces[index][4];
        p1 = pieces[index][ (2+rot)%4 ];
        p2 = pieces[index][ (3+rot)%4 ];
        return get_key(p1, p2);
    }else{
        index = board[r_now*c + c_now];
        rot = 4 - pieces[index][4];
        p1 = pieces[index][ (1+rot)%4 ];
        p2 = pieces[index][ (2+rot)%4 ];
        return get_key(p1, p2);
    }
}

bool fits(int * board, int c, int next_empty, int pieces[][6], int index, int rot_in){
    int r_now, c_now, rot, b_index;
    r_now = next_empty/c;
    c_now = next_empty%c;

    if ( r_now == 0){
        b_index = c_now - 1;
        rot = 4 - pieces[board[b_index]][4];

        bool l1 = (pieces[index][(4 - rot_in)%4] == pieces[board[b_index]][(1+rot)%4]);
        bool l2 = (pieces[index][(7 - rot_in)%4] == pieces[board[b_index]][(2+rot)%4]);
        return l1 && l2 ;   
    }
    if ( c_now == 0){
        b_index = (r_now-1) * c;
        rot = 4-pieces[board[b_index]][4];
        
        bool u1 = (pieces[index][(4 - rot_in)%4] == pieces[board[b_index]][(3 + rot)% 4]);
        bool u2 = (pieces[index][(5 - rot_in)%4] == pieces[board[b_index]][(2 + rot)%4]);
        return  u1 && u2; 
    }
    
    b_index = r_now * c + c_now - 1;
    rot = 4-pieces[board[b_index]][4];

    bool l1 = (pieces[index][(4 - rot_in)%4] == pieces[board[b_index]][(1+rot)%4]);
    bool l2 = (pieces[index][(7 - rot_in)%4] == pieces[board[b_index]][(2+rot)%4]);
    
    b_index = (r_now-1) * c + c_now;
    rot = 4-pieces[board[b_index]][4];

    bool u1 = (pieces[index][(4 - rot_in)%4] == pieces[board[b_index]][(3 + rot)% 4]);
    bool u2 = (pieces[index][(5 - rot_in)%4] == pieces[board[b_index]][(2 + rot)%4]);
    return l1&&l2&&u1&&u2;
}

void insert(int * board, int c, int next_empty, int index){
    int r_now, c_now;
    
    r_now = next_empty/c;
    c_now = next_empty%c;

    board[r_now*c+c_now] = index;
}

void pop(int * board, int c, int next_empty){
    int r_now, c_now;
    r_now = next_empty/c;
    c_now = next_empty%c;
    
    board[r_now*c+c_now] = -1;
}

bool solve(int * board, int r, int c, int next_empty, int pieces[][6]){
    /*for(int i = 0; i < r; i++){
        for (int j = 0; j < c; j++)
            cout<<board[i*c+j]<<" ";
        cout<<"\n";
    }*/
    if (next_empty == r*c)
        return true;
    
    std::string match = get_current_match(board, c, next_empty, pieces);
    //cout<<"match: "<<match<<"\n";
    bool result;
    for(int index: matches[match]){
        if (pieces[index][5] == 0){
            for (int rot = 0; rot < 4; rot++){
                //cout<<rot<<"\n";
                if (fits(board, c, next_empty, pieces, index, rot) == 1){
                    pieces[index][4] = rot;
                    pieces[index][5] = 1;
                    insert(board, c, next_empty, index);

                    result = solve(board, r, c, next_empty + 1, pieces);
                    if (result)
                        return true;
                    
                    pieces[index][4] = 0;
                    pieces[index][5] = 0;
                    pop(board, c, next_empty);
                }
            }
        }
    }
    return false;
}

int main(){
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int _, n, r, c, p1,p2,p3,p4;
    std::cin >> _;

    for (int i = 0; i < _; i++){

        std::cin>>n>>r>>c;
        int pieces[n][6];

        std::cin>>p1>>p2>>p3>>p4;
        create_piece(p1, p2, p3, p4, 0, pieces);

        int * board = new int[r*c];

        fill(r, c, board);
        
        board[0] = 0;

        for (int j = 0; j < n - 1; j++){
            std::cin>>p1>>p2>>p3>>p4;
            create_piece(p1, p2, p3, p4, j + 1, pieces);

            //std::cout<<get_key(p2,p1)<<"\n";
            matches[get_key(p1, p4)].push_back(j+1);
            matches[get_key(p4, p3)].push_back(j+1);
            matches[get_key(p3, p2)].push_back(j+1);
            matches[get_key(p2, p1)].push_back(j+1);
        }

        if (solve(board, r, c, 1, pieces) == true){
            print_board(board, r, c, pieces);
        }else{
            std::cout<<"impossible puzzle!"<<"\n";
        }
        delete[] board;
    }
    return 0;
}