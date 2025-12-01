#if defined(_MSC_VER)
#define strtok_r strtok_s
#endif

#include <fstream>
#include <string.h>
#include <iostream>
constexpr int LINE_MAX_COUNT = 256;

std::ifstream fin("input.txt");

void readnum(const char* str, int& num) {
	num = 0;
	for (int i = 0; str[i]; i++) {
		if (isdigit(str[i])) {
			num = num * 10 + str[i] - '0';
		}
	}
}

bool part2 = true;

int main()
{
	int sum = 0;
	char line[LINE_MAX_COUNT + 1];

	while (fin.getline(line, LINE_MAX_COUNT)) {
		int minred = 0, mingreen = 0, minblue = 0;
		bool possible = true;
		int gameId;

		char *n, *n2;
		char *p = strtok_r(line, ":", &n), *p2;
		readnum(p, gameId);

		p = strtok_r(NULL, ";", &n);

		while (p) {
			int red = 0, green = 0, blue = 0;
			int num;
			p2 = strtok_r(p, ",", &n2);

			while (p2) {
				readnum(p2, num);

				if (strstr(p2, "red")) {
					red += num;
				}
				else if (strstr(p2, "green")) {
					green += num;
				}
				else if (strstr(p2, "blue")) {
					blue += num;
				}

				p2 = strtok_r(NULL, ",", &n2);
			}

			if (part2) {
				minred = std::max(minred, red);
				mingreen = std::max(mingreen, green);
				minblue = std::max(minblue, blue);
			}
			else {
				if (red > 12 || green > 13 || blue > 14) {
					possible = false;
					break;
				}
			}

			p = strtok_r(NULL, ";", &n);
		}

		int power = minred * mingreen * minblue;

		if (part2) {
			sum += power;
		} else if (possible) {
			sum += gameId;
		}
	}

	std::cout << sum;
}