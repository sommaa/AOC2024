#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <tuple>
#include <limits>
#include <utility>

// read the input file and return a vector of strings
std::vector<std::string> read_input(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::vector<std::string>> num_keypad = {
    {"7", "8", "9"},
    {"4", "5", "6"},
    {"1", "2", "3"},
    {"none", "0", "A"}};

std::vector<std::vector<std::string>> dir_keypad = {
    {"none", "^", "A"},
    {"<", "v", ">"},
};

std::vector<std::vector<std::string>> get_options(const std::string& str, 
    const std::map<std::pair<std::string, std::string>, std::vector<std::string>>& sequences) {
    
    std::vector<std::vector<std::string>> options;
    std::string extended_str = "A" + str;
    
    // Iterate through the pairs of characters
    for (size_t i = 0; i < str.length(); i++) {
        std::string x(1, extended_str[i]);    // Convert char to string
        std::string y(1, str[i]);             // Convert char to string
        
        // Look up the sequence for this pair
        auto key = std::make_pair(x, y);
        auto it = sequences.find(key);
        
        if (it != sequences.end()) {
            options.push_back(it->second);
        }
    }
    
    return options;
}

std::vector<std::string> cartesian_product(const std::vector<std::vector<std::string>>& vectors) {
    if (vectors.empty()) return {};
    
    // Initialize with first vector
    std::vector<std::string> result = vectors[0];
    
    // Start from second vector
    for (size_t i = 1; i < vectors.size(); i++) {
        std::vector<std::string> temp;
        for (const auto& current : result) {
            for (const auto& item : vectors[i]) {
                temp.push_back(current + item);
            }
        }
        result = std::move(temp);
    }
    return result;
}

std::vector<std::string> solve(std::string str, std::vector<std::vector<std::string>> keypad) {
    std::map<std::string, std::pair<int, int>> positions;
    for (int i = 0; i < keypad.size(); i++) {
        for (int j = 0; j < keypad[i].size(); j++) {
            if (keypad[i][j] != "none") {
                positions[keypad[i][j]] = {i, j};
            }
        }
    }
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> sequences;
    std::vector<std::string> possibilities;
    std::deque<std::tuple<int, int, std::string>> queue;
    std::string moves;
    bool break_now = false;
    for (auto const &pos_x : positions) {
        for (auto const &pos_y : positions) {
            if (pos_x == pos_y) {
                sequences[{pos_x.first, pos_y.first}] = {"A"};
                continue;
            }

            // clear the queue
            possibilities.clear();
            queue.clear();
            break_now = false;
            int optimal = std::numeric_limits<int>::max();

            queue.push_back(std::make_tuple(pos_x.second.first, pos_x.second.second, moves));
            while (queue.size() > 0 && !break_now) {
                int r = std::get<0>(queue.front());
                int c = std::get<1>(queue.front());
                std::string moves = std::get<2>(queue.front());
                queue.pop_front();
                
                std::vector<std::tuple<int, int, std::string>> moves_set = {
                    std::make_tuple(r - 1, c, "^"),
                    std::make_tuple(r + 1, c, "v"),
                    std::make_tuple(r, c - 1, "<"),
                    std::make_tuple(r, c + 1, ">")
                };

                for (auto const &move : moves_set) {
                    int nr = std::get<0>(move);
                    int nc = std::get<1>(move);
                    std::string nm = std::get<2>(move);

                    if (nr < 0 || nr >= keypad.size() || nc < 0 || nc >= keypad[0].size()) {
                        continue;
                    }

                    if (keypad[nr][nc] == "none") {
                        continue;
                    }

                    if (keypad[nr][nc] == pos_y.first) {
                        if (optimal < moves.size() + 1) {
                            break_now = true;
                            break;
                        }
                        optimal = moves.size() + 1;
                        possibilities.push_back(moves + nm + "A");
                    } else {
                        queue.push_back(std::make_tuple(nr, nc, moves + nm));
                    }
                } 
            } 
            sequences[{pos_x.first, pos_y.first}] = possibilities;
        }
    }
    std::vector<std::vector<std::string>> options = get_options(str, sequences); 
    std::vector<std::string> result = cartesian_product(options);
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> lines = read_input("test.txt");
    int total = 0;

    std::vector<std::string> possible_robot_next;
    for (auto const &line : lines) {
        std::vector<std::string> robot_1 = solve(line, num_keypad);
        std::vector<std::string> robot_next = robot_1;

        for (int i = 0; i < 2; i++) {
            possible_robot_next.clear();
            for (auto const &r : robot_next) {
                std::vector<std::string> robot_next = solve(r, dir_keypad);
                for (auto const &rnext : robot_next) {
                    possible_robot_next.push_back(rnext);
                }
            }
            // filter out the possibilities have a length higher than the lowest sequence 
            int min_length = std::numeric_limits<int>::max();
            for (auto const &rnext : possible_robot_next) {
                if (rnext.size() < min_length) {
                    min_length = rnext.size();
                }
            }
            possible_robot_next.erase(std::remove_if(possible_robot_next.begin(), possible_robot_next.end(), 
                [min_length](const std::string &s) { return s.size() > min_length; }), possible_robot_next.end());
            robot_next = possible_robot_next;
        }
        // convert the digit part of the line to int 
        int digit = std::stoi(line);
        total += robot_next.front().size() * digit;
        std::cout << robot_next.front().size() << std::endl;
    }
    std::cout << total << std::endl;
    return 0;
}
