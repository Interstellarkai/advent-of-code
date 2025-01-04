#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>

#include "../utils/split.h"

class Solution {
 private:
  fstream fin;
  vector<int> data1, data2;

  void parse() {
    string line;
    while (getline(this->fin, line)) {
      vector<string> temp = split(line, "   ");

      this->data1.push_back(stoi(temp[0]));
      this->data2.push_back(stoi(temp[1]));
    }
  }

 public:
  Solution(string fileName) {
    this->fin.open(fileName, ios::in);
    this->parse();
    this->fin.close();
  }

  auto part1() -> int {
    int diff = 0;

    ranges::sort(this->data1);
    ranges::sort(this->data2);

    for (auto it1 = this->data1.begin(), it2 = this->data2.begin();
         it1 != this->data1.end(); ++it1, ++it2) {
      const auto a = *it1;
      const auto b = *it2;
      diff += std::abs(a - b);
    }

    return diff;
  }

  auto part2() -> int {
    int sol = 0;

    ranges::sort(this->data1);
    ranges::sort(this->data2);

    auto m = std::map<int, int>{};

    for (auto it1 = this->data1.begin(), it2 = this->data2.begin();
         it1 != this->data1.end(); ++it1, ++it2) {
      const auto b = *it2;
      ++m[b];
    }

    for (const auto v : this->data1) {
      sol += v * m[v];
    }

    return sol;
  }
};

auto main() -> int {
  Solution aoc = Solution("input.txt");
  Solution test1 = Solution("test1.txt");
  Solution test2 = Solution("test2.txt");

  std::cout << std::format("Part 1 test: {}\n", test1.part1());
  std::cout << std::format("Part 1 actual: {}\n", aoc.part1());
  std::cout << std::format("--------------------------\n");
  std::cout << std::format("Part 2 test: {}\n", test2.part2());
  std::cout << std::format("Part 2 actual: {}\n", aoc.part2());

  return 0;
}