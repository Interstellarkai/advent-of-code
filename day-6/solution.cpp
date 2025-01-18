#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <unordered_set>
#include <vector>

enum class Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};

const std::vector<std::pair<int, int> > Movement = {
    {-1, 0}, // up
    {0, 1}, // right
    {1, 0}, // down
    {0, -1}, // left
};

class Solution {
private:
    std::fstream fin;
    std::vector<std::string> data;

    // Guard
    std::pair<int, int> initPosition;
    Direction initDirection;

    /**
     * Parse the input file
     */
    void parse() {
        std::string line;
        while (getline(this->fin, line)) {
            for (int i = 0; i < line.size(); i++) {
                switch (line[i]) {
                    case '^':
                        this->initPosition = {this->data.size(), i};
                        this->initDirection = Direction::UP;
                        break;
                    case '>':
                        this->initPosition = {this->data.size(), i};
                        this->initDirection = Direction::RIGHT;
                        break;
                    case 'v':
                        this->initPosition = {this->data.size(), i};
                        this->initDirection = Direction::DOWN;
                        break;
                    case '<':
                        this->initPosition = {this->data.size(), i};
                        this->initDirection = Direction::LEFT;
                        break;
                }
            }
            data.emplace_back(line);
        }
    }

    /**
     * Split a string by a delimiter
     * @param s The string to split
     * @param delimiter The delimiter to split by
     * @return A vector of integers
     */
    std::vector<int> split(const std::string &s, const std::string &delimiter) {
        std::vector<int> tokens;
        int start = 0, end = 0;
        while ((end = s.find(delimiter, start)) != std::string::npos) {
            tokens.push_back(std::stoi(s.substr(start, end - start)));
            start = end + delimiter.size();
        }
        tokens.push_back(std::stoi(s.substr(start)));
        return tokens;
    }

    /**
     * Check if a coordinate is out of bound
     * @param x The x-coordinate
     * @param y The y-coordinate
     * @return True if out of bound, false otherwise
     */
    bool outOfBound(const int x, const int y) {
        return x < 0 || x >= this->data.size() || y < 0 || y >= this->data[0].size();
    };

    /*
     * Get the route of the Santa
     * @param ignoreDirection Whether to ignore the direction of the Santa
     * @return A set of coordinates
     */
    std::pair<std::unordered_set<std::string>, bool> getRoute(bool ignoreDirection = true) {
        // Initialize
        std::pair<int, int> curPosition = this->initPosition;
        Direction curDirection = this->initDirection;

        // Keep track of visited coordinates
        bool isLooping = false;
        std::unordered_set<std::string> visited{
            std::to_string(initPosition.first) + "," + std::to_string(initPosition.second)
        };

        // Traverse the path
        while (!outOfBound(curPosition.first, curPosition.second)) {
            // Moving forward
            std::pair<int, int> nextPosition = {
                curPosition.first + Movement[static_cast<int>(curDirection)].first,
                curPosition.second + Movement[static_cast<int>(curDirection)].second
            };

            // If forward is "#", then turn right
            while (this->data[nextPosition.first][nextPosition.second] == '#') {
                curDirection = static_cast<Direction>((static_cast<int>(curDirection) + 1) % 4); // Rotate clockwise
                nextPosition = {
                    curPosition.first + Movement[static_cast<int>(curDirection)].first,
                    curPosition.second + Movement[static_cast<int>(curDirection)].second
                };
            }

            // Move in the current direction
            curPosition = nextPosition;
            std::string coordinate;
            // part 1
            if (ignoreDirection) {
                coordinate = std::format("{},{}",
                                         curPosition.first,
                                         curPosition.second);
            }
            // part 2
            else {
                coordinate = std::format("{},{},{}",
                                         curPosition.first,
                                         curPosition.second,
                                         static_cast<int>(curDirection));
                if (visited.contains(coordinate)) {
                    isLooping = true;
                    break;
                }
            }
            visited.insert(coordinate);
        }
        return {visited, isLooping};
    }

public:
    Solution(std::string fileName) {
        this->fin.open(fileName, std::ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> long long {
        return getRoute().first.size() - 1;
    }

    // Modified version of part 1 (visited now tracks direction, and we only place obstacle if it's on the OG path)
    auto part2() -> long long {
        std::set<std::pair<int, int>> result;
        const auto originalRoute = getRoute(false).first;
        for (const std::string &coordinate: originalRoute) {
            const auto points = split(coordinate, ",");
            const int x = points[0], y = points[1];

            this->data[x][y] = '#';
            // backtrack
            if (getRoute(false).second) {
                result.insert({x, y});
            }
            this->data[x][y] = '.';
        }
        return result.size();
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
