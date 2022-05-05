#include <array>
#include <chrono>
#include <cstring>
#include <deque>
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
#include <set>
#include <queue>

using namespace std;

#define COL_SIZE 1002



int *operation_time, *graph, *marks, *recursion_stack, *dp, start_node,
    end_node, n, best, total_time;

deque<int>  q, bottleneck;
priority_queue<int, vector<int>, std::greater<int> > sorted_queue;

int dfs(int node) {
  int i,j;
  marks[node] = 1;
  recursion_stack[node] = 1;

    for (i = 0, j=0; i < COL_SIZE-2; i++) {
        if(graph[node*COL_SIZE + 2 + i] == 1){

          j++;
          if (marks[i] == 0) {
            if (dfs(i) == 0)
                return 0;

            if (dp[node] < operation_time[node] + dp[i])
                dp[node] = operation_time[node] + dp[i];
        } else {
            if (recursion_stack[i] == 1) {
                return 0;
            }
            if (dp[node] < operation_time[node] + dp[i])
                dp[node] = operation_time[node] + dp[i];
        }
      }
      if (j >= graph[node*COL_SIZE])
        break;
    }

    if (dp[node] == -1)
        dp[node] = operation_time[node];
    recursion_stack[node] = 0; // remove node from stack
    return 1;
}


int connectivity() {
  memset(marks, 0, sizeof(int) * n);

  if (dfs(start_node) == 0)
    return 0;

  for (int i = 0; i < n; i++) {
    if (marks[i] == 0) {
      return 0;
    }
  }
  return 1;
}

bool check_valid() {
  int null_out_count = 0;

  for (int i = 0; i < n; i++) {
    // if node_i out-degree is 0
    if (graph[i * COL_SIZE] == 0) {
      end_node = i;
      if (++null_out_count != 1)
        return 0;
    }
  }

  if (null_out_count == 0) {
    return 0;
  }

  return connectivity();
}

/*
void topological_dfs(int node) {
  int i,j;
  marks[node] = 1;

  for (i = 0, j=0; i < COL_SIZE-2; i++) {
      if(graph[node * COL_SIZE + 2 + i] == 1){
          j++;
          if (marks[i]==0)
            topological_dfs(i);
      }
      if (j >= graph[node * COL_SIZE])
        break;
  }
  q.push_front(node + 1);
}
*/

void topological_bfs(){
    int current_node, i, j;
    sorted_queue.push(start_node);

    while (!sorted_queue.empty()){
        current_node = sorted_queue.top();
        sorted_queue.pop();
        
        bottleneck.push_front(current_node+1);

        for (i = 0, j = 0; i < COL_SIZE-2; i++){
            if (graph[current_node * COL_SIZE + 2 + i] == 1){
                j++;
                if (--graph[i * COL_SIZE + 1] == 0){
                    sorted_queue.push(i);
                }
                if (j >= graph[current_node * COL_SIZE])
                    break;
            }
        }
    }
}

void single_operation() {
    memset(marks, 0, sizeof(int) * n);

    //topological_dfs(start_node);
    topological_bfs();
    std::cout << total_time << '\n';

    while (!bottleneck.empty()) {
        std::cout << bottleneck.back() << "\n";
        bottleneck.pop_back();
    }
}


int sum(){
    int s = 0;
    for (int i = 0; i <1000; i++){
        s+=marks[i];
    }
    return s;
}

void find_bottleneck(){
    int current_node, i, j;

    memset(marks, 0, sizeof(int) * 1000);
    q.push_back(start_node);
    marks[start_node] = 1;

    while (!q.empty()){
        current_node = q.front();
        q.pop_front();

        for (i = 0; i < 1000; i++){
            if (graph[i*COL_SIZE + 2 + current_node] == 1){
                marks[i]=0;
            }
        }

        if (sum() == 1)
            bottleneck.push_back(current_node + 1);

        for (i = 0, j = 0; i < COL_SIZE-2; i++){
            if (graph[current_node * COL_SIZE + 2 + i] == 1){
                j++;
                if (--graph[i * COL_SIZE + 1] == 0){
                    q.push_back(i);
                    marks[i] = 1;
                }
                if (j >= graph[current_node * COL_SIZE])
                    break;
            }
        }

        //marks[current_node] = 0;
    }

    while( !bottleneck.empty() ){
        cout<<bottleneck.front()<<"\n";
        bottleneck.pop_front();
    }
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(0);

  operation_time = new int[1000];
  graph = new int[1002000];
  marks = new int[1000];
  recursion_stack = new int[1000];
  dp = new int[1000];

  memset(operation_time, 0, sizeof(int) * 1000);
  memset(graph, 0, sizeof(int) * 1002000);
  memset(recursion_stack, 0, sizeof(int) * 1000);
  memset(dp, -1, sizeof(int) * 1000);
  memset(marks, 0, sizeof(int) * 1000);

  int m, i, j, op;
  int start_declared = 0;

  std::cin >> n;
  for (i = 0; i < n; i++) {
    std::cin >> operation_time[i] >> m;

    total_time += operation_time[i];
    graph[i * COL_SIZE + 1] = m;

    if (m == 0) {
      if (start_declared) {
        std::cout << "INVALID\n";
        return 0;

      } else {
        start_node = i;
        start_declared = 1;
      }
    } else {
      for (j = 0; j < m; j++) {
        std::cin >> op;
        op--;
        graph[op * COL_SIZE]++;
        graph[op * COL_SIZE + 2 + i] = 1;
      }
    }
  }
    
    /*
    for (i=0; i <n; i++){
        for(j=0;j<n;j++){
            cout<<graph[i*COL_SIZE + 2 + j]<<" ";
        }
        cout<<"\n";
    }
    */
  if (!check_valid()) {
    std::cout << "INVALID\n";
    return 0;
  }
    
  std::cin >> m;
  switch (m) {
  case 0:
    std::cout << "VALID\n";
    break;
  case 1:
    single_operation();
    break;
  case 2:
    cout << dp[start_node] << "\n";
    break;
  case 3:
    find_bottleneck();
    break;
  }

  delete[] operation_time;
  delete[] graph;
  delete[] marks;
  delete[] dp;
  delete[] recursion_stack;
}

