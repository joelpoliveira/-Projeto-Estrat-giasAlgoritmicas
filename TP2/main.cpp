#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

int *graph, *values, n, best;
bool * is_visited;

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int node_now, other_node, total_nodes=0;

        
    best = 0;
    graph = new int[100000*10];

    values = new int[100000];
    is_visited = new bool[10000];
    for (int i = 0; i < 10000; i++) is_visited[i] = false; 

    while (!cin.eof()) {
        cin >> node_now;
        if (node_now == -1){
            //cout<<"nodes number = "<<total_nodes<<"\n";
            total_nodes=0;
        }else{
            while( (cin>>other_node) ){
                if ( cin.peek()=='\n' ){
                    values[node_now] = other_node;
                    cout << "\n";
                    break;
                }else{
                    cout<<other_node<<" ";
                }
            }
            total_nodes+=1;
        }
    }

  return 0;
}