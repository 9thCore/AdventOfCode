#if defined(_MSC_VER)
#define strtok_r strtok_s
#endif

#include <fstream>
#include <iostream>
#include <string>
#include <functional>
std::ifstream fin("input.txt");

constexpr int MAXLEN = 256;
constexpr int MAXNUM = 100;
constexpr int CARDCNT = 204;

void readnum(char* s, const char* delim, std::function<void(int)> f) {
	char* c = NULL;
	char* p = strtok_r(s, delim, &c);

	while (p) {
		int num = std::stoi(p);
		f(num);
		p = strtok_r(NULL, delim, &c);
	}
}

void clear(bool* arr, int len) {
	for (int i = 0; i < len; i++) {
		arr[i] = 0;
	}
}

void clear(int* arr, int len) {
	for (int i = 0; i < len; i++) {
		arr[i] = 0;
	}
}

int main()
{
	int cardcopies[CARDCNT];
	bool winners[MAXNUM];
	char line[MAXLEN];
	
	int sum1 = 0, sum2 = 0;

	clear(cardcopies, CARDCNT);
	while (fin.getline(line, MAXLEN)) {
		clear(winners, MAXNUM);

		char* context = NULL;
		char* p = strtok_r(line, ":", &context);
		int id = std::stol(p+4);

		p = strtok_r(NULL, ":", &context);

		p = strtok_r(p, "|", &context);
		readnum(p, " ", [&](int n) {
			winners[n] = true;
		});

		bool matchedany = false;
		int matched = 0, points = 1;
		p = strtok_r(NULL, "|", &context);
		readnum(p, " ", [&](int n) {
			if (winners[n]) {
				matched++;
				points *= 2;
				matchedany = true;
			}
		});

		if (matchedany) {
			sum1 += points/2;
		}

		for (int i = 0; i < matched; i++) {
			cardcopies[id+i] += cardcopies[id-1] + 1;
			sum2 += cardcopies[id - 1] + 1;
		}

		sum2++;
	}

	std::cout << "Part 1: " << sum1 << "\n";
	std::cout << "Part 2: " << sum2;
}