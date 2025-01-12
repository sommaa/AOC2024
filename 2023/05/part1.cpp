#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

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
void parse_input(std::vector<long long> &seeds,
                 std::vector<std::vector<RangeMap>> &maps) {
    std::string line;

    // Parse seeds
    std::getline(std::cin, line);
    std::string seed_str = line.substr(7);
    std::istringstream seed_stream(seed_str);
    long long seed;
    while (seed_stream >> seed) {
        seeds.push_back(seed);
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
    std::vector<long long> seeds;
    std::vector<std::vector<RangeMap>> maps;

    // Parse input
    parse_input(seeds, maps);

    // Transform seeds through all maps
    std::vector<long long> final_locations = seeds;

    for (const auto& map : maps) {
        for (size_t i = 0; i < final_locations.size(); ++i) {
            bool mapped = false;

            for (const auto& range : map) {
                if (final_locations[i] >= range.source_start &&
                    final_locations[i] < range.source_start + range.range_size) {
                    // Transform the value
                    final_locations[i] = range.destination_start +
                                         (final_locations[i] - range.source_start);
                    mapped = true;
                    break;
                }
            }

            // If not mapped, the value remains the same (identity mapping)
            if (!mapped) {
                final_locations[i] = final_locations[i];
            }
        }
    }

    // Find the minimum positive value in final_locations
    long long min_location = std::numeric_limits<long long>::max();
    for (long long location : final_locations) {
        if (location >= 0 && location < min_location) {
            min_location = location;
        }
    }

    // Output the result
    std::cout << "The lowest location number is: " << min_location << std::endl;
    return 0;
}

