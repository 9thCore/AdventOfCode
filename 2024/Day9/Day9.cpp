#include <iostream>
#include <fstream>
#include <vector>
#include <string>
std::ifstream fin("input.txt");

struct memblock {
	int id;
	int start = 0;
	int length = 0;

	memblock(int pId, int pStart, int pLength) : id(pId), start(pStart), length(pLength) {	}

	memblock &operator =(const memblock &pOther) {
		id = pOther.id;
		start = pOther.start;
		length = pOther.length;
		return *this;
	}
};

typedef std::vector<memblock> data;

void print(const data &);
void part1(data);
void part2(data);

int main()
{
	data input;
	std::string line;

	std::getline(fin, line);

	int iterator = 0, globalIndex = 0;
	bool usedSpace = true;

	for (const char ch : line) {
		int repeats = ch - '0';
		if (usedSpace) {
			input.push_back({ globalIndex++, iterator, repeats });
		}

		usedSpace = !usedSpace;
		iterator += repeats;
	}

	part1(input);
	std::cout << "\n";
	part2(input);
}

// debug functionality
void print(const data &pInput) {
	for (auto iterator = pInput.cbegin(), end = pInput.cend(); iterator != end; iterator++) {
		const memblock &block = *iterator;
		for (int i = 0; i < block.length; i++) {
			std::cout << block.id;
		}

		if (iterator != end - 1) {
			const memblock &next = *(iterator + 1);
			for (int i = 0; i < next.start - block.start - block.length; i++) {
				std::cout << '.';
			}
		}
	}
}

bool find_free_space(const data &pInput, int pLength, int &pFreeSpacePos, int &pIndex) {
	int guess = 0;
	for (int i = 0; i < pInput.size() - 1; i++) {
		const memblock &prev = pInput[i];
		const memblock &next = pInput[i + 1];
		int nextFree = prev.start + prev.length;
		if (next.start - nextFree >= pLength) {
			pFreeSpacePos = nextFree;
			pIndex = i + 1;
			return true;
		}
	}
	return false;
}

bool find_free_space(const data &pInput, int &pFreeSpacePos, int &pIndex) {
	return find_free_space(pInput, 1, pFreeSpacePos, pIndex);
}

void part1(data pInput) {
	// preprocessing
	int freeSpace, insertIndex;
	while (find_free_space(pInput, freeSpace, insertIndex)) {
		memblock &last = pInput.back();
		last.length--;

		if (last.length < 1) {
			pInput.pop_back();
		}

		bool inserted = false;
		if (insertIndex > 0) {
			memblock &match = pInput[insertIndex - 1];
			if (match.id == last.id) {
				match.length++;
				inserted = true;
			}
		}

		if (!inserted) {
			pInput.emplace(pInput.begin() + insertIndex, last.id, freeSpace, 1);
		}
	}

	long long sum = 0;
	for (const auto &block : pInput) {
		// the memory blocks are contiguous, so
		// the arithmetic progression sum can
		// be used to quickly calculate its local checksum
		sum += (long long)(block.start * 2 + block.length - 1) * block.length * block.id / 2;
	}

	std::cout << "part 1: " << sum;
}

void part2(data pInput) {
	int fileID = pInput.back().id;
	
	// preprocessing
	int freeSpace, insertIndex;
	while (fileID > 0) {
		int blockIndex = -1;
		for (int i = 0; i < pInput.size(); i++) {
			if (pInput[i].id == fileID) {
				blockIndex = i;
				break;
			}
		}

		if (blockIndex < 0) {
			std::cout << "???";
			exit(1);
		}

		memblock &block = pInput[blockIndex];

		if (find_free_space(pInput, block.length, freeSpace, insertIndex)) {
			if (insertIndex == blockIndex) {
				block.start = freeSpace;
			} else if (insertIndex < blockIndex) {
				memblock copy = { block.id, freeSpace, block.length };
				pInput.erase(pInput.begin() + blockIndex);
				pInput.insert(pInput.begin() + insertIndex, copy);
			}
			else {
				fileID--;
			}
		}
		else {
			fileID--;
		}
	}

	long long sum = 0;
	for (const auto &block : pInput) {
		// same reason as part 1
		sum += (long long)(block.start * 2 + block.length - 1) * block.length * block.id / 2;
	}

	std::cout << "part 2: " << sum;
}