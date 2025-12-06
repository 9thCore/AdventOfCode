#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARBITRARY_EQUATION_MAX_COUNT 1000
#define ARBITRARY_EQUATION_MAX_LENGTH 10
#define ARBITRARY_INPUT_LINE_LENGTH 3800

typedef unsigned long long number_t;
#define NUMBER_FORMAT "%llu"
#define NUMBER_CAST(string) atoll(string)

typedef struct {
    number_t numbers[ARBITRARY_EQUATION_MAX_LENGTH];
    int length;
    char operation;
} equation_t;

typedef struct {
    equation_t equations[ARBITRARY_EQUATION_MAX_COUNT];
    int equation_count;
} program_input;

void part1(program_input);
void part2(program_input);

int main(int argc, char **argv)
{
    program_input input = {};
    int equation_number_index = 0;

    char line[ARBITRARY_INPUT_LINE_LENGTH];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        equation_t *equation = input.equations;

        char *p = strtok(line, " ");
        while (p) {
            if (*p == '\n') {
                break;
            }

            if (*p == '+'
            || *p == '*') {
                equation->operation = *p;
            } else {
                equation->numbers[equation_number_index] = NUMBER_CAST(p);
            }

            p = strtok(NULL, " ");
            equation++;
        }

        input.equation_count = equation - input.equations;
        equation_number_index++;
    }

    for (int i = 0; i < input.equation_count; i++) {
        input.equations[i].length = equation_number_index - 1;
    }

    part1(input);
    part2(input);

    return EXIT_SUCCESS;
}

number_t compute(equation_t equation) {
    int add_or_multiply = equation.operation == '+';

    number_t result = (add_or_multiply ? 0 : 1);
    for (int i = 0; i < equation.length; i++) {
        if (add_or_multiply) {
            result += equation.numbers[i];
        } else {
            result *= equation.numbers[i];
        }
    }

    return result;
}

void part1(program_input input) {
    number_t answer = 0;

    for (int i = 0; i < input.equation_count; i++) {
        answer += compute(input.equations[i]);
    }

    printf("The answer to part 1 is: " NUMBER_FORMAT "\n", answer);
}

void part2(program_input input) {
    number_t answer = 0;

    printf("The answer to part 2 is: " NUMBER_FORMAT "\n", answer);
}