#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

std::vector<std::string> import_data() {
    std::vector<std::string> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        data.push_back(line);
    }
    return data;
}

long long manattan_distance(std::pair<long long, long long> a, std::pair<long long, long long> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

std::unordered_set<long long> get_empty_columns(const std::vector<std::string>& data) {
    std::unordered_set<long long> empty_columns;
    for (size_t i = 0; i < data[0].length(); i++) {
        bool has_hash = false;
        for (const auto& row : data) {
            if (row[i] == '#') {
                has_hash = true;
                break;
            }
        }
        
        if (!has_hash) {
            empty_columns.insert(i);
        }
    }
    return empty_columns;
}

std::unordered_set<long long> get_empty_rows(const std::vector<std::string>& data) {
    std::unordered_set<long long> empty_rows;
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i].find("#") == std::string::npos) {
            empty_rows.insert(i);
        }
    }
    return empty_rows;
}

std::vector<std::pair<long long, long long>> get_stars_coordinates(const std::vector<std::string>& data) {
    std::vector<std::pair<long long, long long>> stars;
    for (long long i = 0; i < data.size(); i++) {
        for (long long j = 0; j < data[i].size(); j++) {
            if (data[i][j] == '#') {
                stars.push_back({i, j});
            }
        }
    }
    return stars;
}

void manipulate_star_coordinates(std::vector<std::pair<long long, long long>>& stars, const std::unordered_set<long long>& empty_rows, const std::unordered_set<long long>& empty_columns, long long& empty_value) {
    empty_value--;
    for (auto& star : stars) {
        // update row 
        long long star_row_tmp = star.first;
        for (auto row_pos : empty_rows) {
            if (row_pos < star.first) {
                star_row_tmp += empty_value;
            }
        }
        star.first = star_row_tmp;

        // update column 
        long long star_column_tmp = star.second;
        for (auto col_pos : empty_columns) {
            if (col_pos < star.second) {
                star_column_tmp += empty_value;
            }
        }
        star.second = star_column_tmp;
    }
}

int main() {
    long long empty_value = 1000000;
    
    auto data = import_data();
    auto stars = get_stars_coordinates(data);
    auto empty_rows = get_empty_rows(data);
    auto empty_columns = get_empty_columns(data);
    manipulate_star_coordinates(stars, empty_rows, empty_columns, empty_value);
    long long sum_distance = 0;
    for (long long i = 0; i < stars.size(); i++) {
        for (long long j = i + 1; j < stars.size(); j++) {
            sum_distance += (manattan_distance(stars[i], stars[j]));
        }
    }
    std::cout << "Sum of manhattan distances: " << sum_distance << std::endl;
    return 0;
}


