#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

const std::string PARSE_INITIAL = ": ";
const std::string PARSE_WHITESPACE = " ";


class Solution {
private:
    std::fstream fin;
    std::vector<std::pair<long, std::vector<int> > > data;

    /**
     * Parse the input file
     */
    void parse() {
        try {
            std::string line;
            while (std::getline(this->fin, line)) {
                size_t start = 0, end = 0;
                auto [fst, snd] = std::pair<long, std::vector<int> >();
                // left
                end = line.find(PARSE_INITIAL, start);
                fst = std::stol(line.substr(start, end - start));
                start = end + PARSE_INITIAL.size();
                // right
                auto values = std::vector<int>();
                while ((end = line.find(PARSE_WHITESPACE, start)) != std::string::npos) {
                    values.emplace_back(std::stoi(line.substr(start, end - start)));
                    start = end + PARSE_WHITESPACE.size();
                }
                // leftover
                values.emplace_back(std::stoi(line.substr(start)));
                snd = values;
                this->data.emplace_back(fst, snd);
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }
    }

    // backtrack (dfs, O(options^n) => part 1: O(2^n), part 2: O(3^n))
    bool valid (const std::pair<long, std::vector<int> > &pair, long current, size_t index, bool concat = false) {
        if (index == pair.second.size()) {
            return pair.first == current;
        }

        // multiply
        if (valid(pair, current * pair.second[index], index + 1, concat)) {
            return true;
        }
        // addition
        if (valid(pair, current + pair.second[index], index + 1, concat)) {
            return true;
        }

        // concatenation
        if (concat) {
            // padding
            for (int copy = pair.second[index]; copy > 0; copy /= 10) {
                current *= 10;
            }
            if (valid(pair, current  + pair.second[index], index + 1, concat)) {
                return true;
            }
        }

        return false;
    }

public:
    Solution(std::string fileName) {
        this->fin.open(fileName, std::ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> long long {
        long long result = 0;
        for (const auto &pair : this->data) {
            if (valid(pair, 0, 0)) {
                result += pair.first;
            }
        }
        return result;
    }

    auto part2() -> long long {
        long long result = 0;
        for (const auto &pair : this->data) {
            if (valid(pair, 0, 0, true)) {
                result += pair.first;
            }
        }
        return result;
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
