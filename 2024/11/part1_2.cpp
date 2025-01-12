#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

// Function to print __int128 (since std::cout doesn't support it directly)
void print_int128(__int128 num);

// Function to import numbers from a single line in a file
std::vector<__int128> importNumbersFromLine(const std::string& filePath) {
    std::vector<__int128> numbers;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return numbers; // Return an empty vector on error
    }
    std::string line;
    if (std::getline(file, line)) { // Read the first line from the file
        std::istringstream iss(line);
        long long num; // Temporarily store numbers as long long
        while (iss >> num) { // Extract integers from the line
            numbers.push_back(static_cast<__int128>(num));
        }
    } else {
        std::cerr << "Error: Could not read a line from the file." << std::endl;
    }
    file.close();
    return numbers;
}

// Cache for dynamic programming, unordered map for O(1) access time
std::unordered_map<size_t, __int128> cache;

// Define a very secure hash function for combining number and n
size_t hash_function(__int128 number, __int128 n) {
    // Convert __int128 to two 64-bit parts for hashing
    uint64_t low_number = static_cast<uint64_t>(number & 0xFFFFFFFFFFFFFFFF);
    uint64_t high_number = static_cast<uint64_t>((number >> 64) & 0xFFFFFFFFFFFFFFFF);
    uint64_t low_n = static_cast<uint64_t>(n & 0xFFFFFFFFFFFFFFFF);
    uint64_t high_n = static_cast<uint64_t>((n >> 64) & 0xFFFFFFFFFFFFFFFF);

    // Prime numbers for scrambling
    const uint64_t prime1 = 0x9E3779B185EBCA87; // Large prime
    const uint64_t prime2 = 0xC6A4A7935BD1E995; // Another large prime
    const uint64_t prime3 = 0x165667B19E3779F9; // Third large prime

    // Mix all parts together using bitwise operations
    uint64_t hash = prime1;
    hash ^= (low_number + prime2) * prime3;
    hash = (hash << 31) | (hash >> 33); // Rotate left
    hash ^= (high_number + prime1) * prime2;
    hash = (hash << 27) | (hash >> 37); // Rotate left
    hash ^= (low_n + prime3) * prime1;
    hash = (hash << 29) | (hash >> 35); // Rotate left
    hash ^= (high_n + prime2) * prime3;

    // Final mixing
    hash ^= hash >> 33;
    hash *= prime1;
    hash ^= hash >> 29;
    hash *= prime2;
    hash ^= hash >> 32;

    return static_cast<size_t>(hash); // Truncate to size_t
}

// Define dynamic programming function
__int128 count_stones(__int128 number, __int128 n) {
    std::cout << "Calling count_stones with number: ";
    print_int128(number);
    std::cout << " and n: ";
    print_int128(n);
    std::cout << std::endl;

    // Base case
    if (n == 0) {
        std::cout << "Base case reached: returning 1\n";
        return 1;
    }

    // Compute hash for current state
    size_t hash = hash_function(number, n);
    std::cout << "Hash for state (number: ";
    print_int128(number);
    std::cout << ", n: ";
    print_int128(n);
    std::cout << "): " << hash << std::endl;

    // Check if the result is already in the cache
    if (cache.find(hash) != cache.end()) {
        std::cout << "Cache hit for hash: " << hash << " returning cached result\n";
        return cache[hash];
    }

    // Recursive computation
    __int128 result = 0;

    if (number == 0) {
        std::cout << "Number is 0, calling count_stones(1, n - 1)\n";
        result = count_stones(1, n - 1);
    }
    // If the length of the number is divisible by 2, split the number into two parts
    else if (std::to_string(static_cast<long long>(number)).length() % 2 == 0) {
        std::cout << "Number length is even, splitting number: ";
        print_int128(number);
        std::cout << std::endl;
        std::string num = std::to_string(static_cast<long long>(number));
        int half = num.length() / 2;
        std::string first = num.substr(0, half);
        std::string second = num.substr(half);

        __int128 firstNum = static_cast<__int128>(std::stoll(first));
        __int128 secondNum = static_cast<__int128>(std::stoll(second));

        std::cout << "First part: ";
        print_int128(firstNum);
        std::cout << ", Second part: ";
        print_int128(secondNum);
        std::cout << std::endl;

        result += count_stones(firstNum, n - 1);
        result += count_stones(secondNum, n - 1);
    } else {
        std::cout << "Number length is odd, multiplying number: ";
        print_int128(number);
        std::cout << " by 2024\n";
        __int128 transformed = number * 2024;
        result += count_stones(transformed, n - 1);
    }

    // Cache the result
    std::cout << "Caching result for hash: " << hash << " with value: ";
    print_int128(result);
    std::cout << std::endl;
    cache[hash] = result;
    return result;
}

// Function to print __int128 (since std::cout doesn't support it directly)
void print_int128(__int128 num) {
    if (num < 0) {
        std::cout << "-";
        num = -num;
    }

    if (num >= 10) {
        print_int128(num / 10);  // Recursively call for the next digit
    }

    std::cout << static_cast<char>('0' + (num % 10));  // Print the current digit
}

// Example usage
int main() {
    std::string filePath = "in.txt"; // Replace with your file path
    std::vector<__int128> numbers = importNumbersFromLine(filePath);

    if (numbers.empty()) {
        std::cerr << "Error: No numbers found in the input file." << std::endl;
        return 1;
    }

    // Number of operations
    __int128 n = 25;
    /*__int128 n = 75;*/
    __int128 stone_counter = 0;

    for (__int128 number : numbers) {
        std::cout << "Processing number: ";
        print_int128(number);
        std::cout << std::endl;
        __int128 res = count_stones(number, n);
        std::cout << "Number of stones for initial stone ";
        print_int128(number);
        std::cout << ": ";
        print_int128(res);
        std::cout << std::endl;
        stone_counter += res;
    }

    std::cout << "\nTotal number of stones after ";
    print_int128(n);
    std::cout << " transformations: ";
    print_int128(stone_counter);
    std::cout << std::endl;

    return 0;
}

