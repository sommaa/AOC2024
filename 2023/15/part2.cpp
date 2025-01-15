#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

std::map<int, std::vector<std::string>> box_to_lens;
std::unordered_map<std::string, int> lens_to_focal;

std::vector<std::string> split(const std::string &delim) {
    std::string line;
    std::getline(std::cin, line);
    std::vector<std::string> data;
    size_t pos = 0;
    while ((pos = line.find(delim)) != std::string::npos) {
        data.push_back(line.substr(0, pos));
        line.erase(0, pos + delim.length());
    }
    data.push_back(line);
    return data;
}

int hash(const std::string &s) {
    int h = 0;
    for (const auto &c : s) {
        h += int(c);
        h *= 17;
        h %= 256;
    }
    return h;
}


void fill_map(const std::vector<std::string> &data) {
    for (const auto &lens : data) {
        if (std::find(lens.begin(), lens.end(), '=') != lens.end()) {
            size_t pos = lens.find("=");
            std::string lens_name = lens.substr(0, pos);
            int focal_length = std::stoi(lens.substr(pos + 1));

            int box = hash(lens_name);

            // if the lens is already in the box substitute the focal length otherwise add it 
            if (box_to_lens.find(box) != box_to_lens.end()) {
                if (std::find(box_to_lens[box].begin(), box_to_lens[box].end(), lens_name) != box_to_lens[box].end()) {
                    lens_to_focal[lens_name] = focal_length;
                } else {
                    box_to_lens[box].push_back(lens_name);
                    lens_to_focal[lens_name] = focal_length;
                }
            } else {
                box_to_lens[box] = {lens_name};
                lens_to_focal[lens_name] = focal_length;
            }   
        }
        if (std::find(lens.begin(), lens.end(), '-') != lens.end()) {
            size_t pos = lens.find("-");
            std::string lens_name = lens.substr(0, pos);

            int box = hash(lens_name);

            // if the lens is already in the box remove it 
            if (box_to_lens.find(box) != box_to_lens.end()) {
                if (std::find(box_to_lens[box].begin(), box_to_lens[box].end(), lens_name) != box_to_lens[box].end()) {
                    box_to_lens[box].erase(std::remove(box_to_lens[box].begin(), box_to_lens[box].end(), lens_name), box_to_lens[box].end());
                    lens_to_focal.erase(lens_name);
                }
            }
        }
    }
}

void print_map() {
    for (const auto& [box, lenses] : box_to_lens) {
        std::cout << "Box " << box << ": ";
        for (const auto& lens : lenses) {
            std::cout << lens << "=" << lens_to_focal[lens] << " ";
        }
        std::cout << std::endl;
    }
}

int calc_focal_power() {
    int total = 0;

    //loop on boxes 
    for (const auto& [box, lenses] : box_to_lens) {
        //loop on lenses in the box
        int index = 0;
        for (const auto& lens : lenses) {
            index++;
            total += (box + 1) * index * lens_to_focal[lens];
        }
    }
    return total;
}

int main() {
    std::vector<std::string> data = split(",");
    fill_map(data);
    print_map();
    std::cout << calc_focal_power() << std::endl;
    return 0;
}
