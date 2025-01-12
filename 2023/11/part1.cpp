#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> import_data() {
    std::vector<std::string> data;
    std::string line;
    
    // Read initial data
    while (std::getline(std::cin, line)) {
        data.push_back(line);
        if (line.find("#") == std::string::npos) {
            data.push_back(line);
        }
    }

    // Check columns for # and duplicate if none found
    for (size_t i = 0; i < data[0].length(); i++) {
        bool has_hash = false;
        for (const auto& row : data) {
            if (row[i] == '#') {
                has_hash = true;
                break;
            }
        }
        
        if (!has_hash) {
            // Insert new column after current one
            for (auto& row : data) {
                row.insert(i + 1, 1, '.');
            }
            i++; // Skip the newly inserted column
        }
    }
    return data;
}

int manattan_distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

std::vector<std::pair<int, int>> get_stars_coordinates(const std::vector<std::string>& data) {
    std::vector<std::pair<int, int>> stars;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == '#') {
                stars.push_back({i, j});
            }
        }
    }
    return stars;
}

int main() {
    auto data = import_data();
    auto stars = get_stars_coordinates(data);
    print_data(data);
    int sum_distance = 0;
    for (int i = 0; i < stars.size(); i++) {
        for (int j = i + 1; j < stars.size(); j++) {
            sum_distance += manattan_distance(stars[i], stars[j]);
        }
    }
    std::cout << "Sum of manhattan distances: " << sum_distance << std::endl;
    return 0;
}


