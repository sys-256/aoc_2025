#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Day4 {
    private:
    std::vector<std::vector<bool>> grid;

    bool has_roll(size_t i, size_t j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size()) {
            return false;
        } else {
            return grid[i][j];
        }
    }

    bool can_access(size_t i, size_t j) {
        long adjecent_rolls = 0;
        adjecent_rolls += has_roll(i-1, j-1);
        adjecent_rolls += has_roll(i-1, j);
        adjecent_rolls += has_roll(i-1, j+1);
        adjecent_rolls += has_roll(i, j-1);
        adjecent_rolls += has_roll(i, j+1);
        adjecent_rolls += has_roll(i+1, j-1);
        adjecent_rolls += has_roll(i+1, j);
        adjecent_rolls += has_roll(i+1, j+1);
        return has_roll(i, j) && adjecent_rolls < 4;
    }

    long remove_accessable() {
        // Returns how many it removed
        long removed = 0;
        for (long i = 0; i < grid.size(); i++) {
            for (long j = 0; j < grid[0].size(); j++) {
                if (can_access(i, j)) {
                    removed += 1;
                    grid[i][j] = false;
                }
            }
        }
        return removed;
    }

    public:
    void add_row(std::string row_str) {
        std::vector<bool> row;
        for (char c : row_str) {
            row.push_back(c == '@');
        }
        grid.push_back(row);
    }

    long get_rolls_removable() {
        long total_removed = 0;
        long removed = remove_accessable();
        while (removed > 0) {
            total_removed += removed;
            removed = remove_accessable();
        }
        return total_removed;
    }

    long get_rolls_accessible() {
        long n = 0;
        for (long i = 0; i < grid.size(); i++) {
            for (long j = 0; j < grid[0].size(); j++) {
                n += can_access(i, j);
            }
        }
        return n;
    }
};

int main() {
    Day4 day4;
    std::ifstream file("day4_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        day4.add_row(line);
    }

    std::cout << day4.get_rolls_removable() << "\n";

    file.close();
    return 0;
}