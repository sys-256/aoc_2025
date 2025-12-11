#include <cstdint>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

class Box {
	private:
	std::int64_t position[3];

	public:
	Box(){}
	Box(std::string row) {
		std::regex pattern("\\d+");
		std::int64_t i = 0;
		for (auto it = std::sregex_iterator(row.begin(), row.end(), pattern); it != std::sregex_iterator(); it++) {
			std::smatch match = *it;
			position[i] = std::stoll(match.str());
			i += 1;
		}
	}

	std::int64_t* get_positions() {
		return position;
	}

	bool operator<(const Box& other) const {
		return position[0] < other.position[0];
	}

	bool operator==(const Box& other) const {
		return position[0] == other.position[0] && position[1] == other.position[1] && position[2] == other.position[2];
	}

	friend std::ostream& operator<<(std::ostream& os, const Box& box) {
		os << "(" << box.position[0] << ", " << box.position[1] << ", " << box.position[2] << ")"; 
		return os;
	}
};

class Day8 {
	// This implementation works well for the practice, but is too high
	// for the final, and I'm not sure why. The graph implementation does
	// work well though
	private:
	std::vector<std::vector<Box>> circuits;
	std::map<std::pair<Box, Box>, double> distances;

	double get_distance(Box box1, Box box2) {
		std::int64_t *pos1 = box1.get_positions();
		std::int64_t *pos2 = box2.get_positions();
		return sqrt(pow(pos1[0]-pos2[0], 2)+pow(pos1[1]-pos2[1], 2)+pow(pos1[2]-pos2[2], 2));
	}

	auto get_minimum_distance() {
		return std::min_element(distances.begin(), distances.end(), [](const auto& L, const auto& R) {
			return L.second < R.second;
		});
	}

	public:
	void add_row(std::string row) {
		circuits.push_back({Box(row)});
	}

	void add_distances() {
		for (std::int64_t i = 0; i < circuits.size(); i++) {
			for (std::int64_t j = i + 1; j < circuits.size(); j++) {
				Box box1 = circuits[i][0], box2 = circuits[j][0];
				distances.insert({{box1, box2}, get_distance(box1, box2)});
			}
		}
	}

	void optimize_circuits() {
		// TODO only 10 connections for practice (1000 for final)
		for (std::int64_t i = 0; i < 10; i++) {
			auto minimum = get_minimum_distance();
			auto box1 = (*minimum).first.first;
			auto box2 = (*minimum).first.second;

			auto it_both = std::find_if(circuits.begin(), circuits.end(), [&box1, &box2](const std::vector<Box>& circuit) {
				return std::count(circuit.begin(), circuit.end(), box1) > 0 &&
					std::count(circuit.begin(), circuit.end(), box2) > 0;
			});

			if (it_both == circuits.end()) {
				// If there's no circuit with both box1 and box2,
				// move everything in circuit with box2 in it to circuit of box1
				auto it_box1 = std::find_if(circuits.begin(), circuits.end(), [&box1](const std::vector<Box>& circuit) {
					return std::count(circuit.begin(), circuit.end(), box1) > 0;
				});
				for (
				auto it_box2 = std::find_if(circuits.begin(), circuits.end(), [&box2](const std::vector<Box>& circuit) {
					return std::count(circuit.begin(), circuit.end(), box2) > 0;
				});
				it_box2 != circuits.end();
				it_box2 = std::find_if(it_box2 + 1, circuits.end(), [&box2](const std::vector<Box>& circuit) {
					return std::count(circuit.begin(), circuit.end(), box2) > 0;
				})) {
					for (Box box : *it_box2) {
						it_box1->push_back(box);
					}
					circuits.erase(it_box2);
				}
			}

			distances.erase(minimum);
		}
	}

	std::int64_t get_largest() {
		std::sort(circuits.begin(), circuits.end(), [](std::vector<Box>& a, std::vector<Box>& b) {
			return a.size() > b.size();
		});
		return circuits[0].size() * circuits[1].size() * circuits[2].size();
	}
};

class Day8_new {
	private:
	std::map<std::pair<size_t, size_t>, double> distances;
	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Box> g;

	double get_distance(Box box1, Box box2) {
		std::int64_t *pos1 = box1.get_positions();
		std::int64_t *pos2 = box2.get_positions();
		return sqrt(pow(pos1[0]-pos2[0], 2)+pow(pos1[1]-pos2[1], 2)+pow(pos1[2]-pos2[2], 2));
	}

	auto get_minimum_distance() {
		return std::min_element(distances.begin(), distances.end(), [](const auto& L, const auto& R) {
			return L.second < R.second;
		});
	}

	public:
	void add_row(std::string row) {
		auto v1 = boost::add_vertex(Box(row), g);
	}

	void add_distances() {
		for (auto vi = boost::vertices(g).first; vi != boost::vertices(g).second; vi++) {
			for (auto vj = vi + 1; vj != boost::vertices(g).second; vj++) {
				distances.insert({{*vi, *vj}, get_distance(g[*vi], g[*vj])});
			}
		}
	}

	void optimize_circuits_part1() {
		// TODO only 10 connections for practice (1000 for final)
		for (std::int64_t i = 0; i < 1000; i++) {
			auto minimum = get_minimum_distance();
			boost::add_edge(minimum->first.first, minimum->first.second, g);
			distances.erase(minimum);
		}
	}

	std::int64_t get_largest() {
		// Vector to hold the component index for each vertex
		std::vector<int> component(num_vertices(g));
		int num_components = boost::connected_components(g, &component[0]);

		// Vector to count sizes of each component
		std::vector<int> sizes(num_components, 0);
		for (const auto& comp : component) {
			sizes[comp] += 1;
		}

		std::sort(sizes.begin(), sizes.end(), std::greater<>());
		return sizes[0]*sizes[1]*sizes[2];
	}

	std::int64_t get_last() {
		std::vector<int> component(num_vertices(g));
		Box last_added_1, last_added_2;
		while (boost::connected_components(g, &component[0]) != 1) {
			auto minimum = get_minimum_distance();
			boost::add_edge(minimum->first.first, minimum->first.second, g);
			distances.erase(minimum);
			last_added_1 = g[minimum->first.first];
			last_added_2 = g[minimum->first.second];
		}
		return last_added_1.get_positions()[0] * last_added_2.get_positions()[0];
	}
};

int main() {
	Day8_new day8;
	std::ifstream file("day8_final.input");

	if (!file.is_open()) {
		std::cerr << "Error opening the file! Exiting.\n";
		return 1;
	}

	std::string line;
	while (std::getline(file, line)) {
		day8.add_row(line);
	}

	day8.add_distances();
	// day8.optimize_circuits();

	std::cout << day8.get_last() << "\n";

	file.close();
	return 0;
}