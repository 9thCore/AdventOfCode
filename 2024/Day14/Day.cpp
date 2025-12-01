#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
std::ifstream fin("input.txt");

typedef long long num;

struct point {
    num i = 0, j = 0;
};

struct robot {
    point position, velocity;
};

typedef std::vector<robot> data;

const point PLAYABLE_AREA = { 103, 101 };

void part1(data);
void part2(data);

int main()
{
    data input;
    
    std::string line;
    std::smatch match;
    std::regex reg("p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)");

    while (std::getline(fin, line)) {
        if (!std::regex_match(line, match, reg)) {
            exit(1);
        }

        input.emplace_back();
        input.back().position.j = std::stoll(match[1]);
        input.back().position.i = std::stoll(match[2]);
        input.back().velocity.j = std::stoll(match[3]);
        input.back().velocity.i = std::stoll(match[4]);
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

void print(const data &pInput, std::ostream &pOut = std::cout, char pDefault = '.') {
    for (int i = 0; i < PLAYABLE_AREA.i; i++) {
        for (int j = 0; j < PLAYABLE_AREA.j; j++) {
            int cnt = 0;
            for (auto &robot : pInput) {
                if (robot.position.i == i && robot.position.j == j) {
                    cnt++;
                }
            }
            if (cnt) {
                pOut << cnt;
            }
            else {
                pOut << pDefault;
            }
        }
        pOut << "\n";
    }
}

num constrain(num a, num b) {
    while (a < 0) {
        a += b;
    }
    while (a >= b) {
        a -= b;
    }
    return a;
}

num compute_safety_in(const data &pInput, int pQuadrant) {
    bool left = pQuadrant & 1;
    bool top = pQuadrant & 2;
    num count = 0;
    for (const auto &robot : pInput) {
        bool horizontal = (robot.position.j < PLAYABLE_AREA.j / 2 && left)
            || (robot.position.j > PLAYABLE_AREA.j / 2 && !left);

        bool vertical = (robot.position.i < PLAYABLE_AREA.i / 2 && top)
            || (robot.position.i > PLAYABLE_AREA.i / 2 && !top);

        if (horizontal && vertical) {
            count++;
        }
    }
    return count;
}

void part1(data pInput) {
    for (auto &robot : pInput) {
        robot.position.i = constrain(robot.position.i + robot.velocity.i * 100, PLAYABLE_AREA.i);
        robot.position.j = constrain(robot.position.j + robot.velocity.j * 100, PLAYABLE_AREA.j);
    }

    num safety = 1;
    for (int quadrant = 0; quadrant < 4; quadrant++) {
        safety *= compute_safety_in(pInput, quadrant);
    }

    std::cout << "part 1: " << safety;
}

void part2(data pInput) {
    std::ofstream fout("output.txt");

    int i = 0;
    while (i++ < 10000) {
        for (auto &robot : pInput) {
            robot.position.i = constrain(robot.position.i + robot.velocity.i, PLAYABLE_AREA.i);
            robot.position.j = constrain(robot.position.j + robot.velocity.j, PLAYABLE_AREA.j);
        }

        fout << "Step: " << i << "\n";
        print(pInput, fout, ' ');
        fout << "\n\n\n\n\n\n\n\n";
    }
    
    fout << "part 2: idk lol go figure it out";
    // all manual this go around!
}