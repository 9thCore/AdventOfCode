#include <iostream>
#include <fstream>
constexpr int GRID_SIZE = 300;
constexpr int XOFFSET[8] = {-1, 1,  0, 0, -1, -1, 1,  1};
constexpr int YOFFSET[8] = { 0, 0, -1, 1, -1,  1, 1, -1};
constexpr int NUMOFFSET[2] = { -1, 1 };

bool part2 = true;

std::ifstream fin("input.txt");

struct cell {
	char c = '.';
	bool ispart = false;
	int gearadjacency = 0;
} cells[GRID_SIZE][GRID_SIZE];

bool symbol(char c) {
	return !isdigit(c) && c != '.';
}

bool validpos(int i, int j, int n, int m) {
	return i >= 0 && i < n && j >= 0 && j < m;
}

// but only horizontally. numbers are horizontal
void dfs(int i, int j, int n, int m) {
	cells[i][j].ispart = true;
	for (int k = 0; k < 2; k++) {
		int off = NUMOFFSET[k];
		if (validpos(i, j + off, n, m) && isdigit(cells[i][j + off].c) && !cells[i][j + off].ispart) {
			dfs(i, j + off, n, m);
		}
	}
}

int main()
{
	std::string line;
	int n = 0, m = 0;

	while (fin >> line) {
		for (m = 0; line[m]; m++) {
			cells[n][m].c = line[m];
		}
		n++;
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (symbol(cells[i][j].c)) {
				for (int k = 0; k < 8; k++) {
					int xoff = XOFFSET[k];
					int yoff = YOFFSET[k];
					if (validpos(i + yoff, j + xoff, n, m)
						&& isdigit(cells[i + yoff][j + xoff].c)
						&& !cells[i + yoff][j + xoff].ispart) {
						dfs(i + yoff, j + xoff, n, m);
						cells[i][j].gearadjacency++;
					}
				}
			}
		}
	}

	int sum = 0;

	if (part2) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (cells[i][j].c == '*' && cells[i][j].gearadjacency == 2) {
					int p = 1;
					// this sucks
					for (int k = 0; k < 8; k++) {
						int num = 0;

						int xoff = XOFFSET[k];
						int yoff = YOFFSET[k];
						if (cells[i + yoff][j + xoff].ispart) {
							int numstartidx = j + xoff;
							while (numstartidx >= 0 && cells[i + yoff][numstartidx].ispart) { numstartidx--; }
							while (isdigit(cells[i + yoff][++numstartidx].c)) { cells[i + yoff][numstartidx].ispart = false; num = num * 10 + cells[i + yoff][numstartidx].c - '0'; }
							p *= num;
						}
					}

					sum += p;
				}
			}
		}
	}
	else {
		int num = 0;
		bool partnum = false;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				char c = cells[i][j].c;
				if (cells[i][j].ispart) {
					partnum = true;
				}

				if (isdigit(c)) {
					num = num * 10 + c - '0';
				}
				else {
					if (partnum) {
						sum += num;
					}
					num = 0;
					partnum = false;
				}
			}
			sum += num;
			num = 0;
			partnum = false;
		}
		if (partnum) {
			sum += num;
		}
	}
	
	std::cout << sum;

	return 0;
}