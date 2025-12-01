#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <array>
#include <queue>
std::ifstream fin("input.txt");

typedef long long num;

struct point {
    num i, j;
};

constexpr point SIZE = { 71, 71 };
constexpr point DELTA[] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

struct cell {
    bool corrupted = 0;
};

typedef std::array<std::array<cell, SIZE.j>, SIZE.i> map;
typedef std::vector<point> bytes;

struct data {
    map mapData;
    bytes byteData;
};

void part1(data);
void part2(data);

int main()
{
    data input;

    std::regex byteRegex("^(\\d+),(\\d+)$");
    std::smatch match;
    
    std::string line;
    while (std::getline(fin, line)) {
        if (!std::regex_match(line, match, byteRegex)) {
            exit(1);
        }

        input.byteData.push_back({ std::stoll(match[1]), std::stoll(match[2]) });
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

bool valid(const point &pPoint) {
    return pPoint.i >= 0 && pPoint.i < SIZE.i
        && pPoint.j >= 0 && pPoint.j < SIZE.j;
}

num bfs(data pInput) {
    struct node {
        num distance;
        point pos;
    };

    std::queue<node> queue;

    queue.push({ 0, {0, 0} });
    pInput.mapData[0][0].corrupted = 1;

    while (!queue.empty()) {
        node current = queue.front();
        queue.pop();

        if (current.pos.i == SIZE.i - 1 && current.pos.j == SIZE.j - 1) {
            return current.distance;
        }

        for (int dir = 0; dir < 4; dir++) {
            point nextPos = { current.pos.i + DELTA[dir].i, current.pos.j + DELTA[dir].j };
            if (!valid(nextPos)
                || pInput.mapData[nextPos.i][nextPos.j].corrupted) {
                continue;
            }

            queue.push({ current.distance + 1, nextPos });
            pInput.mapData[nextPos.i][nextPos.j].corrupted = 1;
        }
    }

    return -1;
}

void part1(data pInput) {
    num simulationsLeft = 1024;

    for (auto &byte : pInput.byteData) {
        if (simulationsLeft-- < 1) {
            break;
        }

        pInput.mapData[byte.i][byte.j].corrupted = 1;
    }

    std::cout << "part 1: " << bfs(pInput);
}

void part2(data pInput) {
    for (auto &byte : pInput.byteData) {
        pInput.mapData[byte.i][byte.j].corrupted = 1;

        if (bfs(pInput) == -1) {
            std::cout << "part 2: " << byte.i << ',' << byte.j;
            return;
        }
    }

    std::cout << "part2: ???,???";
}