#include <bits/stdc++.h>

// Custom hash function for pairs to use in unordered containers
struct PairHash {
    template <typename T, typename U>
    size_t operator()(const std::pair<T, U>& p) const {
        auto h1 = std::hash<T>{}(p.first);
        auto h2 = std::hash<U>{}(p.second);
        return h1 ^ h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    }
};

// Type aliases for readability
using Coord = std::pair<int, int>;
using Graph = std::unordered_map<Coord, std::vector<std::pair<Coord, int>>, PairHash>;

class MazeSolver {
private:
    std::vector<std::string> maze;
    Coord start, end;
    Graph graph;
    
    // Directions for movement
    const std::vector<Coord> adj = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    
    // Allowed movement directions based on tile type
    const std::map<char, std::vector<Coord>> allowed_directions = {
        {'<', {{0, -1}}},
        {'>', {{0, 1}}},
        {'^', {{-1, 0}}},
        {'v', {{1, 0}}},
        {'.', {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}}
    };

    // Find nodes with 3 or more paths
    std::vector<Coord> findIntersectionNodes() {
        std::vector<Coord> nodes;
        nodes.push_back({0, 1});
        nodes.push_back({static_cast<int>(maze.size()) - 1, static_cast<int>(maze[0].size()) - 2});
        
        for (int i = 0; i < maze.size(); i++) {
            for (int j = 0; j < maze[i].size(); j++) {
                if (maze[i][j] == '#') continue;
                
                int next = 0;
                for (auto [dx, dy] : adj) {
                    int nx = i + dx, ny = j + dy;
                    if (nx < 0 || nx >= maze.size() || ny < 0 || ny >= maze[i].size()) continue;
                    if (maze[nx][ny] != '#') next++;
                }
                
                if (next >= 3) nodes.push_back({i, j});
            }
        }
        return nodes;
    }

    // Contract edges between intersection nodes
    Graph contractEdges() {
        Graph contracted_graph;
        std::vector<Coord> nodes = findIntersectionNodes();
        
        for (auto [sr, sc] : nodes) {
            std::queue<std::tuple<int, Coord>> q;
            std::unordered_set<Coord, PairHash> visited;
            
            q.push({0, {sr, sc}});
            visited.insert({sr, sc});
            
            while (!q.empty()) {
                auto [dist, current] = q.front();
                auto [r, c] = current;
                q.pop();
                
                if (dist != 0 && std::find(nodes.begin(), nodes.end(), current) != nodes.end()) {
                    contracted_graph[{sr, sc}].push_back({current, dist});
                    continue;
                }
                
                for (auto [dx, dy] : allowed_directions.at(maze[r][c])) {
                    int nr = r + dx, nc = c + dy;
                    if (nr < 0 || nr >= maze.size() || nc < 0 || nc >= maze[r].size()) continue;
                    if (maze[nr][nc] == '#') continue;
                    if (visited.count({nr, nc})) continue;
                    
                    q.push({dist + 1, {nr, nc}});
                    visited.insert({nr, nc});
                }
            }
        }
        return contracted_graph;
    }

    // Depth-first search to find longest path
    int dfs(const Coord& current, std::unordered_set<Coord, PairHash>& visited) {
        if (current == end) return 0;
        
        int max_dist = INT_MIN;
        visited.insert(current);
        
        for (auto [next, dist] : graph[current]) {
            if (visited.count(next)) continue;
            
            int path_length = dfs(next, visited);
            if (path_length != INT_MIN) {
                max_dist = std::max(max_dist, path_length + dist);
            }
        }
        
        visited.erase(current);
        return max_dist;
    }

public:
    MazeSolver(const std::vector<std::string>& input_maze) : maze(input_maze) {
        start = {0, 1};
        end = {static_cast<int>(maze.size()) - 1, static_cast<int>(maze[0].size()) - 2};
        graph = contractEdges();
    }

    int findLongestPath() {
        std::unordered_set<Coord, PairHash> visited;
        return dfs(start, visited);
    }
};

// Input maze from stdin
std::vector<std::string> import_maze() {
    std::vector<std::string> maze;
    std::string line;
    while (std::getline(std::cin, line)) {
        maze.push_back(line);
    }
    return maze;
}

int main() {
    std::vector<std::string> maze = import_maze();
    MazeSolver solver(maze);
    std::cout << solver.findLongestPath() << std::endl;
    return 0;
}
