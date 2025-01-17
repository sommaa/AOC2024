#include <bits/stdc++.h>

std::vector<std::vector<int>> import_data() {
    std::vector<std::vector<int>> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        data.push_back(row);
    }
    return data;
}

int shortest_path(const std::vector<std::vector<int>> &data) {
    // compare: compare heat loss minimum
    auto compare = [](const std::vector<int> &a, const std::vector<int> &b) {
        return a[0] > b[0]; // Min-heap based on heat loss
    };
    // hpq: heat loss, r, c, dr, dc, n - priority based on min heat loss
    std::priority_queue<std::vector<int>, std::vector<std::vector<int>>,
                        decltype(compare)>
        hpq(compare);
    // visited: r, c, dr, dc, n
    std::set<std::tuple<int, int, int, int, int>> visited;

    // directions: up, down, left, right
    const std::vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // start with initial heat loss of 0 (don't count starting position)
    hpq.push({0, 0, 0, 0, 0, 0});

    while (!hpq.empty()) {
        auto top = hpq.top();
        int heat_loss = top[0];
        int r = top[1];
        int c = top[2];
        int dr = top[3];
        int dc = top[4];
        int n = top[5];
        hpq.pop();

        // reached destination
        if (r == data.size() - 1 and c == data[0].size() - 1 and n >= 4) {
               return heat_loss;
        }

        // seen before
        if (visited.find(std::make_tuple(r, c, dr, dc, n)) != visited.end()) {
            continue;
        }

        visited.insert(std::make_tuple(r, c, dr, dc, n));

        if (n < 10 and (dr != 0 or dc != 0)) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 and nr < data.size() and nc >= 0 and
                nc < data[0].size()) {
                hpq.push({heat_loss + data[nr][nc], nr, nc, dr, dc, n + 1});
            }
        }

        if (n >= 4 or (dr == 0 and dc == 0)) {
            for (const auto &[ndr, ndc] : directions) {
                if (dr != ndr or dc != ndc) {
                    int nr = r + ndr;
                    int nc = c + ndc;
                    if (nr >= 0 and nr < data.size() and nc >= 0 and
                        nc < data[0].size()) {
                        hpq.push(
                            {heat_loss + data[nr][nc], nr, nc, ndr, ndc, 1});
                    }
                }
            }
        }
    }

    return -1;
}

int main() {
    std::vector<std::vector<int>> data = import_data();
    std::cout << shortest_path(data) << std::endl;
    return 0;
}
