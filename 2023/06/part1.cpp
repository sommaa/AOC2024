#include <iostream>
#include <string>
#include <vector>
#include <sstream>

void import_data(std::vector<int>& times, std::vector<int>& distances) {
    std::string line;
    
    // Read times
    std::getline(std::cin, line);
    line = line.substr(line.find(":") + 1);  // Remove "Time:" prefix
    std::istringstream time_stream(line);
    int value;
    while (time_stream >> value) {
        times.push_back(value);
    }
    
    // Read distances
    std::getline(std::cin, line);
    line = line.substr(line.find(":") + 1);  // Remove "Distance:" prefix
    std::istringstream dist_stream(line);
    while (dist_stream >> value) {
        distances.push_back(value);
    }
}

int main() {
    std::vector<int> times;
    std::vector<int> distances;
    import_data(times, distances);

    // time = time_button + time_walk
    // distance_walk = speed_walk * time_walk
    // speed_walk = time_button
    // -->
    // distance_walk = time_button * time_walk
    // -->
    // time_button = time - distance_walk/time_button
    int margin = 1;
    for (int i = 0; i < times.size(); i++) {
        int local_margin = 0;
        int time = times[i];
        int distance = distances[i];
        for (int time_button = 0; time_button <= time; time_button++) {
            int time_walk = time - time_button;
            int distance_walk = time_button * time_walk;
            if (distance_walk > distance) {
                local_margin++;
            }
        }
        if (local_margin > 0) {
            std::cout << local_margin << std::endl;
            margin *= local_margin;
        }

    }
    std::cout << margin << std::endl;
    return 0;
}
