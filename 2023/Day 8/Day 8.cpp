#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
std::ifstream fin("input.txt");

struct hash {
	int hashnum = 0;

	hash(std::string s) {
		hashnum = (s[0] * 100 + s[1]) * 100 + s[2];
	}

	hash() { }

	static hash getinfork(std::pair<hash, hash> f, char dir) {
		if (dir == 'L') {
			return f.first;
		}
		return f.second;
	}

	bool endsin(char c) {
		return hashnum % 100 == c;
	}

	hash removelast() {
		hash h;
		h.hashnum = hashnum - hashnum % 100;
		return h;
	}

	bool prefixmatch(hash h) {
		return removelast().hashnum == h.removelast().hashnum;
	}

	std::string tostring() {
		return std::string{ (char)(hashnum / 10000 % 100), (char)(hashnum / 100 % 100), (char)(hashnum % 100) };
	}

	bool operator!=(const hash& h) {
		return this->hashnum != h.hashnum;
	}

	bool operator<(const hash& h) const noexcept
	{
		return hashnum < h.hashnum;
	}

	hash& operator=(const hash& h) {
		hashnum = h.hashnum;
		return *this;
	}
};

long long traverse(std::string path, std::map<hash, std::pair<hash, hash>> forks, hash h) {
	long long pathidx = 0, pathlen = 0;
	while (!h.endsin('Z')) {
		h = hash::getinfork(forks[h], path[pathidx]);
		pathidx = (pathidx + 1) % path.size();
		pathlen++;
	}
	return pathlen;
}

void part1(std::string path, std::map<hash, std::pair<hash, hash>> forks) {
	std::cout << "Part 1: " << traverse(path, forks, hash("AAA")) << "\n";
}

long long gcd(long long a, long long b) {
	if (b == 0) { return a; }
	return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
	if (a > b) { return (a / gcd(a, b)) * b; }
	else { return (b / gcd(a, b)) * a; }
}

void part2(std::string path, std::map<hash, std::pair<hash, hash>> forks) {
	long long pathlen = 1;

	for (std::pair<hash, std::pair<hash, hash>> kv : forks) {
		if (kv.first.endsin('A')) {
			int steps = traverse(path, forks, kv.first);
			pathlen = lcm(pathlen, steps);
		}
	}

	std::cout << "Part 2: " << pathlen << "\n";
}

int main()
{
	std::map<hash, std::pair<hash, hash>> forks;
	std::string path, key, eqsign, left, right;
	fin >> path;

	while (fin >> key >> eqsign >> left >> right) {
		left = left.substr(1, 3);
		right = right.substr(0, 3);

		forks[hash(key)] = std::pair<hash, hash>(hash(left), hash(right));
	}

	part1(path, forks);
	part2(path, forks);
}