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

int *values, 
    *graph,
    cum_sum,
    max_sum;

pair<int, int> *dp;

pair<int,int> min(pair<int,int> a, pair<int,int> b){
    if (a.first<b.first)
        return a;
    else if (a.first==b.first){
        if (a.second>b.second)
            return a;
        return b;
    }
    return b;
}

int is_better(pair<int, int> a, pair<int,int> b){
    if (a.first<b.first)
        return 1;
    else if (a.first==b.first){
        if (a.second>b.second)
            return 1;
        return 0;
    }
    return 0;
}

pair<int,int> resolve(int node_now, int is_watched){
    int graph_index = node_now * 10, dp_index = node_now*2;
    if (graph[graph_index]==-1){
        if (is_watched){
            return {is_watched, 0};
        return {is_watched, values[node_now]};
        }
    }
    else if(dp[dp_index + is_watched].first!=-1){
        return dp[dp_index + is_watched];
    }

    int i, j;
    pair <int,int> result = {0,0}, resp, resp1, resp2;
    for (i = 0; i < 10; i++){
        j = graph_index+i;
        if (graph[j]==-1) break;
        
        if (is_watched){
            resp = min(resolve(graph[j], 1), resolve(graph[j], 0));
            result.first+=resp.first;
            result.second+=resp.second;
        }else{
            resp1=resolve(graph[j], 1);
            result.first+=resp1.first;
            result.second+=resp1.second;
        }
    }
    
    if (is_watched){
        result.first +=is_watched;
        result.second+=values[node_now];
    }
    dp[dp_index + is_watched] = result;
    return dp[dp_index + is_watched];    
}


int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int node_now, 
        other_node, 
        v_index,
        i;
    pair<int,int> res;
    values = new int[100000];
    dp = new pair<int,int>[100000 * 2];
    graph = new int[100000 * 10];

    cum_sum = 0;
    max_sum  = 0;
    for(i = 0; i < 100000; i++) values[i] = 0;
    for(i = 0; i < 200000; i++) dp[i] = {-1,-1};
    for(i = 0; i < 1000000; i++) graph[i] = -1;

    while (!cin.eof()) {
        cin >> node_now;
        v_index=0;

        if (node_now == -1){
            res = resolve(0, 0);
            cout<<res.first<<" "<<res.second<<"\n";

            //reset data;
            max_sum = 0;
            for(i = 0; i < 100000; i++) values[i] = 0;
            for(i = 0; i < 200000; i++) dp[i] = {-1,-1};
            for(i = 0; i < 1000000; i++) graph[i] = -1;
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

    delete [] values;
    delete [] dp;
    delete [] graph;
  return 0;
}