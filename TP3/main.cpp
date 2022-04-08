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

int *operation_time;

bool check_valid() { return true; }

void single_operation() {}

void multi_operation() {}

void find_bottleneck() {}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(0);

  operation_time = new int[1000];
  memset(operation_time, 0, sizeof(int) * 1000);

  int n, m, op;
  std::cin >> n;

  for (int i = 0; i < n; i++) {
    std::cin >> operation_time[i] >> m;
    // std::cout << "Read: " << operation_time[i] << " " << m << "\n";

    for (int j = 0; j < m; j++) {
      std::cin >> op;
      // std::cout << "op = " << op << "\n";
    }
  }

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

  std::cin >> n;
  switch (n) {
  case 0:
    std::cout << "Operation 0\n";
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
