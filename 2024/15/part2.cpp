#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct pair_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

class robot_puzzle {
  private:
    std::unordered_set<std::pair<int, int>, pair_hash> walls;
    std::vector<std::pair<int, int>> boxes;
    std::pair<int, int> robot_pos;
    std::string directions;
    int rows;
    int cols;

    bool would_hit_wall(int cr, int cc) const {
        return walls.count({cr, cc}) > 0;
    }

    void try_to_move(int dy, int dx) {
        int new_y = robot_pos.first + dy;
        int new_x = robot_pos.second + dx;

        if (would_hit_wall(new_y, new_x)) {
            return;
        }

        std::pair<int, int> *box_in_way = nullptr;
        for (auto &box : boxes) {
            if (box.first == new_y && box.second == new_x) {
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

            int next_y = box->first + dy;
            int next_x = box->second + dx;

            if (would_hit_wall(next_y, next_x)) {
                boxes_can_move = false;
                break;
            }

            boxes_to_move.push_back(box);

            for (auto &other_box : boxes) {
                if (&other_box == box) {
                    continue;
                }
                if (other_box.first == next_y && other_box.second == next_x) {
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
        if (!file) {
            std::cerr << "Error opening file: " << filename << '\n';
            return;
        }

        std::string line;
        bool found_blank_line = false;
        int r = 0;

        while (std::getline(file, line)) {
            if (line.empty()) {
                found_blank_line = true;
                rows = r;
                continue;
            }

            if (found_blank_line) {
                directions += line;
                continue;
            }

            if (r == 0) {
                cols = static_cast<int>(line.size()) * 2;
            }

            for (size_t c = 0; c < line.size(); ++c) {
                char ch = line[c];
                int pos_c = static_cast<int>(c) * 2;
                if (ch == '@') {
                    robot_pos = {r, pos_c};
                } else if (ch == '#') {
                    walls.emplace(r, pos_c);
                    walls.emplace(r, pos_c + 1);
                } else if (ch == 'O') {
                    boxes.emplace_back(r, pos_c);
                }
            }
            ++r;
        }
        rows = r;
    }

    void simulate_moves() {
        const std::unordered_map<char, std::pair<int, int>> direction_map = {
            {'^', {-1, 0}}, {'v', {1, 0}}, {'<', {0, -1}}, {'>', {0, 1}}};

        for (char direction : directions) {
            auto it = direction_map.find(direction);
            if (it != direction_map.end()) {
                const auto &[dy, dx] = it->second;
                try_to_move(dy, dx);
            }
        }
    }

    int calculate_total() const {
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
