#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Bid {
    std::string hand; // Original hand (e.g., "32T3K")
    int bidAmount;
    int rank; // Hand type rank (1-7)
};

// Card value mapping
int get_card_value(char c) {
    static const std::map<char, int> values = {
        {'2', 2},  {'3', 3},  {'4', 4}, {'5', 5},  {'6', 6},
        {'7', 7},  {'8', 8},  {'9', 9}, {'T', 10}, {'J', 11},
        {'Q', 12}, {'K', 13}, {'A', 14}};
    return values.at(c);
}

int rank_hand(const std::string &hand) {
    std::map<char, int> counts;
    for (char c : hand) {
        counts[c]++;
    }

    std::vector<int> frequencies;
    for (const auto &pair : counts) {
        frequencies.push_back(pair.second);
    }
    std::sort(frequencies.begin(), frequencies.end(), std::greater<int>());

    // Five of a kind
    if (frequencies[0] == 5)
        return 7;
    // Four of a kind
    if (frequencies[0] == 4)
        return 6;
    // Full house
    if (frequencies[0] == 3 && frequencies[1] == 2)
        return 5;
    // Three of a kind
    if (frequencies[0] == 3)
        return 4;
    // Two pair
    if (frequencies[0] == 2 && frequencies[1] == 2)
        return 3;
    // One pair
    if (frequencies[0] == 2)
        return 2;
    // High card
    return 1;
}

bool operator<(const Bid &lhs, const Bid &rhs) {
    if (lhs.rank != rhs.rank) {
        return lhs.rank < rhs.rank;
    }

    // Compare cards from left to right
    for (int i = 0; i < 5; i++) {
        int lval = get_card_value(lhs.hand[i]);
        int rval = get_card_value(rhs.hand[i]);
        if (lval != rval) {
            return lval < rval;
        }
    }
    return false;
}

std::vector<Bid> read_bids() {
    std::vector<Bid> bids;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty())
            break;

        std::istringstream stream(line);
        Bid bid;
        if (stream >> bid.hand >> bid.bidAmount) {
            bid.rank = rank_hand(bid.hand);
            bids.push_back(bid);
        } else {
            std::cerr << "Invalid input format: " << line << std::endl;
        }
    }
    return bids;
}

int main() {
    std::vector<Bid> bids = read_bids();
    std::sort(bids.begin(), bids.end());

    long long sum = 0;
    for (int i = 0; i < bids.size(); i++) {
        sum += static_cast<long long>(bids[i].bidAmount) * (i + 1);
    }

    std::cout << sum << std::endl;
    return 0;
}
