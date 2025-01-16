#include <bits/stdc++.h>

struct TupleHash {
    template <typename T>
    std::size_t operator()(const T& tuple) const {
        auto hash = std::hash<int>{}(std::get<0>(tuple));
        hash ^= std::hash<int>{}(std::get<1>(tuple)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= std::hash<int>{}(std::get<2>(tuple)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= std::hash<int>{}(std::get<3>(tuple)) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        auto hash = std::hash<int>{}(p.first);
        hash ^= std::hash<int>{}(p.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

std::vector<std::string> import_data() {
    std::vector<std::string> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        data.push_back(line);
    }
    return data;
}

int bfs(const std::vector<std::string>& data) {
    std::unordered_set<std::tuple<int, int, int, int>, TupleHash> visited;
    std::deque<std::tuple<int, int, int, int>> q;

    // Start at (-1, 0) with direction (0, 1)
    q.push_back({0, -1, 0, 1});

    while (!q.empty()) {
        auto [r, c, dr, dc] = q.front();
        q.pop_front();

        r += dr;
        c += dc;

        // Check bounds
        if (r < 0 || r >= data.size() || c < 0 || c >= data[0].size()) continue;

        // If already visited, skip
        if (visited.count({r, c, dr, dc})) continue;

        visited.insert({r, c, dr, dc});

        char cell = data[r][c];
        if (cell == '.' || (cell == '-' && dc != 0) || (cell == '|' && dr != 0)) {
            q.push_back({r, c, dr, dc});
        } else if (cell == '\\') {
            q.push_back({r, c, dc, dr}); 
        } else if (cell == '/') {
            q.push_back({r, c, -dc, -dr});
        } else if (cell == '|') {
            q.push_back({r, c, 1, 0});
            q.push_back({r, c, -1, 0});
        } else if (cell == '-') {
            q.push_back({r, c, 0, 1});
            q.push_back({r, c, 0, -1});
        } else {
            std::cerr << "Invalid cell: " << cell << std::endl;
            std::exit(1);
        }
    }

    // Count unique positions visited
    std::unordered_set<std::pair<int, int>, PairHash> visited_cells;
    for (const auto& [r, c, dr, dc] : visited) {
        visited_cells.insert({r, c});
    }
    return visited_cells.size();
}

int main() {
    std::vector<std::string> data = import_data();
    int result = bfs(data);
    std::cout << result << std::endl;
}

