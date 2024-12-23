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
#include <unordered_map>
#include <cstddef>
#include <cstdint>

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

std::map<std::pair<std::string, std::string>, std::vector<std::string>> compute_sequences(std::vector<std::vector<std::string>> keypad) {
    std::map<std::string, std::pair<int, int>> positions;
    for (long long i = 0; i < keypad.size(); i++) {
        for (long long j = 0; j < keypad[i].size(); j++) {
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
            long long optimal = std::numeric_limits<long long>::max();

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
    return sequences;
}

std::vector<std::vector<std::string>> num_keypad = {
    {"7", "8", "9"},
    {"4", "5", "6"},
    {"1", "2", "3"},
    {"none", "0", "A"}};

std::map<std::pair<std::string, std::string>, std::vector<std::string>> num_sequences = compute_sequences(num_keypad);

std::vector<std::vector<std::string>> dir_keypad = {
    {"none", "^", "A"},
    {"<", "v", ">"},
};

std::map<std::pair<std::string, std::string>, std::vector<std::string>> dir_sequences = compute_sequences(dir_keypad);

std::vector<std::string> solve(std::string str, std::map<std::pair<std::string, std::string>, std::vector<std::string>> sequences) {
    std::vector<std::vector<std::string>> options = get_options(str, sequences); 
    std::vector<std::string> result = cartesian_product(options);
    return result;
}

std::map<std::pair<std::string, std::string>, long long> dir_lengths;

// unordered map for caching, hash -> optimal length
std::unordered_map<std::size_t, long long> cache;


std::size_t hash_function(const std::string& x, const std::string& y, int depth) {
    // Combine string hashes and depth into a single hash value
    std::hash<std::string> string_hasher;
    std::hash<int> int_hasher;

    std::size_t h1 = string_hasher(x);
    std::size_t h2 = string_hasher(y);
    std::size_t h3 = int_hasher(depth);

    // Combine hashes (e.g., using XOR and bit-shifting)
    return h1 ^ (h2 << 1) ^ (h3 << 2);
}

long long compute_length(const std::string& x, const std::string& y, int depth) {
    // cache
    std::size_t hash = hash_function(x, y, depth);
    auto it = cache.find(hash);
    if (it != cache.end()) {
        return it->second;
    }

    if (depth == 1) {
        return dir_lengths.at({x, y});
    }
    // Recursive case
    long long optimal = std::numeric_limits<long long>::max();
    // Get sequences for this pair
    const auto& sequences = dir_sequences.at({x, y});
    // Try each sequence
    for (const auto& seq : sequences) {
        long long length = 0;
        std::string extended_seq = "A" + seq;
        // Sum up lengths for each pair of characters
        for (size_t i = 0; i < seq.length(); i++) {
            std::string a(1, extended_seq[i]);  // Convert char to string
            std::string b(1, seq[i]);          // Convert char to string
            length += compute_length(a, b, depth-1);
        }
        optimal = std::min(optimal, length);
    }
    // Store result in cache
    cache[hash] = optimal;
    return optimal;
}

int main(int argc, char *argv[]) {
    // initialize dir_lengths
    for (const auto& pair : dir_sequences) {
        if (!pair.second.empty()) {
            dir_lengths[pair.first] = pair.second[0].length();
        }
    }

    std::vector<std::string> lines = read_input("in.txt");
    long long total = 0;

    for (auto const &line : lines) {
        std::vector<std::string> inputs = solve(line, num_sequences);
        long long optimal = std::numeric_limits<long long>::max();
        for (const auto& input : inputs) {
            long long length = 0;
            std::string extended_input = "A" + input;
            for (size_t i = 0; i < input.length(); i++) {
                std::string a(1, extended_input[i]);  // Convert char to string
                std::string b(1, input[i]);          // Convert char to string
                length += compute_length(a, b, 25);
            }
            optimal = std::min(optimal, length);
        }
        total += optimal * std::stoll(line);
    }
    std::cout << total << std::endl;
    return 0;
}

