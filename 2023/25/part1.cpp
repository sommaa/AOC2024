#include <bits/stdc++.h>

struct string_hash {
    size_t operator()(const std::string &str) const {
        size_t hash = 14695981039346656037ULL;
        for (char c : str) {
            hash ^= c;
            hash *= 1099511628211ULL;
        }
        return hash;
    }
};

using node_map =
    std::unordered_map<std::string, std::vector<size_t>, string_hash>;
using hash_to_string =
    std::unordered_map<size_t, std::string, std::hash<size_t>>;

size_t hash_and_store(const std::string &str,
                      hash_to_string &hash_to_string_map) {
    static size_t next_hash = 0;
    static std::unordered_map<std::string, size_t, string_hash> string_to_hash;

    auto it = string_to_hash.find(str);
    if (it != string_to_hash.end()) {
        return it->second;
    }

    size_t hash = next_hash++;
    string_to_hash[str] = hash;
    hash_to_string_map[hash] = str;
    return hash;
}

// Add a helper function to add bidirectional edges
void add_edge(node_map &connections, const std::string &from, size_t from_hash,
              const std::string &to, size_t to_hash) {
    connections[from].push_back(to_hash);
    connections[to].push_back(from_hash);
}

node_map parse_input(hash_to_string &hash_to_string_map) {
    node_map connections;
    std::string line;

    while (std::getline(std::cin, line) && !line.empty()) {
        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
            continue;

        std::string key = line.substr(0, colon_pos);
        std::string values_str = line.substr(colon_pos + 1);
        std::stringstream ss(values_str);
        std::string value;

        size_t key_hash = hash_and_store(key, hash_to_string_map);

        while (ss >> value) {
            size_t value_hash = hash_and_store(value, hash_to_string_map);
            // Add bidirectional edges
            add_edge(connections, key, key_hash, value, value_hash);
        }
    }
    return connections;
}

std::vector<std::string>
find_shortest_path(const node_map &connections, const std::string &start,
                   const std::string &end,
                   const hash_to_string &hash_to_string_map) {
    size_t start_hash =
        hash_and_store(start, const_cast<hash_to_string &>(hash_to_string_map));
    size_t end_hash =
        hash_and_store(end, const_cast<hash_to_string &>(hash_to_string_map));

    std::queue<size_t> node_queue;
    std::unordered_map<size_t, size_t> parent_nodes;
    std::unordered_set<size_t> visited_nodes;

    node_queue.push(start_hash);
    visited_nodes.insert(start_hash);

    while (!node_queue.empty()) {
        size_t current_node = node_queue.front();
        node_queue.pop();

        // Check if we've reached the target
        if (current_node == end_hash) {
            break;
        }

        // Get the string representation of the current node
        std::string current_str = hash_to_string_map.at(current_node);
        auto it = connections.find(current_str);
        if (it == connections.end())
            continue;

        for (size_t neighbor_node : it->second) {
            if (visited_nodes.find(neighbor_node) == visited_nodes.end()) {
                node_queue.push(neighbor_node);
                visited_nodes.insert(neighbor_node);
                parent_nodes[neighbor_node] = current_node;
            }
        }
    }

    // If end node wasn't visited, no path exists
    if (visited_nodes.find(end_hash) == visited_nodes.end()) {
        return {};
    }

    std::vector<std::string> path_nodes;
    size_t current_node = end_hash;

    while (current_node != start_hash) {
        path_nodes.push_back(hash_to_string_map.at(current_node));
        auto it = parent_nodes.find(current_node);
        if (it == parent_nodes.end())
            return {}; // No path found
        current_node = it->second;
    }

    path_nodes.push_back(hash_to_string_map.at(start_hash));
    std::reverse(path_nodes.begin(), path_nodes.end());
    return path_nodes;
}

std::unordered_set<std::string, string_hash>
get_unique_nodes(const node_map &connections) {
    std::unordered_set<std::string, string_hash> unique_nodes;
    for (const auto &[key, values] : connections) {
        unique_nodes.insert(key);
    }
    return unique_nodes;
}

std::string
get_random_node(const std::unordered_set<std::string, string_hash> &nodes) {
    if (nodes.empty()) {
        throw std::runtime_error("No nodes in the graph");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nodes.size() - 1);
    int random_index = dis(gen);

    auto it = nodes.begin();
    std::advance(it, random_index);
    return *it;
}

std::vector<std::vector<std::string>>
find_connected_components(const node_map &connections,
                          const hash_to_string &hash_to_string_map) {
    std::unordered_set<size_t> visited;
    std::vector<std::vector<std::string>> components;

    // Helper function for BFS to find a connected component
    auto bfs = [&](size_t start_hash) -> std::vector<std::string> {
        std::vector<std::string> component;
        std::queue<size_t> queue;
        queue.push(start_hash);
        visited.insert(start_hash);

        while (!queue.empty()) {
            size_t current = queue.front();
            queue.pop();

            component.push_back(hash_to_string_map.at(current));

            const std::string &current_str = hash_to_string_map.at(current);
            auto it = connections.find(current_str);
            if (it != connections.end()) {
                for (size_t neighbor : it->second) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        queue.push(neighbor);
                    }
                }
            }
        }

        return component;
    };

    // Iterate over all nodes and perform BFS for unvisited nodes
    for (const auto &[node, neighbors] : connections) {
        size_t node_hash = hash_and_store(
            node, const_cast<hash_to_string &>(hash_to_string_map));
        if (visited.find(node_hash) == visited.end()) {
            components.push_back(bfs(node_hash));
        }
    }

    return components;
}

std::vector<int> analyze_graph(const node_map &connections,
                   const hash_to_string &hash_to_string_map) {
    auto components =
        find_connected_components(connections, hash_to_string_map);

    std::cout << "Number of connected components: " << components.size()
              << std::endl;
    for (size_t i = 0; i < components.size(); ++i) {
        std::cout << "Component " << i + 1 << " has " << components[i].size()
                  << " nodes." << std::endl;
    }
    std::vector<int> component_sizes;
    for (const auto &component : components) {
        component_sizes.push_back(component.size());
    }
    return component_sizes;
}

std::vector<int> disconnect_and_analyze(node_map &connections,
                            const std::vector<std::pair<std::string, std::string>> &edges_to_disconnect,
                            const hash_to_string &hash_to_string_map) {
    // Disconnect the specified edges from the graph
    for (const auto &[node1, node2] : edges_to_disconnect) {
        size_t node1_hash = hash_and_store(node1, const_cast<hash_to_string &>(hash_to_string_map));
        size_t node2_hash = hash_and_store(node2, const_cast<hash_to_string &>(hash_to_string_map));

        // Remove node2 from node1's adjacency list
        auto it1 = connections.find(node1);
        if (it1 != connections.end()) {
            auto &neighbors = it1->second;
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), node2_hash), neighbors.end());
        }

        // Remove node1 from node2's adjacency list
        auto it2 = connections.find(node2);
        if (it2 != connections.end()) {
            auto &neighbors = it2->second;
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), node1_hash), neighbors.end());
        }
    }

    // Analyze the modified graph
    return analyze_graph(connections, hash_to_string_map);
}

// Type alias for clarity
using Pair = std::pair<std::string, std::string>;
using Pairing = std::vector<Pair>;

void generatePairings(const std::vector<std::string>& strings, Pairing& current, std::vector<Pairing>& result, std::vector<bool>& used) {
    // Check if we have used all strings
    if (current.size() == strings.size() / 2) {
        result.push_back(current);
        return;
    }

    // Find the first unused string
    int firstUnused = -1;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (!used[i]) {
            firstUnused = i;
            break;
        }
    }

    if (firstUnused == -1) return; // Should not happen

    // Mark the first unused string as used
    used[firstUnused] = true;

    // Pair it with every other unused string
    for (size_t j = firstUnused + 1; j < strings.size(); ++j) {
        if (!used[j]) {
            used[j] = true;
            current.emplace_back(strings[firstUnused], strings[j]);

            // Recurse to generate further pairings
            generatePairings(strings, current, result, used);

            // Backtrack
            current.pop_back();
            used[j] = false;
        }
    }

    // Backtrack
    used[firstUnused] = false;
}

std::vector<Pairing> generateAllPairings(const std::vector<std::string>& strings) {
    if (strings.size() % 2 != 0) {
        throw std::invalid_argument("Number of strings must be even.");
    }

    std::vector<Pairing> result;
    Pairing current;
    std::vector<bool> used(strings.size(), false);

    generatePairings(strings, current, result, used);
    return result;
}

int main() {
    hash_to_string hash_to_string_map;
    node_map connections = parse_input(hash_to_string_map);

    auto nodes = get_unique_nodes(connections);
    std::map<std::string, int> node_to_number_of_paths_passed;

    for (int i = 0; i < 2000; ++i) {
        std::string start_node = get_random_node(nodes);
        std::string end_node;

        do {
            end_node = get_random_node(nodes);
        } while (end_node == start_node);

        std::vector<std::string> shortest_path = find_shortest_path(
            connections, start_node, end_node, hash_to_string_map);

        // add +1 to all nodes in the path
        for (const std::string &node : shortest_path) {
            node_to_number_of_paths_passed[node]++;
        }
    }

    // sort the nodes by the number of paths passed
    std::vector<std::pair<std::string, int>> sorted_nodes(
        node_to_number_of_paths_passed.begin(),
        node_to_number_of_paths_passed.end());
    std::sort(sorted_nodes.begin(), sorted_nodes.end(),
              [](const auto &lhs, const auto &rhs) {
                  return lhs.second > rhs.second;
              });

    // get the first 6 nodes
    std::vector<std::string> nodes_to_disconnect;
    for (size_t i = 0; i < 6; ++i) {
        nodes_to_disconnect.push_back(sorted_nodes[i].first);
    }

    // generate all possible pairings of the nodes 
    auto pairings = generateAllPairings(nodes_to_disconnect);
   
    for (const auto& pairing : pairings) {
        std::cout << "Pairing: ";
        for (const auto& [node1, node2] : pairing) {
            std::cout << node1 << " " << node2 << ", ";
        }
        std::cout << std::endl;

        // disconnect the pairings and analyze the graph
        std::vector<int> components = disconnect_and_analyze(connections, pairing, hash_to_string_map);
        if (components.size() == 2) {
            int result = components[0] * components[1];
            std::cout << "Result: " << result << std::endl;
            break;
        }
    }
    return 0;
}

