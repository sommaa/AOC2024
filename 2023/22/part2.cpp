#include <bits/stdc++.h>

struct Point3D {
    int x, y, z;
};

struct Brick {
    Point3D start;
    Point3D end;
};

std::vector<Brick> readBricks() {
    std::vector<Brick> bricks;
    std::string line;

    while (std::getline(std::cin, line) && !line.empty()) {
        Brick brick;

        // Find the tilde that separates start and end coordinates
        size_t tilde_pos = line.find('~');

        // Parse start coordinates
        size_t comma1 = line.find(',');
        size_t comma2 = line.find(',', comma1 + 1);

        brick.start.x = std::stoi(line.substr(0, comma1));
        brick.start.y = std::stoi(line.substr(comma1 + 1, comma2 - comma1 - 1));
        brick.start.z =
            std::stoi(line.substr(comma2 + 1, tilde_pos - comma2 - 1));

        // Parse end coordinates
        size_t comma3 = line.find(',', tilde_pos + 1);
        size_t comma4 = line.find(',', comma3 + 1);

        brick.end.x =
            std::stoi(line.substr(tilde_pos + 1, comma3 - tilde_pos - 1));
        brick.end.y = std::stoi(line.substr(comma3 + 1, comma4 - comma3 - 1));
        brick.end.z = std::stoi(line.substr(comma4 + 1));

        bricks.push_back(brick);
    }

    return bricks;
}

void sort_bricks_alongz(std::vector<Brick> &bricks) {
    std::sort(bricks.begin(), bricks.end(), [](const Brick &a, const Brick &b) {
        return a.start.z < b.start.z;
    });
}

bool overlapping(const Brick &a, const Brick &b) {
    return std::max(a.start.x, b.start.x) <= std::min(a.end.x, b.end.x) &&
           std::max(a.start.y, b.start.y) <= std::min(a.end.y, b.end.y);
}

void stack_bricks(std::vector<Brick> &bricks) {
    for (int i = 0; i < bricks.size(); i++) {
        Brick &brick = bricks[i];
        int max_z = 1;
        for (int j = 0; j < i; j++) {
            Brick &other = bricks[j];
            if (overlapping(brick, other)) {
                max_z = std::max(max_z, other.end.z + 1);
            }
        }
        brick.end.z -= brick.start.z - max_z;
        brick.start.z = max_z;
    }
}

using supp_map = std::unordered_map<int, std::unordered_set<int>>;

std::pair<supp_map, supp_map>
get_supported_bricks(const std::vector<Brick> &bricks) {
    supp_map a_supports;
    supp_map a_is_supported_by;

    for (int i = 0; i < bricks.size(); i++) {
        a_supports[i] = std::unordered_set<int>();
        a_is_supported_by[i] = std::unordered_set<int>();
    }
    for (int i = 0; i < bricks.size(); i++) {
        for (int j = 0; j < bricks.size(); j++) {
            if (overlapping(bricks[i], bricks[j]) &&
                bricks[j].start.z == bricks[i].end.z + 1) {
                a_supports[i].insert(j);
                a_is_supported_by[j].insert(i);
            }
        }
    }
    return {a_supports, a_is_supported_by};
}


int count_falling(const supp_map &a_supports, const supp_map &a_is_supported_by,
                  const std::vector<Brick> &bricks) {
    int count = 0;
    std::deque<int> q;
    for (int i = 0; i < bricks.size(); i++) {
        q.clear();

        // Accessing a_supports with const correctness
        const auto &supporters = a_supports.at(i);
        for (const auto &supporter : supporters) {
            const auto &supporter_dependencies = a_is_supported_by.at(supporter);
            if (supporter_dependencies.size() == 1) {
                q.push_back(supporter);
            }
        }

        std::unordered_set<int> falling;
        for (int i : q) {
            falling.insert(i);
        }
        falling.insert(i);

        while (!q.empty()) {
            int j = q.front();
            q.pop_front();

            for (auto k : a_supports.at(j)) {
                if (falling.find(k) == falling.end()) {
                    if (std::all_of(a_is_supported_by.at(k).begin(), a_is_supported_by.at(k).end(),
                                    [&falling](int x) { return falling.find(x) != falling.end(); })) {
                        q.push_back(k);
                        falling.insert(k);
                    }
                }
            }
        }
        count += falling.size() - 1;
    }
    return count;
}

int main() {
    std::vector<Brick> bricks = readBricks();
    sort_bricks_alongz(bricks);
    stack_bricks(bricks);
    sort_bricks_alongz(bricks);
    std::pair<supp_map, supp_map> supported_bricks = get_supported_bricks(bricks);
    std::cout << count_falling(supported_bricks.first, supported_bricks.second, bricks) << std::endl;
    return 0;
}
