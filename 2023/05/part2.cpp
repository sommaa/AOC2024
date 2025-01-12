#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Struct to store a mapping
struct RangeMap {
    long long destination_start;
    long long source_start;
    long long range_size;
};

// Function to parse a single map line
RangeMap parse_map_line(const std::string &line) {
    std::istringstream iss(line);
    RangeMap map;
    iss >> map.destination_start >> map.source_start >> map.range_size;
    return map;
}

// Function to parse input
void parse_input(std::vector<std::pair<long long, long long>> &seeds,
                 std::vector<std::vector<RangeMap>> &maps) {
    std::string line;

    // Parse seeds
    std::getline(std::cin, line);
    std::string seed_str = line.substr(7);
    std::istringstream seed_stream(seed_str);
    long long seed1, seed2;
    while (seed_stream >> seed1 >> seed2) {
        seeds.emplace_back(seed1, seed2 + seed1);
    }

    // Parse maps
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        int map_index = maps.size();
        maps.emplace_back();

        while (std::getline(std::cin, line) && !line.empty()) {
            maps[map_index].push_back(parse_map_line(line));
        }
    }
}

int main() {
    std::vector<std::pair<long long, long long>> seeds;
    std::vector<std::vector<RangeMap>> maps;

    // Parse input
    parse_input(seeds, maps);

    // Transform seeds through all maps
    std::vector<std::pair<long long, long long>> final_locations = seeds;
    std::vector<std::pair<long long, long long>> new_locations;

    for (const auto& map : maps) {
        while (!final_locations.empty()) {
            auto [seed_start, seed_end] = final_locations.back();
            final_locations.pop_back();
            bool mapped = false;
            for (const auto& range_map : map) {
                long long destination_start = range_map.destination_start;
                long long source_start = range_map.source_start;
                long long range_size = range_map.range_size;
                long long overlap_start = std::max(seed_start, source_start);
                long long overlap_end = std::min(seed_end, source_start + range_size);
                if (overlap_start < overlap_end) {
                    new_locations.emplace_back(overlap_start - source_start + destination_start,
                                               overlap_end - source_start + destination_start);
                    if (overlap_start > seed_start) {
                        final_locations.emplace_back(seed_start, overlap_start);
                    }
                    if (seed_end > overlap_end) {
                        final_locations.emplace_back(overlap_end, seed_end);
                    }
                    mapped = true;
                    break;
                }
            }
            if (!mapped) {
                new_locations.emplace_back(seed_start, seed_end);
            }
        }
        final_locations = new_locations;
        new_locations.clear();
    }

    // sort final locations by the start value
    std::sort(final_locations.begin(), final_locations.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    std::cout << final_locations[0].first << std::endl;
    return 0;
}
