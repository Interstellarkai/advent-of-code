#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

class Solution {
 private:
  std::fstream fin;
  std::vector<std::string> data;

  void parse() {
    std::string line;
    while (getline(this->fin, line)) {
      this->data.emplace_back(line);
    }
  }

 public:
  Solution(std::string fileName) {
    this->fin.open(fileName, std::ios::in);
    this->parse();
    this->fin.close();
  }

  auto part1() -> int {
    int sol = 0;
    const std::string word = "XMAS";

    // Define all 8 possible directions
    const std::vector<std::pair<int, int>> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

    // DFS function with a fixed direction
    std::function<int(size_t, size_t, size_t, int)> dfs;
    dfs = [&](size_t i, size_t j, size_t pos, int dirIndex) -> bool {
      // If out of bounds or character doesn't match, terminate
      if (!(0 <= i && i < this->data.size()) ||
          !(0 <= j && j < this->data[0].size()) ||
          this->data[i][j] != word[pos]) {
        return false;
      }

      // If we've matched the entire word
      if (pos == word.size() - 1) {
        return true;
      }

      // Move in the same direction
      const auto [di, dj] = directions[dirIndex];
      return dfs(i + di, j + dj, pos + 1, dirIndex);
    };

    // Iterate over all cells
    for (size_t i = 0; i < this->data.size(); ++i) {
      for (size_t j = 0; j < this->data[i].size(); ++j) {
        if (this->data[i][j] != 'X') {
          continue;
        }
        // Start a DFS in all 8 directions
        for (size_t dirIndex = 0; dirIndex < directions.size(); ++dirIndex) {
          sol += dfs(i, j, 0, dirIndex) ? 1 : 0;
        }
      }
    }

    return sol;
  }

  auto part2() -> long long {
    long long sol = 0;

    auto isXMas = [&](size_t i, size_t j) -> bool {
      if (this->data[i][j] != 'A') return false;

      auto checkGradient = [&](int di, int dj) -> bool {
        return (this->data[i + di][j + dj] == 'M' && this->data[i - di][j - dj] == 'S') ||
               (this->data[i + di][j + dj] == 'S' && this->data[i - di][j - dj] == 'M');
      };

      return checkGradient(1, 1) && checkGradient(1, -1);
    };

    for (size_t i = 1; i < this->data.size() - 1; ++i) {
      for (size_t j = 1; j < this->data[i].size() - 1; ++j) {
        if (isXMas(i, j)) sol++;
      }
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
