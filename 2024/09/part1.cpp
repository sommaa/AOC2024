#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>

// Function to create the filesystem (array of blocks)
// diskmap: A string of digits representing the filesystem layout.
// The function returns a vector of integers where each element is either a file id or -1 (empty space).
std::vector<int> make_filesystem(const std::string &diskmap) {
    std::vector<int> blocks;  // This will store the blocks of the filesystem
    bool is_file = true;      // Flag to indicate if we're currently processing a file or empty space
    int id = 0;               // File ID (incremented with each new file)

    // Loop through each character in the diskmap string
    for (char c : diskmap) {
        int x = c - '0';  // Convert the character to an integer (0-9)

        if (is_file) {
            // Insert 'x' instances of the current file ID into the blocks vector
            blocks.insert(blocks.end(), x, id);
            ++id;  // Increment the file ID for the next file
            is_file = false;  // After processing a file, the next part will be empty space
        } else {
            // Insert 'x' instances of -1 (empty space) into the blocks vector
            blocks.insert(blocks.end(), x, -1);
            is_file = true;  // After processing empty space, the next part will be a file
        }
    }

    return blocks;  // Return the constructed filesystem blocks
}

// Function to move the files within the filesystem
// arr: A vector representing the filesystem blocks
// The function will compact the filesystem by moving the files towards the front, leaving empty spaces at the end.
std::vector<int> move(std::vector<int> &arr) {
    size_t first_free = 0;  // Pointer to the first free space in the array (initially set to 0)

    // Find the first free block (i.e., -1)
    while (first_free < arr.size() && arr[first_free] != -1) {
        ++first_free;  // Move forward until a free space (-1) is found
    }

    int i = arr.size() - 1;  // Pointer to the last block in the array

    // Find the last file (non -1) block from the end
    while (i >= 0 && arr[i] == -1) {
        --i;  // Move backward until a file is found (non -1)
    }

    // Move files towards the front, swapping with the first free space
    while (i > static_cast<int>(first_free)) {
        arr[first_free] = arr[i];  // Move the file to the first free position
        arr[i] = -1;  // Mark the previous position as free space

        // Move the pointer i to the next non-free space (if any)
        do {
            --i;
        } while (i >= 0 && arr[i] == -1);  // Skip over any empty spaces

        // Move the pointer first_free to the next free space (if any)
        do {
            ++first_free;
        } while (first_free < arr.size() && arr[first_free] != -1);  // Skip over non-empty blocks
    }

    return arr;  // Return the modified filesystem with files moved
}

// Function to calculate the checksum of the filesystem
// arr: A vector representing the filesystem blocks
// The checksum is the sum of each block's index multiplied by its corresponding file ID (ignoring -1 spaces).
long long checksum(const std::vector<int> &arr) {
    long long ans = 0;  // Variable to store the checksum result

    // Iterate through the array and calculate the checksum
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] != -1) {  // Skip empty spaces (-1)
            ans += static_cast<long long>(i) * arr[i];  // Add the index multiplied by the file ID
        }
    }
    return ans;  // Return the calculated checksum
}

int main() {
    // Open the input file in read mode
    std::ifstream fin("in.txt");

    // Check if the file was successfully opened
    if (!fin) {
        std::cerr << "Error: Unable to open input file.\n";  // Error message if the file cannot be opened
        return 1;  // Exit with an error code
    }

    std::string line;  // String to hold the contents of the file

    // Read the entire line from the input file (assumes it's a single line)
    std::getline(fin, line);
    fin.close();  // Close the file after reading

    // Check if the input file is empty
    if (line.empty()) {
        std::cerr << "Error: Input file is empty.\n";  // Error message for empty file
        return 1;  // Exit with an error code
    }

    // Create the filesystem from the input string
    std::vector<int> filesystem = make_filesystem(line);

    // Move the files within the filesystem
    move(filesystem);

    // Calculate the checksum of the moved filesystem
    long long ans = checksum(filesystem);

    // Output the checksum result
    std::cout << ans << std::endl;

    return 0;  // Return success
}

