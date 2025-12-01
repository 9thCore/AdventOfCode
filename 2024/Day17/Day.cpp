#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <queue>
std::ifstream fin("input.txt");

typedef long long num;

struct data {
    num registerA = 0, registerB = 0, registerC = 0, pointer = 0;
    std::vector<num> program;
    std::queue<num> output;
};

void part1(data);
void part2(data);

int main()
{
    data input;

    std::regex regexes[] = {
        std::regex("^Register A: (.+)$"),
        std::regex("^Register B: (.+)$"),
        std::regex("^Register C: (.+)$"),
        std::regex("^Program: "),
        std::regex("([^,]+),?")
    };

    std::smatch match;
    
    std::string line;
    while (std::getline(fin, line)) {
        if (std::regex_match(line, match, regexes[0])) {
            input.registerA = std::stoll(match[1]);
        }
        else if (std::regex_match(line, match, regexes[1])) {
            input.registerB = std::stoll(match[1]);
        }
        else if (std::regex_match(line, match, regexes[2])) {
            input.registerC = std::stoll(match[1]);
        }
        else {
            std::regex_search(line, match, regexes[3]);
            line = match.suffix();
            while (std::regex_search(line, match, regexes[4])) {
                input.program.push_back(std::stoll(match[1]));
                line = match.suffix();
            }
        }
    }

    part1(input);
    std::cout << "\n";
    part2(input);
}

bool get_op(data &pInput, num &pOpcode, num &pOperand) {
    if (pInput.pointer + 1 >= pInput.program.size()) {
        return false;
    }

    pOpcode = pInput.program[pInput.pointer];
    pOperand = pInput.program[pInput.pointer + 1];
    pInput.pointer += 2;

    return true;
}

num parse_operand(const data &pInput, num pOperand) {
    switch (pOperand) {
    case 4:
        return pInput.registerA;
    case 5:
        return pInput.registerB;
    case 6:
        return pInput.registerC;
    case 7:
        exit(1);
    default:
        return pOperand; // 0-3, assumedly
    }
}

num divide(const data &pInput, num pValue) {
    return pInput.registerA >> pValue;
}

void perform_op(data &pInput, num pOpcode, num pOperand) {
    num literal = parse_operand(pInput, pOperand);
    switch (pOpcode) {
    case 0: // adv
        pInput.registerA = divide(pInput, literal);
        return;
    case 1: // bxl
        pInput.registerB = pInput.registerB ^ literal;
        return;
    case 2: // bxl
        pInput.registerB = literal % 8;
        return;
    case 3: // jnz
        if (pInput.registerA != 0) {
            pInput.pointer = literal;
        }
        return;
    case 4: // bxc
        pInput.registerB = pInput.registerB ^ pInput.registerC;
        return;
    case 5: // out
        pInput.output.push(literal % 8);
        return;
    case 6: // bdv
        pInput.registerB = divide(pInput, literal);
        return;
    case 7: // cdv
        pInput.registerC = divide(pInput, literal);
        return;
    default:
        exit(1);
    }
}

void print_output(data &pInput) {
    while (!pInput.output.empty()) {
        int value = pInput.output.front();
        pInput.output.pop();
        std::cout << value;
        if (!pInput.output.empty()) {
            std::cout << ',';
        }
    }
}

void part1(data pInput) {
    num opcode, operand;
    std::cout << "part 1: ";
    while (get_op(pInput, opcode, operand)) {
        perform_op(pInput, opcode, operand);
    }
    print_output(pInput);
}

void part2(data pInput) {
    // dummy
}