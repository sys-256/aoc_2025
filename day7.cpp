#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class Day7 {
	private:
	std::vector<std::pair<long, long>> beam_positions;
	std::vector<std::vector<bool>> splitters;
	long total_splits;

	bool iterate() {
		std::vector<std::pair<long, long>> new_beams;
		for (long i = 0; i < beam_positions.size(); i++) {
			if (beam_positions[i].first < 0) continue; // Disabled beams
			
			beam_positions[i].first += 1;
			if (beam_positions[i].first == splitters.size()) {
				return false; // Stop the program
			}
			if (splitters[beam_positions[i].first][beam_positions[i].second]) {
				// If this beam is now on a splitter
				new_beams.push_back({beam_positions[i].first, beam_positions[i].second - 1});
				new_beams.push_back({beam_positions[i].first, beam_positions[i].second + 1});
				beam_positions[i] = {-1, -1};
				total_splits += 1;
			}
		}
		for (auto beam : new_beams) {
			insert_beam(beam);
		}
		return true;
	}

	void insert_beam(std::pair<long, long> beam) {
		if (std::count(beam_positions.begin(), beam_positions.end(), beam) <= 0) {
			beam_positions.push_back(beam);
		}
	}

	public:
	Day7() {
		total_splits = 0;
	}

	void add_row(std::string row) {
		std::vector<bool> splitter_row;
		for (long i = 0; i < row.length(); i++) {
			switch (row[i]) {
				case 'S':
					// This assumes there's only 1 S which is
					// on the first row
					beam_positions = {{0, i}};
				case '.':
					splitter_row.push_back(false);
					break;
				case '^':
					splitter_row.push_back(true);
					break;
			}
		}
		splitters.push_back(splitter_row);
	}

	long get_splits() {
		while (iterate()) {
			std::cout << "";
		}
		return total_splits;
	}
};

int main() {
    Day7 day7;
    std::ifstream file("day7_final.input");

    if (!file.is_open()) {
        std::cerr << "Error opening the file! Exiting.\n";
        return 1;
    }

    std::string line;
    bool switched = false;
    while (std::getline(file, line)) {
        day7.add_row(line);
    }

    std::cout << day7.get_splits() << "\n";

    file.close();
    return 0;
}