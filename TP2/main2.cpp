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
    *graph;

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

pair<int,int> resolve(int node_now, int is_watched){
    //cout<<"##DEBUG##\n"<<node_now<<" "<<is_watched<<"\n"
    //<<dp[2].second<<" "<<dp[8].second<<"\n##DEBUG##\n";
    int graph_index = node_now * 10, dp_index = node_now*2;
    if (graph[graph_index]==-1){
        if (is_watched){
            return {is_watched, values[node_now]};
        return {is_watched, 0};
        }
    }
    else if(dp[dp_index + is_watched].first!=-1){
        return dp[dp_index + is_watched];
    }

    int i, j;
    pair <int,int> result = {0,0}, resp;
    for (i = 0; i < 10; i++){
        j = graph_index+i;
        if (graph[j]==-1) break;
        
        if (is_watched){
            resp = min(resolve(graph[j], 1), resolve(graph[j], 0));
            result.first+=resp.first;
            result.second+=resp.second;
        }else{
            resp=resolve(graph[j], 1);
            result.first+=resp.first;
            result.second+=resp.second;
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
    dp = new pair<int,int>[200000];
    graph = new int[1000000];

    for(i = 0; i < 100000; i++) values[i] = 0;
    for(i = 0; i < 200000; i++) dp[i] = {-1,-1};
    for(i = 0; i < 1000000; i++) graph[i] = -1;

    while (!cin.eof()) {
        cin >> node_now;
        v_index=0;

        if (node_now == -1){
            if (graph[0]==-1){
                cout<<1<<" "<<values[0]<<"\n";
            }else{
                res = min(resolve(0, 0), resolve(0,1));
                cout<<res.first<<" "<<res.second<<"\n";
            }
            //reset data;
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