#include <cstdint>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

class Infi {
    private:
    std::vector<std::vector<std::int8_t>> grid;
    std::int64_t trees_cut = 0;
    std::int8_t direction = 0; // 0 is north, 3 is west

    void simulate_day() {
        auto just_added = add_seeds();
        grow_trees(just_added);
        cut_trees();
        direction += 1;
        direction %= 4;
    }

    std::vector<std::pair<std::int64_t, std::int64_t>> add_seeds() {
        std::vector<std::pair<std::int64_t, std::int64_t>> just_added;
        for (std::int64_t i = 0; i < grid.size(); i++) {
            for (std::int64_t j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] != -1) {
                    continue;
                }
                if (!receives_light(i, j)) {
                    continue;
                }
                if (has_height(i-2, j, 2) +
                    has_height(i+2, j, 2) +
                    has_height(i-1, j-1, 2) +
                    has_height(i-1, j+1, 2) +
                    has_height(i+1, j-1, 2) +
                    has_height(i+1, j+1, 2) >= 2) {
                    // If this is true, it's adjacent to at least 2 trees with a height >= 2
                    grid[i][j] = 0;
                    just_added.push_back({i, j});
                }
            }
        }
        return just_added;
    }

    bool has_height(std::int64_t i, std::int64_t j, std::int8_t min_height) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size()) {
            return false;
        }
        return grid[i][j] >= min_height;
    }

    void grow_trees(std::vector<std::pair<std::int64_t, std::int64_t>> just_added) {
        std::vector<std::pair<std::int64_t, std::int64_t>> to_grow;
        for (std::int64_t i = 0; i < grid.size(); i++) {
            for (std::int64_t j = 0; j < grid[i].size(); j++) {
                auto was_just_added = std::find(just_added.begin(), just_added.end(), std::make_pair(i, j));
                if (grid[i][j] >= 0 && was_just_added == just_added.end() && receives_light(i, j)) {
                    // grid[i][j] += 1;
                    to_grow.push_back({i, j});
                }
            }
        }
        for (auto pair : to_grow) {
            grid[pair.first][pair.second] += 1;
        }
    }

    void cut_trees() {
        for (std::int64_t i = 0; i < grid.size(); i++) {
            for (std::int64_t j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == 5) {
                    grid[i][j] = -1;
                    trees_cut += 1;
                }
            }
        }
    }

    bool receives_light(std::int64_t i, std::int64_t j) {
        if (direction == 0) {
            // North
            for (std::int64_t row_i = 0; row_i < i; row_i++) {
                if (grid[row_i][j] > 0 && grid[row_i][j] >= grid[i][j]) {
                    return false;
                }
            }
            return true;
        } else if (direction == 1) {
            // East
            for (std::int64_t col_j = grid[i].size() - 1; col_j > j; col_j--) {
                if (grid[i][col_j] > 0 && grid[i][col_j] >= grid[i][j]) {
                    return false;
                }
            }
            return true;
        } else if (direction == 2) {
            // South
            for (std::int64_t row_i = grid.size() - 1; row_i > i; row_i--) {
                if (grid[row_i][j] > 0 && grid[row_i][j] >= grid[i][j]) {
                    return false;
                }
            }
            return true;
        } else {
            // West
            for (std::int64_t col_j = 0; col_j < j; col_j++) {
                if (grid[i][col_j] > 0 && grid[i][col_j] >= grid[i][j]) {
                    return false;
                }
            }
            return true;
        }
    }

    public:
    void add_line(std::string line) {
        // Lines with numbers in them start with '/' or '\'
        if (line[0] != '/' && line[0] != '\\') {
            return;
        }

        bool offset = line[0] == '\\'; // If it starts '\', we have an offset of 1
        std::regex pattern("/(\\d|(?=\\s))"); // Must start with / and then either a digit or a space
        auto matches_begin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto matches_end = std::sregex_iterator();
        // Default value of -2 means not in use
        std::vector<std::int8_t> row_vec(2 * std::distance(matches_begin, matches_end), -2);

        long index = 0;
        for (auto it = matches_begin; it != matches_end; it++) {
            std::smatch match = *it;
            // Space isn't captured, so if length > 1, there was also a digit
            if (match.str().length() > 1) {
                row_vec[2*index+offset] = match.str()[1] - '0';
            } else {
                // -1 means in use, but no value
                row_vec[2*index+offset] = -1;
            }
            index += 1;
        }
        grid.push_back(row_vec);
    }

    void simulate_days(std::int64_t n) {
        for (std::int64_t i = 0; i < n; i++) {
            simulate_day();
        }
    }

    std::int64_t get_trees_cut() {
        return trees_cut;
    }
};

int main() {
    Infi infi;
    std::ifstream file("infi_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    bool switched = false;
    while (std::getline(file, line)) {
        infi.add_line(line);
    }

    infi.simulate_days(256);

    std::cout << infi.get_trees_cut() << "\n";

    file.close();
    return 0;
}