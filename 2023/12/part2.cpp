#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

// Function to import text
void import_text(std::vector<std::string>& springs, std::vector<std::vector<int>>& sequences) {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::string spring = line.substr(0, line.find(" "));
        std::string sequence = line.substr(line.find(" ") + 1);
        springs.push_back(spring);
        std::vector<int> seq;
        while (sequence.find(",") != std::string::npos) {
            seq.push_back(std::stoi(sequence.substr(0, sequence.find(","))));
            sequence = sequence.substr(sequence.find(",") + 1);
        }
        seq.push_back(std::stoi(sequence));
        sequences.push_back(seq);
    }
}

struct StateHash {
    std::size_t operator()(const std::tuple<size_t, size_t, size_t>& t) const {
        const uint64_t prime1 = 0x100000001b3;          // Large prime for mixing
        const uint64_t prime2 = 0x2e63823a35;           // Another large prime
        const uint64_t prime3 = 0x1e35a7bd34;           // Third prime for better distribution
        
        auto [pos, group_idx, current] = t;
        
        uint64_t hash = 0xcbf29ce484222325;             // FNV offset basis
        
        // Mix each component with different primes
        hash ^= pos + prime1;
        hash *= prime2;
        hash ^= group_idx + prime3;
        hash *= prime1;
        hash ^= current + prime2;
        hash *= prime3;
        
        // Final mixing step
        hash ^= hash >> 33;
        hash *= 0xff51afd7ed558ccd;
        hash ^= hash >> 33;
        hash *= 0xc4ceb9fe1a85ec53;
        hash ^= hash >> 33;
        
        return static_cast<std::size_t>(hash);
    }
};

// Main solver class to avoid global variables and improve locality
class SpringSolver {
private:
    std::unordered_map<std::tuple<size_t, size_t, size_t>, int64_t, StateHash> cache;
    std::string pattern;
    std::vector<int> groups;

    int64_t solve(size_t pos, size_t group_idx, size_t current) {
        // Check cache first
        auto key = std::make_tuple(pos, group_idx, current);
        auto it = cache.find(key);
        if (it != cache.end()) return it->second;

        // Base cases
        if (pos == pattern.size()) {
            if (group_idx == groups.size() && current == 0) return 1;
            if (group_idx == groups.size() - 1 && groups[group_idx] == current) return 1;
            return 0;
        }

        int64_t result = 0;
        char c = pattern[pos];

        // Try placing a working spring
        if (c == '.' || c == '?') {
            if (current == 0) {
                result += solve(pos + 1, group_idx, 0);
            }
            else if (current > 0 && group_idx < groups.size() && groups[group_idx] == current) {
                result += solve(pos + 1, group_idx + 1, 0);
            }
        }

        // Try placing a damaged spring
        if ((c == '#' || c == '?') && (group_idx < groups.size())) {
            if (current < groups[group_idx]) {
                result += solve(pos + 1, group_idx, current + 1);
            }
        }

        cache[key] = result;
        return result;
    }

public:
    int64_t count_ways(const std::string& input_pattern, const std::vector<int>& input_groups) {
        pattern = input_pattern;
        groups = input_groups;
        cache.clear();
        return solve(0, 0, 0);
    }
};

// Helper function to repeat string with minimal allocations
std::string repeat_string(const std::string& s, int times) {
    std::string result;
    result.reserve(s.length() * times + times - 1);
    for (int i = 0; i < times; ++i) {
        if (i > 0) result += '?';
        result += s;
    }
    return result;
}

// Helper function to repeat vector with minimal allocations
std::vector<int> repeat_vector(const std::vector<int>& v, int times) {
    std::vector<int> result;
    result.reserve(v.size() * times);
    for (int i = 0; i < times; ++i) {
        result.insert(result.end(), v.begin(), v.end());
    }
    return result;
}

int main() {
    std::vector<std::string> springs;
    std::vector<std::vector<int>> sequences;
    import_text(springs, sequences);
    
    SpringSolver solver;
    int64_t total = 0;
    
    for (size_t i = 0; i < springs.size(); ++i) {
        std::string expanded_spring = repeat_string(springs[i], 5);
        std::vector<int> expanded_sequence = repeat_vector(sequences[i], 5);
        total += solver.count_ways(expanded_spring, expanded_sequence);
    }
    
    std::cout << total << std::endl;
    return 0;
}
