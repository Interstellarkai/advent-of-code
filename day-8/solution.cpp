#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <vector>

class Solution {
private:
    std::fstream fin;
    std::vector<std::string> data;
    std::unordered_map<char, std::vector<std::pair<int, int> > > coords;

    /**
     * Parse the input file
     */
    void parse() {
        std::string line;
        int row = 0;
        while (std::getline(this->fin, line)) {
            this->data.push_back(line);
            for (int col = 0; col < line.size(); ++col) {
                if (line[col] != '.') {
                    this->coords[line[col]].emplace_back(row, col);
                }
            }
            row++;
        }
    }

    bool isValidPosition(const std::pair<int, int> &pair) {
        const auto [row, col] = pair;
        return row >= 0 && row < this->data.size() && col >= 0 && col < this->data[0].size();
    }

public:
    Solution(std::string fileName) {
        this->fin.open(fileName, std::ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> long long {
        std::set<std::pair<size_t, size_t> > antiNodes;
        // Handle each char at a time
        for (auto [_, values]: this->coords) {
            // Pair up nodes with each other to form anti-nodes (nested for loop)
            for (size_t fst = 0; fst < values.size() - 1; ++fst) {
                for (size_t snd = fst + 1; snd < values.size(); ++snd) {
                    // Get the difference. To simplify the logic of expanding outwards,
                    // let's imagine fst < snd in a horizontal plane
                    auto [fstRow, fstCol] = values[fst];
                    auto [sndRow, sndCol] = values[snd];
                    int rowDiff = sndRow - fstRow;
                    int colDiff = sndCol - fstCol;

                    // First anti-node (left of fst)
                    std::pair<int, int> antiNode1 = {fstRow - rowDiff, fstCol - colDiff};
                    // Second anti-node (right of snd)
                    std::pair<int, int> antiNode2 = {sndRow + rowDiff, sndCol + colDiff};

                    if (isValidPosition(antiNode1)) antiNodes.insert(antiNode1);
                    if (isValidPosition(antiNode2)) antiNodes.insert(antiNode2);
                }
            };
        }

        return antiNodes.size();
    }

    auto part2() -> long long {
        std::set<std::pair<size_t, size_t> > antiNodes;
        // Handle each char at a time
        for (auto [_, values]: this->coords) {
            // Pair up nodes with each other to form anti-nodes (nested for loop)
            for (size_t fst = 0; fst < values.size() - 1; ++fst) {
                for (size_t snd = fst + 1; snd < values.size(); ++snd) {
                    // Get the difference. To simplify the logic of expanding outwards,
                    // let's imagine fst < snd in a horizontal plane
                    auto [fstRow, fstCol] = values[fst];
                    auto [sndRow, sndCol] = values[snd];
                    int rowDiff = sndRow - fstRow;
                    int colDiff = sndCol - fstCol;

                    // First anti-node (cascade the diff multiple time)
                    std::pair<int, int> antiNode1 = {fstRow + rowDiff, fstCol + colDiff};
                    while (isValidPosition(antiNode1)) {
                        antiNodes.insert(antiNode1);
                        antiNode1.first += rowDiff;
                        antiNode1.second += colDiff;
                    }

                    // Second anti-node (cascade the diff multiple time)
                    std::pair<int, int> antiNode2 = {sndRow - rowDiff, sndCol - colDiff};
                    while (isValidPosition(antiNode2)) {
                        antiNodes.insert(antiNode2);
                        antiNode2.first -= rowDiff;
                        antiNode2.second -= colDiff;
                    }

                    // Include itself
                    if (isValidPosition({fstRow, fstCol})) antiNodes.insert({fstRow, fstCol});
                    if (isValidPosition({sndRow, sndCol})) antiNodes.insert({sndRow, sndCol});
                }
            };
        }

        return antiNodes.size();
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
