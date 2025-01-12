#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

void import_data(long long &time, long long &distance) {
    std::string line;

    // Read time
    std::getline(std::cin, line);
    line = line.substr(line.find(":") + 1); // Remove "Time:" prefix
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    time = std::stoll(line);

    // Read distance
    std::getline(std::cin, line);
    line = line.substr(line.find(":") + 1); // Remove "Distance:" prefix
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    distance = std::stoll(line);
}

int main() {
    long long time;
    long long distance;
    import_data(time, distance);
    // time = time_button + time_walk
    // distance_walk = speed_walk * time_walk
    // speed_walk = time_button
    // -->
    // distance_walk = time_button * time_walk
    // -->
    // time_button = time - distance_walk/time_button
    long long margin = 0;
    for (long long time_button = 0; time_button <= time; time_button++) {
        long long time_walk = time - time_button;
        long long distance_walk = time_button * time_walk;
        if (distance_walk > distance) {
            margin++;
        }
    }
    std::cout << margin << std::endl;
    return 0;
}
