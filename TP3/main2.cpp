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

using namespace std;

#define COL_SIZE 1002

int *operation_time, *graph, *marks, *recursion_stack, *dp, start_node,
    end_node, n, best, total_time;

deque<int>  q, 
            q1, 
            q2, 
            bottleneck;

int dfs(int node) {
  int child;
cout<<"node = "<<node<<"\n";
  marks[node] = 1;
  recursion_stack[node] = 1;

  for (int i = 0; i < graph[node * COL_SIZE]; i++) {
    child = graph[node * COL_SIZE + 2 + i];
    // std::cout << "node: " << node + 1 << " child = " << child + 1 << "\n";

    if (marks[child] == 0) {
      if (dfs(child) == 0)
        return 0;
      if (dp[node] < operation_time[node] + dp[child])
        dp[node] = operation_time[node] + dp[child];
    } else {
      if (recursion_stack[child] == 1) {
        return 0;
      }
      if (dp[node] < operation_time[node] + dp[child])
        dp[node] = operation_time[node] + dp[child];
    }
  }

  if (dp[node] == -1)
    dp[node] = operation_time[node];
  recursion_stack[node] = 0; // remove node from stack
  return 1;
}

int connectivity() {
  // std::cout << "n = " << n << " start_node = " << start_node + 1 << "\n";

  memset(marks, 0, sizeof(int) * n);

  if (dfs(start_node) == 0)
    return 0;

  for (int i = 0; i < n; i++) {
    if (marks[i] == 0) {
      // std::cout << "node " << i + 1 << " not marked\n";
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

void topological_dfs(int node) {
  int child;
  marks[node] = 1;

  for (int i = 0; i < graph[node * COL_SIZE]; i++) {
    child = graph[node * COL_SIZE + 2 + i];
    if (marks[child] == 0)
      topological_dfs(child);
  }
  q.push_front(node + 1);
}

void single_operation() {
  memset(marks, 0, sizeof(int) * n);

  topological_dfs(start_node);

  std::cout << total_time << '\n';

  while (!q.empty()) {
    std::cout << q.front() << "\n";
    q.pop_front();
  }
}


int sum(){
    int s = 0;
    for (int i = 0; i <1000; i++){
        s+=marks[i];
    }
    return s;
}


void find_bottleneck() {
    int is_q1=1, 
        current_node, 
        current_node_index,
        in_queue,
        next_proc,
        next_proc_count,
        child, 
        i;
    
    bottleneck.push_back(start_node+1);
    q1.push_back(start_node);
    in_queue = 1;
    next_proc_count = 0;
    
    while (!q1.empty() || !q2.empty()){
        //cout<<is_q1<<"\n";
        if (is_q1){
            if (!q1.empty()){
                current_node = q1.front();
                q1.pop_front();

                current_node_index = current_node * COL_SIZE;

                next_proc = 0;
                for (i = 0; i < graph[current_node_index]; i++){
                    child = graph[current_node_index + 2 + i];

                    if (--graph[child * COL_SIZE + 1]==0){
                        next_proc = 1;
                        q2.push_back(child);
                    }
                }
                next_proc_count += next_proc;

                if (q1.empty()){
                    is_q1=0;
                    
                    cout<<"inc = "<<next_proc_count<<"\tprev_len = "<<in_queue<<"\tnode ="<<q2.front()<<"\n";
                    
                    if(q2.size() == 1 && next_proc_count == in_queue){
                        bottleneck.push_back(q2.front() + 1);
                    }
                    in_queue = q2.size();
                    next_proc_count = 0;
                }
            }
        }else{
            if (!q2.empty()){
                current_node = q2.front();
                q2.pop_front();
                
                current_node_index = current_node * COL_SIZE;
                
                next_proc=0;
                for (i = 0; i < graph[current_node_index]; i++){
                    child = graph[current_node_index + 2 + i];
                    
                    if (--graph[child * COL_SIZE + 1]==0){
                        next_proc=1;
                        q1.push_back(child);
                    }
                }
                next_proc_count+=next_proc;
    
                if (q2.empty()){
                    is_q1=1;
                    
                    cout<<"inc = "<<next_proc_count<<"\tprev_len = "<<in_queue<<"\tnode ="<<q1.front()<<"\n";
                    
                    if(q1.size() == 1 && next_proc_count == in_queue){
                        bottleneck.push_back(q1.front() + 1);
                    }
                    in_queue = q1.size();
                    next_proc_count = 0;
                }
            }            
        }
    }
    
    
    while( !bottleneck.empty() ){
        cout<<bottleneck.front()<<"\n";
        bottleneck.pop_front();
    }
}


/*
void top_dsf(int node, int prev){
    
    marks[node] = 1;
    
    if (--graph[node * COL_SIZE + 1] == 0){
        cout<<"node = "<<node<<"\n";
        
        if (sum()==1){
            bottleneck.push_back(node+1);
        }
        marks[prev] = 0;
        
        int child;
        for (int i = 0; i <graph[node * COL_SIZE]; i++){
            child = graph[node*COL_SIZE + 2 +i];
            top_dsf(child, node);
        }   
    }else{
        marks[prev] = 0;
    }
}

void find_bottleneck(){
    memset(marks, 0, sizeof(int) * 1000);
    marks[start_node] = 1;
    bottleneck.push_back(start_node+1);

    int child;
    for (int i = 0; i < graph[start_node * COL_SIZE]; i++){
        child = graph[start_node * COL_SIZE + 2 + i];
        top_dsf(child, start_node);
    }

    while( !bottleneck.empty() ){
        cout<<bottleneck.front()<<"\n";
        bottleneck.pop_front();
    }
}
*/

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

  int m, i, j, op, aux;
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
        aux = graph[op * COL_SIZE]++;
        graph[op * COL_SIZE + 2 + aux] = i;
      }
    }
  }

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