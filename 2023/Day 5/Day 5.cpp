#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

enum RANGEMATCH {
	NONE = 0,
	PARTIALLEFT = 1,
	PARTIALRIGHT = 2,
	FULL = 3,
	BIGGER = 4
};

int guid = 0;
struct range {
	long long start = 0;
	long long length = 0;
	bool mapped = false;
	int id = 0;

	range(long long s, long long l) {
		start = s;
		length = l;
		id = guid++;
	}

	long long end() {
		return start + length - 1;
	}
	
	RANGEMATCH match(range r) {
		long long as = start, ae = end();
		long long bs = r.start, be = r.end();

		if (as >= bs && ae <= be) {
			return RANGEMATCH::FULL;
		}
		else if (as < bs && ae > be) {
			return RANGEMATCH::BIGGER;
		}
		else if (as < bs && ae >= bs && ae <= be) {
			return RANGEMATCH::PARTIALLEFT;
		}
		else if (ae > be && as >= bs && as <= be) {
			return RANGEMATCH::PARTIALRIGHT;
		}
		
		return RANGEMATCH::NONE;
	}

	range subrange(int pos, int len) {
		range r(start + pos, len);
		return r;
	}

	std::string tostring() {
		std::string s;
		s += std::to_string(start);
		s += '-';
		s += std::to_string(end());
		return s;
	}
};

void part1(std::ifstream& fin) {
	long long x, y, z;
	std::list<range> ranges;
	std::string line;

	// seeds
	std::getline(fin, line, ':');
	std::getline(fin, line);
	std::istringstream s(line);
	while (s >> x) {
		ranges.push_back({ x, 1 });
	}

	// rest
	while (std::getline(fin, line)) {
		if (line.empty()) {
			for (range& range : ranges) {
				range.mapped = false;
			}
			continue;
		}

		std::istringstream s(line);
		if (!(s >> x >> y >> z)) { continue; }

		range source(y, z);
		range dest(x, z);

		for (range &r : ranges) {
			if (r.mapped) { continue; }

			RANGEMATCH match = r.match(source);
			if (match == RANGEMATCH::FULL) {
				r.start = r.start - source.start + dest.start;
				r.mapped = true;
			}
		}
	}

	long long lowest = LLONG_MAX;
	for (range& r : ranges) {
		lowest = std::min(lowest, r.start);
	}

	std::cout << "Part 1: " << lowest << "\n";
}

void part2(std::ifstream& fin) {
	long long x, y, z;
	std::list<range> ranges;
	std::string line;

	// seeds
	std::getline(fin, line, ':');
	std::getline(fin, line);
	std::istringstream s(line);
	while (s >> x >> y) {
		ranges.push_back({ x, y });
	}

	// rest
	while (std::getline(fin, line)) {
		if (line.empty()) {
			for (range& range : ranges) {
				range.mapped = false;
			}
			continue;
		}

		std::istringstream s(line);
		if (!(s >> x >> y >> z)) { continue; }

		range source(y, z);
		range dest(x, z);

		for (range& r : ranges) {
			if (r.mapped) { continue; }

			RANGEMATCH match = r.match(source);
			if (match == RANGEMATCH::FULL) {
				r.start = r.start - source.start + dest.start;
				r.mapped = true;
			}
			else if (match == RANGEMATCH::PARTIALLEFT) {
				range sub = r.subrange(0, source.start - r.start);
				r.length -= sub.length;
				r.start += sub.length;
				r.mapped = true;

				r.start = r.start - source.start + dest.start;
				ranges.push_back(sub);
			}
			else if (match == RANGEMATCH::PARTIALRIGHT) {
				range sub = r.subrange(source.end() - r.start + 1, r.end() - source.end());
				r.length -= sub.length;
				r.mapped = true;

				r.start = r.start - source.start + dest.start;
				ranges.push_back(sub);
			}
			else if (match == RANGEMATCH::BIGGER) {
				range sub1 = r.subrange(0, source.start - r.start);
				range sub2 = r.subrange(r.length - r.end() + source.end(), r.end() - source.end());
				r.length -= sub1.length + sub2.length;
				r.start += sub1.length;
				r.mapped = true;

				r.start = r.start - source.start + dest.start;
				ranges.push_back(sub1);
				ranges.push_back(sub2);
			}
		}
	}

	long long lowest = LLONG_MAX;
	for (range& range : ranges) {
		lowest = std::min(lowest, range.start);
	}

	std::cout << "Part 2: " << lowest << "\n";
}

int main() {
	std::ifstream fin("input.txt");
	part1(fin);

	std::ifstream fin2("input.txt");
	part2(fin2);

	return 0;
}