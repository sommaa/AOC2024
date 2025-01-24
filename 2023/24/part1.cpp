#include <bits/stdc++.h>

struct Vec3D {
    long double x, y, z; // Changed to long double for signed calculations
};

struct HailStone {
    Vec3D vel;
    Vec3D pos;

    long double a, b, c;

    void compute_params() {
        a = vel.y;
        b = -vel.x;
        c = pos.x * vel.y - pos.y * vel.x;
    }
};

std::vector<HailStone> read_input() {
    std::vector<HailStone> stones;
    std::string line;
    while (std::getline(std::cin, line)) {
        HailStone stone;
        sscanf(line.c_str(), "%Lf, %Lf, %Lf @ %Lf, %Lf, %Lf",
               &stone.pos.x, &stone.pos.y, &stone.pos.z, &stone.vel.x,
               &stone.vel.y, &stone.vel.z);
        stone.compute_params();
        stones.push_back(stone);
    }
    return stones;
}

int main() {
    std::vector<HailStone> stones = read_input();
    std::pair<long double, long double> collision_range = 
        std::make_pair(200000000000000, 400000000000000);

    size_t count = 0;

    for (size_t i = 0; i < stones.size(); i++) {
        for (size_t j = 0; j < i; j++) { // Match Python's iteration logic
            HailStone &hs1 = stones[i];
            HailStone &hs2 = stones[j];

            long double denominator = hs1.a * hs2.b - hs2.a * hs1.b;
            if (denominator == 0) {
                continue; // Parallel lines, no collision
            }

            // Use long double for precision
            long double x = (hs1.c * hs2.b - hs2.c * hs1.b) / denominator;
            long double y = (hs2.c * hs1.a - hs1.c * hs2.a) / denominator;

            std::cout << "DEBUG: Pair (" << i << ", " << j << "): "
                      << "x=" << x << ", y=" << y 
                      << ", denom=" << denominator 
                      << ", hs1=(" << hs1.a << ", " << hs1.b << ", " << hs1.c << ") "
                      << ", hs2=(" << hs2.a << ", " << hs2.b << ", " << hs2.c << ")" 
                      << std::endl;

            if (x >= collision_range.first && x <= collision_range.second &&
                y >= collision_range.first && y <= collision_range.second) {
                // Forward in future
                bool valid = true;
                for (HailStone *hs : {&hs1, &hs2}) {
                    if ((x - hs->pos.x) * hs->vel.x < 0 || 
                        (y - hs->pos.y) * hs->vel.y < 0) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    std::cout << std::fixed << std::setprecision(0)
                              << "collision at " << x << ", " << y << std::endl;
                    count++;
                }
            }
        }
    }
    std::cout << "Total collisions: " << count << std::endl;
    return 0;
}

