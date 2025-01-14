#include <bits/stdc++.h>

// import block
std::vector<std::string> import_data() {
    std::vector<std::string> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        data.push_back(line);
    }
    return data;
}

void print_data(std::vector<std::string> &data) {
    for (auto line : data) {
        std::cout << line << std::endl;
    }
}

std::vector<std::pair<int, int>> get_mov_rocks(std::vector<std::string> &data) {
    std::vector<std::pair<int, int>> rocks;
    for (int i = 1; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 'O') {
                if (data[i - 1][j] == '.') {
                    rocks.push_back({i, j});
                }
            }
        }
    }
    return rocks;
}

void move_rocks(std::vector<std::string> &data) {
    std::vector<std::pair<int, int>> mov_rocks = get_mov_rocks(data);
    while (mov_rocks.size() > 0) {
        for (auto rock : mov_rocks) {
            data[rock.first][rock.second] = '.';
            data[rock.first - 1][rock.second] = 'O';
        }
        mov_rocks = get_mov_rocks(data);
    }
}

int calc_weight(std::vector<std::string> &data) {
    int weight = 0;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 'O') {
                weight += data.size() - i;
            }
        }
    }
    return weight;
}

int main() {
    std::vector<std::string> data = import_data();
    move_rocks(data);
    print_data(data);
    std::cout << calc_weight(data) << std::endl;
    return 0;
}
