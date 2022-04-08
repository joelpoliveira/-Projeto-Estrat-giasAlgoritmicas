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

int *values, *graph, *dp;

int min(int a, int b) { return a < b ? a : b; }

int resolve(int node_now, int is_watcher) {
  int graph_index = node_now * 10, dp_index = node_now * 4;

  // if is leaf
  if (graph[graph_index] == -1) {
    if (is_watcher) {
      dp[dp_index + 3] = values[node_now];
    } else {
      dp[dp_index + 2] = 0;
    }
    return is_watcher;
  } else if (dp[dp_index + is_watcher] != -1) {
    return dp[dp_index + is_watcher];
  }

  int res = 0, sum = 0, i, j, next_is_watcher, next_is_not, next_dp_index;

  for (i = 0; i < 10; i++) {
    j = graph_index + i;
    if (graph[j] == -1)
      break;

    next_dp_index = graph[j] * 4;

    if (is_watcher) {
      next_is_watcher = resolve(graph[j], 1);
      next_is_not = resolve(graph[j], 0);
      if (next_is_watcher == next_is_not) {
        res += next_is_watcher;

        // if path uses same nodes, use the max valued;
        if (dp[next_dp_index + 3] > dp[next_dp_index + 2]) {
          sum += dp[next_dp_index + 3];
        } else {
          sum += dp[next_dp_index + 2];
        }
      } else if (next_is_watcher < next_is_not) {
        res += next_is_watcher;
        sum += dp[next_dp_index + 3];
      } else {
        res += next_is_not;
        sum += dp[next_dp_index + 2];
      }
    } else {
      res += resolve(graph[j], 1);
      sum += dp[next_dp_index + 3];
    }
  }
  dp[dp_index + is_watcher] = res + is_watcher;
  if (is_watcher)
    dp[dp_index + 3] = sum + values[node_now];
  else
    dp[dp_index + 2] = sum;
  return dp[dp_index + is_watcher];
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(0);

  int node_now, other_node, v_index, res0, res1;

  values = new int[100000];
  dp = new int[400000];
  graph = new int[1000000];

  memset(values, 0, sizeof(int) * 100000);
  memset(dp, -1, sizeof(int) * 400000);
  memset(graph, -1, sizeof(int) * 1000000);

  while (cin >> node_now) {
    // cin >> node_now;
    v_index = 0;

    if (node_now == -1) {
      res0 = resolve(0, 0);
      res1 = resolve(0, 1);
      // cout<<dp[0]<<" "<<dp[1]<<" "<<dp[2]<<" "<<dp[3]<<"\n";
      if (res0 == res1) {
        if (dp[3] > dp[2]) {
          cout << res1 << " " << dp[3] << "\n";
        } else {
          cout << res0 << " " << dp[2] << "\n";
        }
      } else if (res0 < res1) {
        cout << res0 << " " << dp[2] << "\n";
      } else {
        cout << res1 << " " << dp[3] << "\n";
      }

      // reset data;
      memset(values, 0, sizeof(int) * 100000);
      memset(dp, -1, sizeof(int) * 400000);
      memset(graph, -1, sizeof(int) * 1000000);
    } else {
      while ((cin >> other_node)) {
        if (cin.peek() == '\n') {
          // assign node value;
          values[node_now] = other_node;
          break;
        } else {
          graph[node_now * 10 + v_index++] = other_node;
        }
      }
    }
  }
  delete[] values;
  delete[] dp;
  delete[] graph;
  return 0;
}