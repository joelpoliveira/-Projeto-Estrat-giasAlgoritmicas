#include <array>
#include <chrono>
#include <cstring>
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

int *operation_time, 
    *graph, 
    *marks,
    n;

void dfs(int node){
    int child;
    marks[node] = 1;
    for (int i = 2; i < n+2; i++){
        child = graph[node * 1000 + i];
        if (marks[child]==0)
            dfs( child );
    }
}

int connectivity(int start_node){
    memset(marks, 0, sizeof(int) * 1000);
    dfs(start_node);
    for (int i = 0; i < n; i++){
        if (marks[i] == 0){
            return 0;
        }
    }
    return 1;
}

int check_valid() { 
    int null_in_count, null_out_count, start_node=0;
    null_in_count = null_out_count = 0;

    for (int i = 0; i < n; i++){
        if (graph[i*1000]==0){
            null_in_count++;
            start_node = i;
        }

        if (graph[i*1000 + 1]==0)
            null_out_count++;
    }
    cout<<null_in_count<<" "<<null_out_count<<"\n";
    if (null_in_count != 1 || null_out_count != 1){
        return 0;
    }

    return connectivity(start_node);
}

void single_operation() {
  // Greedy algorithm to select operation order
  int time = 0;
  for (int i = 0; i < n; i++) {
      time += operation_time[i];
      for (int j = 0; j < graph[i]; j++){
          
      }
  }
  std::cout << time << '\n';
  // std::cout << 
}

void multi_operation() {
  // Find the operation that takes the longest
  int best = 0;
  for (int i = 0; i < n; i++) {
    if (operation_time[i] > best) {
      best = operation_time[i];
    }
  }
  // return best;
  std::cout << best << "\n";
}

void find_bottleneck() {}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    operation_time = new int[1000];
    graph = new int[102000];
    marks = new int[1000];

    memset(operation_time, 0, sizeof(int) * 1000);
    memset(graph, 0, sizeof(int) * 102000);
    
    
    int m, i, j, op;
    
    std::cin >> n;
    for (i = 0; i < n; i++) {
        std::cin >> operation_time[i] >> m;

        
        for (j = 0; j < m; j++) {
            std::cin >> op;
            op--;
            //incremente node i outdegree
            graph[i * 1000 + 1]++;
            //increment node op indegree
            graph[op * 1000]++;
            graph[i * 1000 + 2 + j] = op;
        }
    
    }

  if ( check_valid() == 0) {
    std::cout << "INVALID\n";
    return 0;
  }

  // std::cout << "operation_time:\n";
  //   for (int i = 0; i < 1000; i++) {
  //     if (operation_time[i] != 0) {
  //       std::cout << operation_time[i] << "\n";
  //     }
  //   }

  std::cin >> m;
  switch (m) {
  case 0:
    std::cout << "VALID\n";
    break;
  case 1:
    std::cout << "Operation 1\n";
    single_operation();
    break;
  case 2:
    multi_operation();
    std::cout << "Operation 2\n";
    break;
  case 3:
    find_bottleneck();
    std::cout << "Operation 3\n";
    break;
  }

  return 0;
}
