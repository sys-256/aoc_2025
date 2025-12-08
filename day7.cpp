#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::pair<long, long>, long> table;

class Beam {
	private:
	std::pair<long, long> beam_position; // row, column
	std::vector<std::vector<bool>> splitters;

	public:
	Beam(std::pair<long, long> pos, std::vector<std::vector<bool>> split):
		beam_position(pos), splitters(split) {}

	long get_timelines() {
		beam_position.first += 1;

		if (table.find(beam_position) != table.end()) {
			return table[beam_position];
		}
		if (beam_position.first == splitters.size()) {
			table.insert({beam_position, 1L});
			return 1;
		}
		if (splitters[beam_position.first][beam_position.second]) {
			Beam beamL({beam_position.first, beam_position.second - 1}, splitters);
			Beam beamR({beam_position.first, beam_position.second + 1}, splitters);
			long result = beamL.get_timelines() + beamR.get_timelines();
			table.insert({beam_position, result});
			return result;
		}
		return get_timelines();
	}
};

class Day7 {
	private:
	std::map<long, long> beam_positions; // column, row
	std::vector<std::vector<bool>> splitters;
	long total_splits;

	bool iterate() {
		std::vector<std::pair<long, long>> to_add;
		std::vector<long> to_erase;
		for (auto& pair : beam_positions) {
			pair.second += 1;
			if (pair.second == splitters.size()) {
				return false; // Stop the program
			}
			if (splitters[pair.second][pair.first]) {
				// If this beam is now on a splitter
				to_add.push_back(std::make_pair(pair.first - 1, pair.second));
				to_add.push_back(std::make_pair(pair.first + 1, pair.second));
				to_erase.push_back(pair.first);
				total_splits += 1;
			}
		}
		for (auto pair : to_add) {
			beam_positions.insert(pair);
		}
		for (auto key : to_erase) {
			beam_positions.erase(key);
		}
		return true;
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
					beam_positions.insert(std::make_pair(i, 0));
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
		while (iterate()) {}
		return total_splits;
	}

	long get_timelines() {
		Beam beam({beam_positions.begin()->second, beam_positions.begin()->first}, splitters);
		return beam.get_timelines();
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

    std::cout << day7.get_timelines() << "\n";

    file.close();
    return 0;
}