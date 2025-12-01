#include <iostream>
#include <fstream>
#include <string>
#include <vector>
std::ifstream fin("input.txt");

typedef std::vector<std::vector<bool>> grid;
typedef std::vector<std::vector<int>> grid_int;

struct point {
	int i, j;
};

void part1(const grid &, point);
void part2(grid, const point &);
void part2_bad(const grid &, point);

int main()
{
	grid input;
	point guardPos = { 0, 0 };
	std::string line;

	int i = 0;
	while (std::getline(fin, line)) {
		input.emplace_back();

		for (int j = 0; j < line.size(); j++) {
			// '.', '^' -> free
			// '#' -> wall
			input.back().push_back((line[j] == '#'));

			if (line[j] == '^') {
				guardPos = { i, j };
			}
		}

		i++;
	}

	part1(input, guardPos);
	std::cout << "\n";
	part2(input, guardPos);
	std::cout << "\a";
}

enum dir {
	NONE = 0,
	LEFT = 1,
	UP = 2,
	RIGHT = 3,
	DOWN = 4
};

dir rotate(const dir &pDir) {
	switch (pDir) {
	case LEFT:
		return UP;
	case UP:
		return RIGHT;
	case RIGHT:
		return DOWN;
	case DOWN:
		return LEFT;
	}
	return NONE;
}

int hash(const dir &pDir) {
	return 1 << (int)pDir;
}

bool move(const grid &pGrid, dir pDir, point &pPos, bool &pWall) {
	pWall = false;

	bool horizontal = pDir & 1;
	bool decrease = pDir < 3;
	int delta = decrease ? -1 : 1;
	point newPoint = {
		!horizontal ? (pPos.i + delta) : pPos.i,
		horizontal ? (pPos.j + delta) : pPos.j
	};

	if (newPoint.i < 0 || newPoint.i >= pGrid.size()
		|| newPoint.j < 0 || newPoint.j >= pGrid[0].size()) {
		return false;
	}

	if (pGrid[newPoint.i][newPoint.j]) {
		pWall = true;
		return true;
	}

	pPos = newPoint;
	return true;
}

void part1(const grid &pGrid, point pGuardPos) {
	dir guardDir = UP;
	grid explored(pGrid.size(), std::vector<bool>(pGrid[0].size()));
	int count = 0;
	bool lookingAtWall = false;

	do {
		if (lookingAtWall) {
			guardDir = rotate(guardDir);
		}
		else {
			count += !explored[pGuardPos.i][pGuardPos.j];
			explored[pGuardPos.i][pGuardPos.j] = 1;
		}
	} while (move(pGrid, guardDir, pGuardPos, lookingAtWall));

	std::cout << "part 1: " << count;
}

bool has_loop(const grid &pGrid, point pGuardPos) {
	dir guardDir = UP;
	grid_int explored(pGrid.size(), std::vector<int>(pGrid[0].size()));
	bool lookingAtWall = false;

	do {
		if (lookingAtWall) {
			guardDir = rotate(guardDir);
		}

		if ((explored[pGuardPos.i][pGuardPos.j] & hash(guardDir)) != 0) {
			return true;
		}

		explored[pGuardPos.i][pGuardPos.j] |= hash(guardDir);
	} while (move(pGrid, guardDir, pGuardPos, lookingAtWall));

	return false;
}

void part2(grid pGrid, const point &pGuardPos) {
	int count = 0;

	for (int i = 0; i < pGrid.size(); i++) {
		for (int j = 0; j < pGrid[i].size(); j++) {
			if (!pGrid[i][j]) {
				pGrid[i][j] = 1;
				count += has_loop(pGrid, pGuardPos);
				pGrid[i][j] = 0;
			}
		}
	}

	std::cout << "part 2: " << count;
}

// "smart" way of solving it
// ...but it does not work (finds extra cases)
// whatever...
void part2_bad(const grid &pGrid, point pGuardPos) {
	dir guardDir = UP;
	grid_int explored(pGrid.size(), std::vector<int>(pGrid[0].size()));
	int count = 0;
	bool lookingAtWall = false;

	do {

		if (lookingAtWall) {
			guardDir = rotate(guardDir);
		}

		dir rotated = rotate(guardDir);
		point iterator = pGuardPos;
		bool lookingAtWallSquared = false;
		int limit = 130 * 130;

		do {
			if (lookingAtWallSquared) {
				rotated = rotate(rotated);
			}

			if ((explored[iterator.i][iterator.j] & hash(rotated)) != 0) {
				count++;
				// std::cout << pGuardPos.i << " " << pGuardPos.j << ": " << iterator.i << ", " << iterator.j << ": " << rotated << " (" << guardDir << ")\n";
				break;
			}
		} while (limit-- >= 0 && move(pGrid, rotated, iterator, lookingAtWallSquared));

		explored[pGuardPos.i][pGuardPos.j] |= hash(guardDir);

		/*
		for (int i = 0; i < pGrid.size(); i++) {
			for (int j = 0; j < pGrid[0].size(); j++) {
				bool vert = (explored[i][j] & hash(UP)) || (explored[i][j] & hash(DOWN));
				bool hori = (explored[i][j] & hash(LEFT)) || (explored[i][j] & hash(RIGHT));
				if (vert && hori) {
					std::cout << '+';
				}
				else if (vert && !hori) {
					std::cout << '|';
				}
				else if (!vert && hori) {
					std::cout << '-';
				}
				else {
					std::cout << (pGrid[i][j] ? '#' : ' ');
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n----------------------\n\n\n----------------------\n";
		*/

	} while (move(pGrid, guardDir, pGuardPos, lookingAtWall));

	std::cout << "part 2: " << count;
}