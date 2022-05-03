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

bool DEBUG = 1;

int *operation_time, *graph, *marks, start_node, end_node, n, total_time = 0;
deque<int> q;
int *recursion_stack;

void print_graph() {
  for (int x = 0; x < n; x++) {
    std::cout << x + 1 << " -> ";
    for (int y = 0; y < graph[x * 1000]; y++) {
      std::cout << graph[x * 1000 + 2 + y] + 1 << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

bool dfs(int node) {
  int child;

  marks[node] = 1;
  recursion_stack[node] = 1;

  for (int i = 0; i < graph[node * 1000]; i++) {
    child = graph[node * 1000 + 2 + i];

    if (DEBUG)
      std::cout << "node: " << node + 1 << " child = " << child + 1 << "\n";

    // Check for back edges
    // If node is visited and in the stack, it is a back edge
    // The graph can't have back edges
    if (marks[child] == 0) {
      if (dfs(child) == 0)
        return 0;
    } else if (recursion_stack[child] == 1) {
      return 0;
    }
  }

  recursion_stack[node] = 0; // remove node from stack
  return 1;
}

// implementation of a topological sort with dfs
void topological_dfs(int node) {
  int child;
  marks[node] = 1;

  for (int i = graph[node * 1000]; i >= 0; i--) {
    child = graph[node * 1000 + 2 + i];
    if (marks[child] == 0)
      topological_dfs(child);
  }

  if (DEBUG)
    std::cout << "Pushed " << node + 1 << "\n";

  q.push_front(node + 1);
}

void topological_dfs_2(int node) {
  int temp = graph[node * 1000];//, best = 0;
  marks[node] = 1;
  // int child = graph[node * 1000 + 2 + i];

  // If has more than one child
  if (temp > 1) {
    for (int j = 0; j < temp - 1; j++) {
      std::cout << graph[node + j * 1000 + 1] << " == " << graph[node + j + 1 * 1000 + 1] << "\n";
      // if (graph[node * 1000 + 2 + j] == graph[(node + j + 1) * 1000 + 1]) {
      //   std::cout << "CENAS\n";
      // }
    }
  } else {
    topological_dfs_2(graph[node * 1000 + 2]);
  }
}

void topological_sorting() {
  deque<int> dq;

  // Calculate indgrees
}

bool connectivity() {
  if (DEBUG)
    std::cout << "n = " << n << " start_node = " << start_node + 1 << "\n";

  memset(marks, 0, sizeof(int) * n);

  // se contem ciclo, é invalido
  if (dfs(start_node) == 0)
    return 0;

  if (DEBUG)
    std::cout << "did DFS\n";

  for (int i = 0; i < n; i++) {
    // se não é conexo, há nós que não foram visitados, a partir do primeiro
    if (marks[i] == 0) {
      if (DEBUG)
        std::cout << "node " << i + 1 << " not marked\n";
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

  // srd::cout << "outdegree count = " << null_out_count << "\n";
  if (null_out_count == 0) {
    return 0;
  }

  return connectivity();
}

void single_operation() {
  memset(marks, 0, sizeof(int) * n);

  // for (int i = 0; i < n; i++) {
  // if (marks[i] == 0)
  topological_dfs(start_node);
  //}

  std::cout << total_time << '\n';

  while (!q.empty()) {
    std::cout << q.front() << "\n";
    q.pop_front();
  }
}

void multi_operation() {
  memset(marks, 0, sizeof(int) * n);
  // total_time = operation_time[start_node];

  topological_dfs_2(start_node);
  // for (int i = 0; i < n; i++) {
  //   // graph[i * 1000 + 2 + j];
  // }

  std::cout << total_time << "\n";
}

void find_bottleneck() { memset(marks, 0, sizeof(int) * n); }

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(0);

  operation_time = new int[1000];
  graph = new int[101000];
  marks = new int[1000];
  recursion_stack = new int[1000];

  memset(operation_time, 0, sizeof(int) * 1000);
  memset(graph, 0, sizeof(int) * 100000);
  memset(recursion_stack, 0, sizeof(int) * 1000);

  /*
    graph[i * 1000] -> outdegree (number of children)
    graph[i * 1000 + 1] -> indegree
    graph[i * 1000 + 2 + n] -> children of node i, n => [0, 9]
  */

  int m, i, j, op, aux;
  bool start_declared = 0;

  std::cin >> n;
  for (i = 0; i < n; i++) {
    std::cin >> operation_time[i] >> m;

    // calculate total_time as input is read
    total_time += operation_time[i];

    // calculate indgree
    graph[i * 1000 + 1] = m;

    if (m == 0) {
      // check for multiple start nodes
      if (start_declared) {
        std::cout << "INVALID\n";
        return 0;

      } else {
        start_node = i;
        start_declared = 1;
      }
    } else {
      // TODO: maybe insertion sort? Check where to insert node before insert
      for (j = 0; j < m; j++) {
        std::cin >> op;
        op--; // convert range from [1, N] to [0, N-1]

        // increment outdegree
        aux = graph[op * 1000]++;

        // insert into adjacency list
        graph[op * 1000 + 2 + aux] = i;

        // if (aux == 1) { // base case -> empty list => insert
        //   graph[op * 1000 + 2 + aux] = i;
        // } else {
        //   for (int k = 0; k < m; k++) {
        //     if (i < graph[op * 1000 + 2 + k]){
        //       // swap
        //     } else {
        //       graph[op * 1000 + 2 + k] = i;
        //     }
        //   }
        // }

        // std::cout << "Inserted: " << op + 1 << " - " << i + 1 << "\n";
        // print_graph();
      }
    }
  }

  // Print list for debuging
  if (DEBUG)
    print_graph();

  // Print indgree for debugging
  // std::cout << "Indgrees:\n";
  // for (i = 0; i < n; i++) {
  //   std::cout << i + 1 << " - " << graph[i * 1000 + 1] << "\n";
  // }

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
    // std::cout << "Operation 1\n";
    single_operation();
    break;
  case 2:
    multi_operation();
    // std::cout << "Operation 2\n";
    break;
  case 3:
    find_bottleneck();
    // std::cout << "Operation 3\n";
    break;
  }

  return 0;
}
