#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Set {
    int red = 0;
    int green = 0;
    int blue = 0;
};

struct Game {
    int id;
    std::vector<Set> sets;
};

// Function to trim whitespace from a string
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" ");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" ");
    return str.substr(first, last - first + 1);
}

// Function to parse a single set of cubes (e.g., "3 blue, 4 red")
Set parseSet(const std::string &setStr) {
    Set set;
    std::stringstream ss(setStr);
    std::string item;

    while (std::getline(ss, item, ',')) {
        item = trim(item);
        int count;
        std::string color;
        std::stringstream itemStream(item);
        itemStream >> count >> color;

        if (color == "red")
            set.red = count;
        else if (color == "green")
            set.green = count;
        else if (color == "blue")
            set.blue = count;
    }

    return set;
}

// Function to parse a complete game line
Game parseGame(const std::string &line) {
    Game game;

    // Extract game ID
    size_t colonPos = line.find(':');
    std::string gameIdStr = line.substr(5, colonPos - 5);
    game.id = std::stoi(gameIdStr);

    // Extract sets
    std::string setsStr = line.substr(colonPos + 1);
    std::stringstream ss(setsStr);
    std::string setStr;

    while (std::getline(ss, setStr, ';')) {
        game.sets.push_back(parseSet(trim(setStr)));
    }

    return game;
}

int main() {
    std::string line;
    std::vector<Game> games;

    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            games.push_back(parseGame(line));
        }
    }

    int sum = 0;
    for (const auto &game : games) {
        int num_red = 0;
        int num_green = 0;
        int num_blue = 0;

        for (size_t i = 0; i < game.sets.size(); ++i) {
            const auto &set = game.sets[i];
            num_red = std::max(num_red, set.red);
            num_green = std::max(num_green, set.green);
            num_blue = std::max(num_blue, set.blue);
        }
        sum += num_red * num_green * num_blue;
    }
    std::cout << sum << std::endl;
    return 0;
}
