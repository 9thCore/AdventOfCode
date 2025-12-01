#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <tuple>
#include <string>
std::ifstream fin("input.txt");

const int DELTA[][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

struct region {
	long long area, perimeter, sides;
};

typedef std::vector<region> data;
typedef std::vector<std::vector<short>> map;

void process(const map &, data &);
void part1(const map &, const data &);
void part2(const map &, const data &);

int main()
{
	map regionMap;
	data input;

	std::string line;

	while (std::getline(fin, line)) {
		regionMap.emplace_back();

		for (const char ch : line) {
			regionMap.back().push_back(ch);
		}
	}

	process(regionMap, input);
	part1(regionMap, input);
	std::cout << "\n";
	part2(regionMap, input);
}

void part2(const map &pMap, const data &pInput) {
	long long sum = 0;

	for (const auto &region : pInput) {
		sum += region.area * region.sides;
	}

	std::cout << "part 2: " << sum;
}

void part1(const map &pMap, const data &pInput) {
	long long sum = 0;

	for (const auto &region : pInput) {
		sum += region.area * region.perimeter;
	}

	std::cout << "part 1: " << sum;
}

long long calculate_sides(const map &, int);

void bfs(const map &pMap, map &pExplored, data &pInput, int pOriginI, int pOriginJ) {
	short match = pMap[pOriginI][pOriginJ];

	pInput.emplace_back();
	pInput.back().area = 1;
	pInput.back().sides = 0;

	map explored;
	explored.resize(pMap.size());
	for (int i = 0; i < pMap.size(); i++) {
		explored[i].resize(pMap[i].size());
	}

	std::queue<std::tuple<int, int>> queue;
	queue.emplace(pOriginI, pOriginJ);
	pExplored[pOriginI][pOriginJ] = 1;
	explored[pOriginI][pOriginJ] = 1;

	while (!queue.empty()) {
		int i = std::get<0>(queue.front()), j = std::get<1>(queue.front());
		queue.pop();

		pInput.back().perimeter += 4;

		for (int dir = 0; dir < 4; dir++) {
			int newI = i + DELTA[dir][0], newJ = j + DELTA[dir][1];
			
			if (newI < 0 || newI >= pMap.size()
				|| newJ < 0 || newJ >= pMap[newI].size()
				|| pMap[newI][newJ] != match) {
				continue;
			}

			// count neighbouring region cells,
			// even if they were already marked
			if (pExplored[newI][newJ]) {
				pInput.back().perimeter--;
				continue;
			}

			queue.emplace(newI, newJ);
			pExplored[newI][newJ] = 1;
			explored[newI][newJ] = 1;
			pInput.back().area++;
			pInput.back().perimeter--;
		}
	}

	for (int dir = 0; dir < 4; dir++) {
		pInput.back().sides += calculate_sides(explored, dir);
	}
}

void process(const map &pMap, data &pInput) {
	map explored;
	explored.resize(pMap.size());

	for (int i = 0; i < pMap.size(); i++) {
		explored[i].resize(pMap[i].size());
	}

	for (int i = 0; i < pMap.size(); i++) {
		for (int j = 0; j < pMap[i].size(); j++) {
			if (!explored[i][j]) {
				bfs(pMap, explored, pInput, i, j);
			}
		}
	}
}

long long calculate_sides(const map &pExplored, int pDir) {
	long long count = 0;

	if (pDir % 2 == 0) {
		for (int i = 0; i < pExplored.size(); i++) {
			for (int j = 0; j < pExplored[i].size(); j++) {
				if (!pExplored[i][j]) {
					continue;
				}

				int offsetI = i + DELTA[pDir][0];
				bool outside = offsetI < 0 || offsetI >= pExplored.size();
				if (outside || !pExplored[offsetI][j]) {
					count++;
					while (j < pExplored[i].size() && pExplored[i][j] && (outside || !pExplored[offsetI][j])) {
						j++;
					}
				}
			}
		}
	}
	else {
		for (int j = 0; j < pExplored[0].size(); j++) {
			for (int i = 0; i < pExplored.size(); i++) {
				if (!pExplored[i][j]) {
					continue;
				}

				int offsetJ = j + DELTA[pDir][1];
				bool outside = offsetJ < 0 || offsetJ >= pExplored[0].size();
				if (outside || !pExplored[i][offsetJ]) {
					count++;
					while (i < pExplored.size() && pExplored[i][j] && (outside || !pExplored[i][offsetJ])) {
						i++;
					}
				}
			}
		}
	}

	return count;
}