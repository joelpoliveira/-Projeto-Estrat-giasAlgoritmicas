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
#include <cstring>
using namespace std;

int *values, 
    *graph,
    *dp, 
    cum_sum,
    max_sum;


int min(int a, int b){
    return a<b?a:b;
}

int resolve(int node_now, int is_watched){
    int graph_index = node_now * 10, dp_index = node_now*2;
    if (graph[graph_index]==-1){
        return is_watched;
    }
    else if(dp[dp_index + is_watched]!=-1){
        return dp[dp_index + is_watched];
    }

    int res = 0, i, j;
    for (i = 0; i < 10; i++){
        j = graph_index+i;
        if (graph[j]==-1) break;
        
        if (is_watched){
            res+=min(resolve(graph[j], 1), resolve(graph[j], 0));
        }else{
            res+=resolve(graph[j], 1);
        }
    }
    dp[dp_index + is_watched] = res + is_watched;
    return dp[dp_index + is_watched];
    
}


int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int node_now, 
        other_node, 
        v_index;

    values = new int[100000];
    dp = new int[100000 * 2];
    graph = new int[100000 * 10];

    cum_sum = 0;
    max_sum  = 0;
    memset(values, 0, sizeof(int) * 100000);
    memset(dp, -1, sizeof(int) * 200000);
    memset(graph, -1, sizeof(int) * 1000000);

    while (!cin.eof()) {
        cin >> node_now;
        v_index=0;

        if (node_now == -1){
            cout<<min(resolve(0, 0), resolve(0,1))<<"\n";

            //reset data;
            max_sum = 0;
            memset(values, 0, sizeof(int) * 100000);
            memset(dp, -1, sizeof(int) * 100000);
            memset(graph, -1, sizeof(int) * 1000000);
        }else{
            while( (cin>>other_node) ){
                if ( cin.peek()=='\n' ){
                    //assign node value;
                    values[node_now] = other_node;
                    break;
                }else{
                    graph[node_now * 10 + v_index++] = other_node; 
                }
            }

        }
    }

  return 0;
}