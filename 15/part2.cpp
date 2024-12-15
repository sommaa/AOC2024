#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class robot_puzzle {
  private:
    std::vector<std::pair<int, int>> walls;
    std::vector<std::pair<int, int>> boxes;
    std::pair<int, int> robot_pos;
    std::string directions;
    int rows;
    int cols;

    bool would_hit_wall(int cr, int cc) {
        for (const auto &wall : walls) {
            if (cr == wall.first && cc == wall.second) {
                return true;
            }
        }
        return false;
    }

    void try_to_move(int dy, int dx) {
        int new_y = robot_pos.first + dy;
        int new_x = robot_pos.second + dx;

        if (would_hit_wall(new_y, new_x)) {
            return;
        }

        std::pair<int, int> *box_in_way = nullptr;
        for (auto &box : boxes) {
            if (new_y == box.first && new_x - box.second >= 0 &&
                new_x - box.second <= 1) {
                box_in_way = &box;
                break;
            }
        }

        if (!box_in_way) {
            robot_pos = {new_y, new_x};
            return;
        }

        bool boxes_can_move = true;
        std::vector<std::pair<int, int> *> boxes_to_examine = {box_in_way};
        std::vector<std::pair<int, int> *> boxes_to_move;

        while (!boxes_to_examine.empty()) {
            auto *box = boxes_to_examine.back();
            boxes_to_examine.pop_back();

            if (std::find(boxes_to_move.begin(), boxes_to_move.end(), box) !=
                boxes_to_move.end()) {
                continue;
            }

            if (would_hit_wall(box->first + dy, box->second + dx) ||
                would_hit_wall(box->first + dy, box->second + dx + 1)) {
                boxes_can_move = false;
                break;
            }

            boxes_to_move.push_back(box);

            for (auto &other_box : boxes) {
                if (std::find(boxes_to_move.begin(), boxes_to_move.end(),
                              &other_box) != boxes_to_move.end()) {
                    continue;
                }
                if (box->first + dy == other_box.first &&
                    other_box.second - (box->second + dx) >= -1 &&
                    other_box.second - (box->second + dx) <= 1) {
                    boxes_to_examine.push_back(&other_box);
                }
            }
        }

        if (boxes_can_move) {
            robot_pos = {new_y, new_x};
            for (auto *box : boxes_to_move) {
                box->first += dy;
                box->second += dx;
            }
        }
    }

  public:
    void readFile(const std::string &filename) {
        std::ifstream file(filename);
        std::string line;
        bool found_blank_line = false;
        int r = 0;

        while (std::getline(file, line)) {
            if (r == 0) {
                cols = line.size();
            }
            if (line.empty()) {
                found_blank_line = true;
                rows = r;
                continue;
            }
            if (found_blank_line) {
                directions += line;
                continue;
            }
            for (int c = 0; c < cols; ++c) {
                if (line[c] == '@') {
                    robot_pos = {r, c * 2};
                } else if (line[c] == '#') {
                    walls.emplace_back(r, c * 2);
                    walls.emplace_back(r, c * 2 + 1);
                } else if (line[c] == 'O') {
                    boxes.emplace_back(r, c * 2);
                }
            }
            r += 1;
        }
    }

    void simulate_moves() {
        std::unordered_map<char, std::pair<int, int>> direction_map = {
            {'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};

        for (char direction : directions) {
            int dy = 0, dx = 0;
            if (direction_map.count(direction)) {
                dy = direction_map[direction].first;
                dx = direction_map[direction].second;
                try_to_move(dy, dx);
            }
        }
    }

    int calculate_total() {
        int total = 0;
        for (const auto &box : boxes) {
            total += 100 * box.first + box.second;
        }
        return total;
    }
};

int main() {
    robot_puzzle puzzle;
    puzzle.readFile("in.txt");
    puzzle.simulate_moves();
    std::cout << puzzle.calculate_total() << std::endl;
    return 0;
}
