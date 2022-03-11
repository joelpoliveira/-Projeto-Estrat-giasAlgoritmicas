#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <array>
#include <chrono>
using namespace std;

// function obtained through stackoverflow:
// https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys
struct ArrayHasher {
    std::size_t operator()(const std::array<int, 2>& a) const {
        std::size_t h = 0;

        for (auto e : a) {
            h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
        }
        return h;
    }   
};

struct ArrayHasher2 {
    std::size_t operator()(const std::array<int, 3>& a) const {
        std::size_t h = 0;

        for (auto e : a) {
            h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
        }
        return h;
    }   
};


unordered_map<array<int, 2>, vector<int*>, ArrayHasher> matches;
unordered_map<array<int, 3>, vector<int*>, ArrayHasher2> double_matches;
int ** board, *pieces, r, c;

int sum(int * counts){
    int s = 0;
    for (int i = 0; i < 1000; i++)
        s+=counts[i];
    return s;
}

void print_board(int ** board){
    int i, j, l, *piece, rot;
    for ( i = 0; i < r; i++){
        for ( l = 0; l < 2; l++){
            for (j = 0; j < c; j++){
                piece = board[i*c + j];
                rot = 4 - piece[4];
                if (l == 0)
                    std::cout<<piece[rot%4]<<" "<<piece[(rot+1)%4];
                else
                    std::cout<<piece[(rot+3)%4]<<" "<<piece[(rot+2)%4];
                
                if(j!=c-1)
                    std::cout<<"  ";
            }
            std::cout<<"\n";
        }
        if(i!=r-1)
            std::cout<<"\n";
    }
    
}

array<int, 2> get_current_match(int next_empty){
    int r_now, c_now, *piece;
    r_now = (next_empty - 1)/c;
    c_now = (next_empty - 1)%c;

    if (c_now == c-1){
        piece = board[r_now*c];
        return {piece[ (6-piece[4])%4 ], piece[ (7-piece[4])%4 ]};

    }else{
        piece = board[r_now*c + c_now];
        return {piece[ (5-piece[4])%4 ], piece[ (6-piece[4])%4 ]};
    }
}

array<int,3> get_current_double_match(int next_empty){
    int r_now, c_now, *piece_up, *piece_left;
    r_now = (next_empty - 1)/c;
    c_now = (next_empty - 1)%c;

    piece_up = board[(r_now-1) * c + c_now+1];
    piece_left = board[r_now * c + c_now];
    return {piece_up[(6-piece_up[4])%4], piece_up[(7-piece_up[4])%4], piece_left[(6-piece_left[4])%4]};
}

bool fits(int next_empty, int * ip, int rot_in){
    int r_now, c_now, rot, *bp;
    r_now = next_empty/c;
    c_now = next_empty%c;
    if ( r_now == 0){
        bp = board[c_now - 1];
        rot = 4 - bp[4];
        //left 1 and left 2
        return (ip[(4 - rot_in)%4] == bp[(1+rot)%4]) && (ip[(7 - rot_in)%4] == bp[(2+rot)%4]) ;   
    }
    if ( c_now == 0){
        bp = board[(r_now-1)*c];
        rot = 4 - bp[4];
        //up 1 and up 2
        return  (ip[(4 - rot_in)%4] == bp[(3 + rot)% 4]) && (ip[(5 - rot_in)%4] == bp[(2 + rot)%4]); 
    }
    
    bp = board[r_now * c + c_now - 1];
    rot = 4-bp[4];
    
    bool l1 = (ip[(4 - rot_in)%4] == bp[(1+rot)%4]);
    bool l2 = (ip[(7 - rot_in)%4] == bp[(2+rot)%4]);
    
    bp = board[(r_now-1) * c + c_now];
    rot = 4-bp[4];

    bool u1 = (ip[(4 - rot_in)%4] == bp[(3 + rot)% 4]);
    bool u2 = (ip[(5 - rot_in)%4] == bp[(2 + rot)%4]);
    return l1&&l2&&u1&&u2;
}


bool solve(int next_empty){
    if (next_empty == r*c)
        return true;
    int r_now, c_now;
    r_now = next_empty/c;
    c_now = next_empty%c;
    if (c_now==0 || r_now == 0){
        array<int,2> match = get_current_match(next_empty);
        //cout<<"match: "<<match<<"\n";

        for(int * piece: matches[match]){
            if (piece[5] == 0){
                for (int rot = 0; rot < 4; rot++){
                    if (fits(next_empty, piece, rot) == 1){
                        piece[4] = rot;
                        piece[5] = 1;
                        board[next_empty/c*c+next_empty%c] = piece;                    
                        if ( solve(next_empty + 1) )
                            return true;
                        piece[5] = 0;
                    }
                }
            }
        }
    }else{
        array<int, 3> match = get_current_double_match(next_empty);
        for(int * piece: double_matches[match]){
            if (piece[5] == 0){
                for (int rot = 0; rot < 4; rot++){
                    if (fits(next_empty, piece, rot) == 1){
                        piece[4] = rot;
                        piece[5] = 1;
                        board[next_empty/c*c+next_empty%c] = piece;                    
                        if ( solve(next_empty + 1) )
                            return true;
                        piece[5] = 0;
                    }
                }
            }
        }
    }
    return false;
}

void print_vector(vector<int * > p){
    for (auto &el: p){
        std::cout<<el<<" ";
    }
    cout<<"\n";
}

void print_list(int * p){
    for (int i = 0; i < r*c; i++){
        std::cout<<p[i*6]<<" ";
    }
    cout<<"\n";
}

int main(){
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    int cases, n, p1,p2,p3,p4, j , i, index;
    std::cin >> cases;
    for (i = 0; i < cases; i++){
        matches.clear();
        double_matches.clear();

        std::cin>>n>>r>>c;
        pieces = new int[n*6];

        int counts[1000] = {0};

        std::cin>>p1>>p2>>p3>>p4;
        pieces[0] = p1;
        pieces[1] = p2;
        pieces[2] = p3;
        pieces[3] = p4;
        pieces[4] = 0; //rotação
        pieces[5] = 0;

        counts[p1] = (counts[p1]+1)%2;
        counts[p2] = (counts[p2]+1)%2;
        counts[p3] = (counts[p3]+1)%2;
        counts[p4] = (counts[p4]+1)%2;

        board = new int*[r*c];
        board[0] = pieces;
        for (j = 1; j < n; j++){
            index = j * 6;
            std::cin>>p1>>p2>>p3>>p4;

            pieces[index]   = p1;
            pieces[index+1] = p2;
            pieces[index+2] = p3;
            pieces[index+3] = p4;
            pieces[index+4] = 0; //rotação
            pieces[index+5] = 0;

            counts[p1] = (counts[p1]+1)%2;
            counts[p2] = (counts[p2]+1)%2;
            counts[p3] = (counts[p3]+1)%2;
            counts[p4] = (counts[p4]+1)%2;

            if (matches.count({p1,p2})==0)
                matches.insert(pair<array<int,2>, vector<int *>>({p1,p4}, {}));
            matches[{p1, p4}].push_back( (pieces+j*6) );

            if (matches.count({p4,p3})==0)
                matches.insert(pair<array<int,2>, vector<int *>>({p4,p3}, {}));
            matches[{p4, p3}].push_back( (pieces+j*6) );
            
            if (matches.count({p3,p2})==0)
                matches.insert(pair<array<int,2>, vector<int *>>({p3,p2}, {}));
            matches[{p3, p2}].push_back( (pieces+j*6) );
            
            if (matches.count({p2,p1})==0)
                matches.insert(pair<array<int,2>, vector<int *>>({p2,p1}, {}));
            matches[{p2, p1}].push_back( (pieces+j*6) );

            if (double_matches.count({p2, p1, p4})==0)
                double_matches.insert(pair<array<int,3>, vector<int *>>({p2, p1, p4}, {}));
            double_matches[{p2, p1, p4}].push_back( (pieces+j*6) );

            if (double_matches.count({p1, p4, p3})==0)
                double_matches.insert(pair<array<int, 3>, vector<int *>>({p1, p4, p3}, {}));
            double_matches[{p1,p4, p3}].push_back( (pieces+j*6) );
            
            if (double_matches.count({p4, p3,p2})==0)
                double_matches.insert(pair<array<int,3>, vector<int *>>({p4, p3,p2}, {}));
            double_matches[{p4,p3,p2}].push_back( (pieces+j*6) );
            
            if (double_matches.count({p3,p2,p1})==0)
                double_matches.insert(pair<array<int,3>, vector<int *>>({p3,p2,p1}, {}));
            double_matches[{p3,p2,p1}].push_back( (pieces+j*6) );
        }
        //auto start = chrono::steady_clock::now();
        if (sum(counts)<=4 && solve(1) == true){
            //auto end = chrono::steady_clock::now();
            //cout<<chrono::duration_cast<chrono::milliseconds>(end - start).count()<<"\n";
            print_board(board);
        }else{
            //auto end = chrono::steady_clock::now();
            //cout<<chrono::duration_cast<chrono::milliseconds>(end - start).count()<<"\n";
            std::cout<<"impossible puzzle!"<<"\n";
        }
        delete[] pieces;
        delete[] board;
        
    }
    return 0;
}