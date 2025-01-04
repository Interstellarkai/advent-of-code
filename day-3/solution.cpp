#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

class Solution {
 private:
  std::fstream fin;
  std::string data;

  void parse() {
    std::string line;
    while (getline(this->fin, line)) {
      this->data += line;
    }
  }

 public:
  Solution(std::string fileName) {
    this->fin.open(fileName, std::ios::in);
    this->parse();
    this->fin.close();
  }

  auto part1() -> long long {
    long long sol = 0;

    const std::regex mul(R"(mul\((\d+),(\d+)\))");

    for (std::sregex_iterator i =
             std::sregex_iterator(this->data.begin(), this->data.end(), mul);
         i != std::sregex_iterator(); ++i) {
      const std::smatch& match = *i;
      const long long a = stoll(match[1]);
      const long long b = stoll(match[2]);
      const long long res = a * b;
      sol += res;
    }

    return sol;
  }

  auto part2() -> long long {
    long long sol = 0;
    std::vector<std::pair<int, std::string>> instructions;

    std::regex _do("do\\(\\)");
    for (std::sregex_iterator i =
             std::sregex_iterator(this->data.begin(), this->data.end(), _do);
         i != std::sregex_iterator(); ++i) {
      const std::smatch& match = *i;
      instructions.emplace_back(match.position(), match.str());
    }

    std::regex dont(R"(don\'t\(\))");
    for (std::sregex_iterator i =
             std::sregex_iterator(this->data.begin(), this->data.end(), dont);
         i != std::sregex_iterator(); ++i) {
      const std::smatch& match = *i;
      instructions.emplace_back(match.position(), match.str());
    }

    std::regex mul(R"(mul\((\d+),(\d+)\))");
    for (std::sregex_iterator i =
             std::sregex_iterator(this->data.begin(), this->data.end(), mul);
         i != std::sregex_iterator(); ++i) {
      const std::smatch& match = *i;
      instructions.emplace_back(match.position(), match.str());
    }

    std::ranges::sort(instructions);

    bool take = true;
    for (auto &instruction : instructions) {
      if (instruction.second == "do()") {
        take = true;
      } else if (instruction.second == "don't()") {
        take = false;
      } else {
        std::smatch match;
        regex_search(instruction.second, match, mul);
        long long a = stoll(match[1]);
        long long b = stoll(match[2]);
        if (take) sol += a * b;
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