#include <iostream>
#include <fstream>
#include <vector>
#include <string>
std::ifstream fin("input.txt");

// {deltaI, deltaJ}
const int DELTA_ORTHOGONAL[][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
const int ORTHOGONAL_DIR_COUNT = sizeof(DELTA_ORTHOGONAL) / sizeof(int) / 2;

struct cell {
	bool visited = false;
	int height;

	cell(int pHeight) : height(pHeight) { }
};

typedef std::vector<cell> line;
typedef std::vector<line> data;

void part1(data);
void part2(data);

int main()
{
	data input;

	std::string line;
	while (std::getline(fin, line)) {
		input.emplace_back();

		for (const char ch : line) {
			input.back().emplace_back(ch - '0');
		}
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

int search_from(data &pInput, bool pMarkPeak, int pI, int pJ, int pLastHeight, int pTargetHeight) {
	if (pI < 0 || pI >= pInput.size()
		|| pJ < 0 || pJ >= pInput[pI].size()
		|| pInput[pI][pJ].visited
		|| pInput[pI][pJ].height != pLastHeight + 1) {
		return 0;
	}

	if (pInput[pI][pJ].height == pTargetHeight) {
		if (pMarkPeak) {
			pInput[pI][pJ].visited = true;
		}
		return 1;
	}

	pInput[pI][pJ].visited = true;

	int result = 0;

	for (int i = 0; i < ORTHOGONAL_DIR_COUNT; i++) {
		result += search_from(pInput, pMarkPeak, pI + DELTA_ORTHOGONAL[i][0], pJ + DELTA_ORTHOGONAL[i][1], pLastHeight + 1, pTargetHeight);
	}

	pInput[pI][pJ].visited = false;

	return result;
}

int search_from(data &pInput, bool pMarkPeak, int pI, int pJ, int pTargetHeight) {
	return search_from(pInput, pMarkPeak, pI, pJ, pInput[pI][pJ].height - 1, pTargetHeight);
}

void part1(data pInput) {
	int sum = 0;

	for (int i = 0; i < pInput.size(); i++) {
		for (int j = 0; j < pInput[i].size(); j++) {
			if (pInput[i][j].height == 0) {
				sum += search_from(pInput, true, i, j, 9);

				for (int i = 0; i < pInput.size(); i++) {
					for (int j = 0; j < pInput[i].size(); j++) {
						pInput[i][j].visited = false;
					}
				}
			}
		}
	}

	std::cout << "part 1: " << sum;
}

void part2(data pInput) {
	int sum = 0;

	for (int i = 0; i < pInput.size(); i++) {
		for (int j = 0; j < pInput[i].size(); j++) {
			if (pInput[i][j].height == 0) {
				sum += search_from(pInput, false, i, j, 9);
			}
		}
	}

	std::cout << "part 2: " << sum;
}