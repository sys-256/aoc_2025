#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

class Day9 {
	private:
	std::vector<std::pair<std::int64_t, std::int64_t>> red_tiles; // In the form of {x, y}
	std::map<std::pair<std::int64_t, std::int64_t>, bool> valid_points;

	bool valid_area(std::pair<std::int64_t, std::int64_t> tile1, std::pair<std::int64_t, std::int64_t> tile2) {
		// For every point in the area, check if it's in a red or green tile
		for (std::int64_t i = std::min(tile1.first, tile2.first); i <= std::max(tile1.first, tile2.first); i++) {
			for (std::int64_t j = std::min(tile1.second, tile2.second); j <= std::max(tile1.second, tile2.second); j++) {
				if (!valid_point({i, j})) return false;
			}
		}
		return true;
	}

	bool valid_point(std::pair<std::int64_t, std::int64_t> pair) {
		// Broken code
		if (auto search = valid_points.find(pair); search != valid_points.end()) {
			return search->second;
		}

		bool inside = false;
		for (auto p1 = red_tiles.begin(), p2 = red_tiles.begin() + 1; p1 < red_tiles.end() && p2 <= red_tiles.end(); p1++, p2++) {
			if (p2 == red_tiles.end()) p2 = red_tiles.begin(); // Wrap around logic

			// We can determine if a point is inside using only the vertical lines, so discord horizontal ones
			if (p1->second == p2->second) {
				// Except if the point is on that horizontal line
				if (pair.second == p1->second && pair.first <= std::max(p1->first, p2->first) && pair.first >= std::min(p1->first, p2->first)) {
					valid_points.insert({pair, true});
					return true;
				}
				continue;
			}

			// If the point is above or below this line entirely, discord this line
			if (pair.second > std::max(p1->second, p2->second) || pair.second < std::min(p1->second, p2->second)) {
				continue;
			}

			// If the point is on this line, we can already return true
			if (pair.first == p1->first) {
				valid_points.insert({pair, true});
				return true;
			}

			// If the point is left of this line, we change the inside boolean
			// If it's left of 2 (or any even number) of lines, it's outside of the polygon
			// If it's left of an odd number of lines, it's inside the polygon
			if (pair.first < p1->first) {
				inside = !inside;
			}
		}
		valid_points.insert({pair, inside});
		return inside;
	}

	public:
	void add_row(std::string row) {
		std::stringstream ss(row);
		std::string d1, d2;
		std::getline(ss, d1, ',');
		std::getline(ss, d2, ',');
		red_tiles.push_back({std::stoll(d1), std::stoll(d2)});
	}

	std::int64_t get_max_area() {
		std::int64_t max_area = -1;
		for (std::int64_t i = 0; i < red_tiles.size() - 1; i++) {
			for (std::int64_t j = i + 1; j < red_tiles.size(); j++) {
				auto p1 = red_tiles[i];
				auto p2 = red_tiles[j];
				std::int64_t area = (std::abs(p1.first - p2.first) + 1) * (std::abs(p1.second - p2.second) + 1);
				if (area > max_area) {
					if (area > 24775315 && area <= 40000000 && valid_area(p1, p2)) {
						max_area = area;
					}
				}
			}
		}
		return max_area;
	}
};

int main() {
	Day9 day9;
	std::ifstream file("day9_final.input");

	if (!file.is_open()) {
		std::cerr << "Error opening the file! Exiting.\n";
		return 1;
	}

	std::string line;
	while (std::getline(file, line)) {
		day9.add_row(line);
	}

	std::cout << day9.get_max_area() << "\n";

	file.close();
	return 0;
}