#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
std::ifstream fin("input.txt");

typedef long long num;

struct point {
    num i = 0, j = 0;
};

typedef std::vector<std::string> map;
typedef std::vector<point> moves;

struct data {
    map mapData;
    point player = { 0, 0 };
    moves moveData;
};

void part1(data);
void part2(const data &);

int main()
{
    data input;
    
    std::string line;
    num i = 0;

    while (std::getline(fin, line)) {
        num j = 0;
    
        bool moveData = false;
        for (char &ch : line) {
            switch (ch) {
                case '@':
                    input.player = { i, j };
                    ch = '.';
                    break;
                case '<':
                    moveData = true;
                    input.moveData.push_back({ 0, -1 });
                    break;
                case 'v':
                    moveData = true;
                    input.moveData.push_back({ 1, 0 });
                    break;
                case '^':
                    moveData = true;
                    input.moveData.push_back({ -1, 0 });
                    break;
                case '>':
                    moveData = true;
                    input.moveData.push_back({ 0, 1 });
                    break;
            }
            j++;
        }

        if (!moveData) {
            input.mapData.push_back(line);
        }

        i++;
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

bool resolve_movement(map &pMap, point &pMover, const point &pDelta, bool pSimulate = false) {
    point nextPos = { pMover.i + pDelta.i, pMover.j + pDelta.j };
    point box = nextPos;
    char &ch = pMap[nextPos.i][nextPos.j];
    bool vertical = pDelta.i != 0;
    bool pushingBigBox = (ch == '[' || ch == ']');
    if (ch == '.') {
        if (!pSimulate) {
            pMover = nextPos;
        }
        return true;
    }
    else if (ch == 'O' && resolve_movement(pMap, box, pDelta)) {
        if (!pSimulate) {
            ch = '.';
            pMap[box.i][box.j] = 'O';
            pMover = nextPos;
        }
        return true;
    }
    else if (pushingBigBox) {
        bool leftSide = (ch == '[');
        if (!vertical && resolve_movement(pMap, box, pDelta)) {
            if (!pSimulate) {
                pMap[box.i][box.j] = ch;
                ch = '.';
                pMover = nextPos;
            }
            return true;
        }
        else if (vertical) {
            point otherSide = { box.i, box.j + (leftSide ? 1 : -1) };
            char &ch2 = pMap[otherSide.i][otherSide.j];
            if (resolve_movement(pMap, box, pDelta, true) && resolve_movement(pMap, otherSide, pDelta, true)) {
                if (!pSimulate) {
                    resolve_movement(pMap, box, pDelta);
                    resolve_movement(pMap, otherSide, pDelta);
                    pMap[box.i][box.j] = ch;
                    pMap[otherSide.i][otherSide.j] = ch2;
                    ch = '.';
                    ch2 = '.';
                    pMover = nextPos;
                }
                return true;
            }
        }
    }
    return false;
}

bool resolve_player_movement(data &pInput, const point &pDelta) {
    return resolve_movement(pInput.mapData, pInput.player, pDelta);
}

num perform_movement(data &pInput) {
    for (const auto &move : pInput.moveData) {
        resolve_player_movement(pInput, move);
    }

    num score = 0;
    num i = 0;
    for (const auto &line : pInput.mapData) {
        num j = 0;
        for (const char &ch : line) {
            if (ch == 'O' || ch == '[') {
                score += i * 100 + j;
            }
            j++;
        }
        i++;
    }

    return score;
}

void part1(data pInput) {
    std::cout << "part 1: " << perform_movement(pInput);
}

void part2(const data &pInput) {
    data bigData;
    bigData.player = { pInput.player.i, pInput.player.j * 2 };
    bigData.moveData = pInput.moveData;

    for (const auto &line : pInput.mapData) {
        std::ostringstream builder;
        for (const char &ch : line) {
            switch (ch) {
                case '#':
                    builder << "##";
                    break;
                case '.':
                    builder << "..";
                    break;
                case 'O':
                    builder << "[]";
                    break;
            }
        }
        bigData.mapData.emplace_back(builder.str());
    }

    std::cout << "part 2: " << perform_movement(bigData);
}