#include <array>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

int *operation_time, *graph, *marks, start_node, end_node, n;

void print_graph() {
  for (int x = 0; x < n; x++) {
    std::cout << x + 1 << " -> ";
    for (int y = 0; y < graph[x * 1000]; y++) {
      std::cout << graph[x * 1000 + 1 + y] + 1 << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

int dfs(int node) {
  int child;
  marks[node] = 1;

  // std::cout << "total children = " << graph[node * 1000] << "\n";

  for (int i = 0; i < graph[node * 1000]; i++) {
    child = graph[node * 1000 + 1 + i];
    std::cout << "node: " << node << " child = " << child << "\n";

    if (marks[child] == 0)
      return dfs(child);

    // se vai a nó que já foi visitado, tem  um ciclo
    else if (marks[child] == 1)
      return 0;
  }
  return 1;
}

bool connectivity() {
  std::cout << "n = " << n << " start_node = " << start_node << "\n";

  memset(marks, 0, sizeof(int) * 1000);

  // se contem ciclo, é invalido
  if (dfs(start_node) == 0)
    return 0;

  std::cout << "did DFS\n";

  for (int i = 0; i < n; i++) {
    // se não é conexo, há nós que não foram visitados, a partir do primeiro
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
    if (graph[i * 1000] == 0) {
      end_node = i;
      if (++null_out_count != 1)
        return 0;
    }
  }

  cout << "outdegree count = " << null_out_count << "\n";
  if (null_out_count == 0) {
    return 0;
  }

  return connectivity();
}

void single_operation() {
  // Greedy algorithm to select operation order
  /*int time = 0;
  for (int i = 0; i < n; i++) {
      time += operation_time[i];
      for (int j = 0; j < graph[i]; j++){

      }
  }*/
  memset(marks, 0, sizeof(int) * 1000);
  queue<int> q;
  marks[start_node] = 1;
  q.push(start_node);

  int t, i; //, total_time = 0;
  while (!q.empty()) {
    t = q.front();
    q.pop();

    for (i = 0; i < graph[t * 1000 + 1]; i++) {
    }
  }

  // std::cout << time << '\n';
  //  std::cout <<
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
  graph = new int[101000];
  marks = new int[1000];

  memset(operation_time, 0, sizeof(int) * 1000);
  memset(graph, 0, sizeof(int) * 101000);

  int m, i, j, op, aux;
  bool start_declared = 0;

  std::cin >> n;
  for (i = 0; i < n; i++) {
    std::cin >> operation_time[i] >> m;

    if (m == 0) { // check for multiple start nodes
      if (start_declared) {
        cout << "1 ";
        std::cout << "INVALID\n";
        return 0;

      } else {
        start_node = i;
        start_declared = 1;
      }
    } else {
      for (j = 0; j < m; j++) {
        std::cin >> op;
        op--; // convert range [1, N] to [0, N-1]

        // increment outdegree
        aux = graph[op * 1000]++;
        // insert into adjacency list
        graph[op * 1000 + 1 + aux] = i;

        //std::cout << "Inserted: " << op + 1 << " - " << i + 1 << "\n";
        //print_graph();
      }
    }
  }

  // Print list for debug
  print_graph();

  if (!check_valid()) {
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
