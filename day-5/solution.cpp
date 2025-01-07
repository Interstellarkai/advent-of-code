#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

class Solution {
private:
    std::fstream fin;
    /*
     * rules: A vector of prerequisite pairs, where pair.first must come before pair.second.
     * orderings: A vector of order sequences to validate against the rules.
     */
    std::vector<std::pair<int, int> > rules;
    std::vector<std::vector<int> > orderings;

    // duplicate and customized version of utils/split.h
    std::vector<int> split(const std::string &s,
                           const std::string &delimiter) {
        std::vector<int> result;
        if (s.empty()) return result;

        size_t start = 0, end = 0;
        while ((end = s.find(delimiter, start)) != std::string::npos) {
            result.emplace_back(std::stoi(s.substr(start, end)));
            start = end + delimiter.length();
        }
        // the last element
        result.emplace_back(std::stoi(s.substr(start)));

        return result;
    };

    void parse() {
        std::string line;
        bool rules = true;
        while (getline(this->fin, line)) {
            if (line.empty()) {
                rules = false;
                continue;
            }

            // section 1
            if (rules) {
                std::vector<int> parts = split(line, "|");
                this->rules.emplace_back(parts[0], parts[1]);
            }
            // section 2
            else {
                std::vector<int> parts = split(line, ",");
                std::vector<int> ordering(parts.size(), 0);
                for (size_t i = 0; i < parts.size(); ++i) {
                    ordering[i] = parts[i];
                }
                this->orderings.emplace_back(ordering);
            }
        }
    }

    /* x|y : x must happen before y
     * Graph representation:
     * - Each node is a page
     * - Each edge is a page ordering rule
     *
     * Calculate In-Degree:
     * - Count the number of edges that point to a node
     * - If a node has an in-degree of 0, it can be the first page
     * - If a node has an in-degree of 1, it can be the second page
     * - Use an array to track the number of prerequisites (in-degree) for each page
     *
     * Topological Sort (BFS):
     * - Start by adding all nodes with no prerequisites (in-degree = 0) to the queue
     * - Process each node by "completing" it, (removing it from the graph), reducing the in-degree of its dependent courses.
     * - Add courses with in-degree = 0 back to the queue
     * - Repeat until the queue is empty
     *
     * Cycle Check:
     * - If the graph has a cycle, it is impossible to complete all courses
     *
     * Approach:
     * Build a graph from rules using an adjacency list representation.
     * Use Kahn's Algorithm for topological sorting:
     * Calculate the in-degree of each node.
     * Start with nodes with in-degree = 0.
     * Gradually remove nodes from the graph as they are "processed."
     * Validate each ordering by ensuring the sequence matches the topological order of the graph.
     */
    std::vector<int> getTopoOrder(const std::vector<std::pair<int, int> > &rules, const std::vector<int> &ordering) {
        // Build the graph and calculate in-degrees
        std::unordered_map<int, std::vector<int> > graph;
        std::unordered_map<int, int> inDegree;

        for (const auto &node: ordering) {
            graph[node] = {};
            inDegree[node] = 0;
        }

        for (const auto &rule: rules) {
            int prerequisite = rule.first;
            int thereafter = rule.second;
            // padded if statement for the case where the node is not in the ordering
            if (graph.contains(prerequisite) && graph.contains(thereafter)) {
                graph[prerequisite].push_back(thereafter);
                inDegree[thereafter]++;
            }
        }

        // Create a queue for nodes with in-degree = 0
        std::deque<int> queue;
        for (const auto &entry: inDegree) {
            if (entry.second == 0) {
                queue.push_back(entry.first);
            }
        }

        // Generate a valid topological order (BFS - queue)
        std::vector<int> sortedTopoOrder;
        while (!queue.empty()) {
            int node = queue.front();
            queue.pop_front();

            sortedTopoOrder.push_back(node);

            for (const auto &neighbor: graph[node]) {
                inDegree[neighbor]--;
                if (inDegree[neighbor] == 0) {
                    queue.push_back(neighbor);
                }
            }
        }

        // If not all nodes are visited, there is a cycle
        // if (sortedTopoOrder.size() != ordering.size()) {
        //     return {};
        // }

        return sortedTopoOrder;
    }

    int middle(const std::vector<int> &updateOrdering) {
        return updateOrdering[updateOrdering.size() / 2];
    };

public:
    Solution(std::string fileName) {
        this->fin.open(fileName, std::ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> long long {
        long long sol = 0;

        for (const auto &ordering: this->orderings) {
            // feed the rules and ordering to get the correct topological order
            std::vector<int> sortedTopoOrder = getTopoOrder(this->rules, ordering);
            if (sortedTopoOrder == ordering) {
                sol += middle(ordering);
            }
        }

        return sol;
    };

    auto part2() -> long long {
        long long sol = 0;

        for (const auto &ordering: this->orderings) {
            // feed the rules and ordering to get the correct topological order
            std::vector<int> sortedTopoOrder = getTopoOrder(this->rules, ordering);
            if (sortedTopoOrder != ordering) {
                sol += middle(sortedTopoOrder);
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
