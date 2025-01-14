#include <bits/stdc++.h>

std::vector<std::string> import_data() {
    std::vector<std::string> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        data.push_back(std::move(line));  // Use move semantics
    }
    return data;
}

const static std::vector<std::pair<int, int>> adjs = {
    {-1, 0}, {0, -1}, {1, 0}, {0, 1}
};

std::vector<std::pair<int, int>> get_mov_rocks(const std::vector<std::string>& data,
                                              const std::pair<int, int>& adj) {
    std::vector<std::pair<int, int>> rocks;
    rocks.reserve(data.size() * data[0].size() / 4);
    const int rows = data.size();
    const int cols = data[0].size();
    
    const int new_row_min = -adj.first;
    const int new_row_max = rows - adj.first;
    const int new_col_min = -adj.second;
    const int new_col_max = cols - adj.second;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (data[i][j] == 'O') {
                if (i >= new_row_min && i < new_row_max &&
                    j >= new_col_min && j < new_col_max &&
                    data[i + adj.first][j + adj.second] == '.') {
                    rocks.emplace_back(i, j);
                }
            }
        }
    }
    return rocks;
}

void cycle_rocks(std::vector<std::string>& data) {
    const int rows = data.size();
    const int cols = data[0].size();
    
    for (const auto& adj : adjs) {
        std::vector<std::pair<int, int>> mov_rocks = get_mov_rocks(data, adj);
        while (!mov_rocks.empty()) {  // Use empty() instead of size() > 0
            for (const auto& rock : mov_rocks) {
                data[rock.first][rock.second] = '.';
                data[rock.first + adj.first][rock.second + adj.second] = 'O';
            }
            mov_rocks = get_mov_rocks(data, adj);
        }
    }
}

int calc_weight(const std::vector<std::string>& data) {
    int weight = 0;
    const int rows = data.size();
    
    for (int i = 0; i < rows; i++) {
        weight += (rows - i) * std::count(data[i].begin(), data[i].end(), 'O');
    }
    return weight;
}

std::pair<std::vector<int>, int> findRepeatingSequence(const std::vector<int>& vec) {
    const int n = vec.size();
    for (int len = 1; len <= n / 2; ++len) {
        bool isRepeating = true;
        const int* const end_ptr = vec.data() + n;
        const int* const start_ptr = end_ptr - len;
        
        for (int i = 0; i < len && isRepeating; ++i) {
            for (const int* ptr = end_ptr - len; ptr >= vec.data() + len; ptr -= len) {
                if (*(ptr + i - len) != *(ptr + i)) {
                    isRepeating = false;
                    break;
                }
            }
        }
        
        if (isRepeating) {
            return {std::vector<int>(start_ptr, end_ptr), len};
        }
    }
    return {{}, 0};
}

int main() {
    std::ios_base::sync_with_stdio(false); 
    std::cin.tie(nullptr);
    
    auto data = import_data();
    bool sequence_found = false;
    std::vector<int> recorded_weights;
    std::vector<int> repeating_sequence;
    int index = 0;
    int len = 0;
    const long long num_cycles = 1000000000;
    
    while (!sequence_found) {
        cycle_rocks(data);
        recorded_weights.push_back(calc_weight(data));
        
        if (recorded_weights.size() > 50) {
            auto [seq, seq_len] = findRepeatingSequence(recorded_weights);
            len = seq_len;
            if (len > 0) {
                repeating_sequence = std::move(seq);
                sequence_found = true;
            }
        }
        index++;
    }
    
    const long long offset = static_cast<long long>(index % len);
    const long long target_index = (num_cycles - offset) % len - 1;
    
    std::cout << repeating_sequence[target_index] << '\n' << index << '\n';
    
    return 0;
}
